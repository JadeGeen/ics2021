#include <isa.h>
#include <cpu/difftest.h>

extern const char* regs[];

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
	for(int i=0;i<32;i++){
		if(!difftest_check_reg(regs[i],pc,ref_r->gpr[i]._32,cpu.gpr[i]._32))
				return false;
	}
   	return true;
}

void isa_difftest_attach() {
}
