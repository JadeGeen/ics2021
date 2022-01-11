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
	int distinguish_num = cpu.gpr[17]._32;
	uint32_t exceptionindex = 0;
	switch(distinguish_num){
		case -1:exceptionindex = 1;break;
		case 0:case 1:case 2:case 3:case 4:case 5:case 6:case 7:case 8:case 9:case 10:case 11:case 12:case 13:case 14:case 15:case 16:case 17:case 18:case 19:exceptionindex = 2;break;
		default:Log("unknown distinguish_num:%u",distinguish_num);assert(0);
	}
#ifdef CONFIG_ETRACE
	Log("raise exception no.%u",exceptionindex);
#endif
	rtl_j(s, isa_raise_intr(exceptionindex,s->pc+4));
}
