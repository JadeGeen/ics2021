def_EHelper(lui) {
	rtl_li(s, ddest, id_src1->imm);
}

def_EHelper(auipc){
	rtl_li(s, ddest, id_src1->imm + s->pc);	
}

def_EHelper(addi){
	rtl_addi(s, ddest, dsrc1, id_src2->imm);
}

def_EHelper(sltiu){
	int32_t temp = *dsrc1;
	int32_t temp2 = id_src2->imm;
	if(temp < temp2)
		rtl_li(s, ddest, 1);
	else
		rtl_li(s, ddest, 0);
}

def_EHelper(add){
	rtl_add(s, ddest, dsrc1, dsrc2);
}

def_EHelper(sub){
	rtl_sub(s, ddest, dsrc1, dsrc2);
}

def_EHelper(sltu){
	if(*dsrc1<*dsrc2)
		rtl_li(s, ddest, 1);
	else
		rtl_li(s, ddest, 0);
}

def_EHelper(xor){
	rtl_xor(s, ddest, dsrc1, dsrc2);
}

def_EHelper(or){
	rtl_or(s, ddest, dsrc1, dsrc2);
}
