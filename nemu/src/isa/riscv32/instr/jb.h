def_EHelper(jal){
	rtl_li(s, ddest, 4 + s->pc);
	rtl_j(s, s->pc + id_src1->imm);
}

def_EHelper(jalr){
	rtl_li(s, ddest, 4 + s->pc);
	rtl_j(s, *dsrc1 + id_src2->imm);
}
