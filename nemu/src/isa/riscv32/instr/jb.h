def_EHelper(jal){
	rtl_li(s, ddest, 4 + s->pc);
	int imm=id_src1->imm;
	if(imm<0){
		imm=-imm;
		rtl_j(s, s->pc-imm);
	}else
		rtl_j(s, s->pc + id_src1->imm);
}

/*
   def_EHelper(jalr){
   s->dnpc = *dsrc1 + id_src2->imm;
   rtl_li(s, ddest, 4 + s->pc);
   }
   */
