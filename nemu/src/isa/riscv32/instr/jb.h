def_EHelper(jal){
	rtl_li(s, ddest, 4 + s->pc);
	rtl_j(s, s->pc + id_src1->imm);
}

/*
def_EHelper(jalr){
	s->dnpc = *dsrc1 + id_src2->imm;
	rtl_li(s, ddest, 4 + s->pc);
}
*/
