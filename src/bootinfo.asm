;descripe:	some symbols declared in boot.asm(namely a bootloader) and will be refered by kernel.elf
;1 if you want to use the label itsef(namely it's 'equ' value),then name it like '_label'\
		if you want to refer the location tha the label indicate, underline-prefix is not\
		needed when name it, because we treat it as normal variable.
%include "../include/utils.inc"

;we use memory segment 0x6000~0x7000 to store information which we got in real mode, and then parse it
;under protection mode.
gequ base_realmod_info,0x6000
struc realmod_info
_mmemseg_num: 		resb 4
_bbase_meminfo: 		resb 256
endstruc
;the following two lable will be cancelled soon, i will enjoy the struc-support under nasm
gequ _memseg_num,base_realmod_info + _mmemseg_num
gequ _base_meminfo,base_realmod_info + _bbase_meminfo

;扇区是从1开始计数的，到了dimg.c里要减1，因为dd是从0开始计数的。
;这个示意图要随着下面数据的变动及时同步。
;+------------------------------------------------------------------------+
;      _______________________________________________________________________
;sector|  1   |  2   |  3   |   4  |   5  |  6   |   7  |      |      |      |
;      |______boot.bin______|_fix__|_____kernel.elf__________________________| 
;+------------------------------------------------------------------------+

gequ _bootbin_start_sector, 1
gequ _bootbin_occupy_sectors,3
gequ _fiximg_start_sector, (_bootbin_start_sector + _bootbin_occupy_sectors)
gequ _fiximg_occupy_sectors, 1
gequ _kernel_image_start_sector, (_fiximg_start_sector + _fiximg_occupy_sectors)

gequ _gpgdir_base,0x100000

lequ _base_kernel_loaded,0x80000
;base_kernel_reset equ 0x30400
base_kernel_reset equ 0x30400
lequ _base_entrance_kernel,base_kernel_reset+0xc0000000
gequ RAMDISK_BASE, 0x10000
