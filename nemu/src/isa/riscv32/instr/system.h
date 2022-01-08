def_EHelper(csrrs){
	switch(id_src2->imm){
		case 0x341:
			dsrc2 = &(cpu.mepc);
			break;
		case 0x342:
			dsrc2 = &(cpu.mcause);
			break;
		case 0x300:
			dsrc2 = &(cpu.mstatus);
			break;
		case 0x305:
			dsrc2 = &(cpu.mtvec);
			break;
	}
	rtl_addi(s, ddest, dsrc2, 0);
	rtl_or(s, dsrc2, dsrc1, dsrc2);
}

def_EHelper(csrrw){
	switch(id_src2->imm){
		case 0x341:
			dsrc2 = &(cpu.mepc);
			break;
		case 0x342:
			dsrc2 = &(cpu.mcause);
			break;
		case 0x300:
			dsrc2 = &(cpu.mstatus);
			break;
		case 0x305:
			dsrc2 = &(cpu.mtvec);
			break;
	}
	rtl_addi(s, ddest, dsrc2, 0);
	rtl_addi(s, dsrc2, dsrc1, 0);
}

def_EHelper(mret){
	rtl_j(s, cpu.mepc);
}

def_EHelper(ecall){
#ifdef CONFIG_ETRACE
	printf("raise exception no.1 yield\n");
#endif
	rtl_j(s, isa_raise_intr(1,s->pc+4));
}
