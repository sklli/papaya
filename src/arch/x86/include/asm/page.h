#ifndef X86_PAGE_H
#define X86_PAGE_H
#include<valType.h>

#define PAGE_SHIFT 12
#define PAGE_SIZE 0x1000
#define PAGE_MASK (~0xfff)
#define pa_idx(paddr) ((paddr)>>PAGE_SHIFT)
#define pa_pg pa_idx

#define PG_P 1
#define PG_USU 4
#define PG_RWW 2
/*TODO cancell the following two macros, and 'vpg', 'ppg' is a good transfer*/
#define PG_H10(pg_id) (pg_id>>10)
#define PG_L10(pg_id) (pg_id&(0x400-1))

static inline void invlpg(void *vaddr){
	__asm__ __volatile__("invlpg (%0)"::"r"(vaddr));
}

#define FLUSH_TLB __asm__ __volatile__("mov %%cr3, %0\n\t"	\
										"mov %0, %%cr3\n\t"\
										:\
										:"r"(0))

/*get page struct by a virtual address*/
#define __va2page_t(vaddr) (mem_map + (((vaddr) - PAGE_OFFSET) >> 12))
#define __pa2page_t(paddr) (mem_map + ((paddr) >> 12))

/* page table/directory entry ==> linear address of target page */
#define pte2page(pte) ((void *)__va((pte).value & PAGE_MASK))
#define pte2page_t(pte) ( mem_map + (pte).physical )



// >不一定比“|, &"用起来更方便。先局部的用一用，尤其是pte
// >这样，一个普通的整形变成了union，其实是反而隐藏了类型信息。但不妨一试。
//  毕竟pte是特殊的，基础的，读者常常知道pte是个unsigned。
#pragma pack(push)
#pragma pack(1)
union pte{
	int value; 		
	struct {
		unsigned present: 1;
		unsigned writable: 1;
		unsigned user: 1;
		unsigned PWT: 1;
		unsigned PCD: 1;
		unsigned accessed: 1;
		unsigned dirty: 1;
		unsigned : 2;
		unsigned avl: 3;
		unsigned physical: 20;
	};
	struct{
		unsigned flags: 12;	
		unsigned: 20;
	};
};

union linear_addr{
	unsigned long value;
	struct{
		unsigned offset: 12;
		unsigned tbl_idx: 10;
		unsigned dir_idx: 10;
	};
};

union cr3{
	int value; 		
	struct {
		unsigned : 12;
		unsigned physical: 20;
	};
};

union pgerr_code{
	u32 value;
	struct{
		unsigned protection: 1;
		unsigned on_write: 1;
		unsigned from_user: 1;
		unsigned dirty_rsv: 1;
		unsigned instruction: 1;
		unsigned : 27;
	};
	struct{
		unsigned $nopage: 1;
		unsigned $on_read: 1;
		unsigned $in_kernel: 1;
		unsigned: 1;
		unsigned $data: 1;
		unsigned: 27;
	};
};
#pragma pack(pop)

#define PAGE_OFFSET 0XC0000000
#define __pa(vaddr) ((unsigned)(vaddr) - PAGE_OFFSET)
#define __va(paddr) ((unsigned)(paddr) + PAGE_OFFSET)
static inline union pte *__va2pte(void *vaddr, union pte *pgdir){
	if(!pgdir) return 0;

	union linear_addr laddr ;
	laddr.value = (unsigned long)vaddr;
	union pte *table = pte2page( pgdir[laddr.dir_idx] );
	return table + laddr.tbl_idx;
}
#define KV __va





#endif
