def_EHelper(jal){
	s->pc = s->pc + id_src2->simm;
	rtl_li(s, ddest, 4 + s->pc);
}
