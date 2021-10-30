#include <isa.h>
#include <memory/paddr.h>

// this is not consistent with uint8_t
// but it is ok since we do not access the array directly
static const uint32_t img [] = {
	//0x00000413,          	//li	s0,0
//	0x00009117,         	//auipc	sp,0x9
//	0xffc10113,
//	0x00c000ef ,         	//jal	ra,80000018 <_trm_init>
//	0x00000513  ,        	//li	a0,0
//	0x00008067  ,		//        	ret
//	0x80000537   ,       	//lui	a0,0x80000
//	0xff010113    ,      	//	addi	sp,sp,-16
//	0x03850513     ,     	//	addi	a0,a0,56 # 80000038 <_end+0xffff7038>
//	0x00112623      ,    	//	sw	ra,12(sp)
//	0xfe9ff0ef       ,   	//	jal	ra,80000010 <main>
//	0x00050513        ,  	//	mv	a0,a0
//	0x0000006b         , 	//	0x6b
//	0x0000006f          ,	//	j	80000034 <_trm_init+0x1c>
  0x800002b7,  // lui t0,0x80000
  0x0002a023,  // sw  zero,0(t0)
  0x0002a503,  // lw  a0,0(t0)
  0x0000006b,  // nemu_trap
};

static void restart() {
  /* Set the initial program counter. */
  cpu.pc = RESET_VECTOR;

  /* The zero register is always 0. */
  cpu.gpr[0]._32 = 0;
}

void init_isa() {
  /* Load built-in image. */
  memcpy(guest_to_host(RESET_VECTOR), img, sizeof(img));

  /* Initialize this virtual computer system. */
  restart();
}
