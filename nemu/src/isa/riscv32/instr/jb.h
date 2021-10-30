def_EHelper(jal){
	s->pc = s->pc + id_src2->imm;
	rtl_li(s, ddest, 4 + s->pc);
}

def_EHelper(jalr){
	s->pc = *dsrc1 + id_src2->imm;
	rtl_li(s, ddest, 4 + s->pc);
}
