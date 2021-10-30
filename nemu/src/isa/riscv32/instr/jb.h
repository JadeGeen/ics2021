def_EHelper(jal){
	rtl_li(s, ddest, 4 + s->pc);
	/*int imm=id_src1->imm;
	imm=(imm<<11)>>11;
	if(imm<0){
		imm=-imm;
		rtl_j(s, s->pc-imm);
	}else*/
		rtl_j(s, s->pc + id_src1->imm);
}

def_EHelper(jalr){
	rtl_li(s, ddest, 4 + s->pc);
	rtl_j(s,*dsrc1+id_src2->imm);
}
