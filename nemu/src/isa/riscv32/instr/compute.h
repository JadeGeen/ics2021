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

def_EHelper(xori){
	rtl_xori(s, ddest, dsrc1, id_src2->imm);
}

def_EHelper(andi){
	rtl_andi(s, ddest, dsrc1, id_src2->imm);
}

def_EHelper(slli){
	uint32_t shamt = (id_src2->imm)&0x1f;
	rtl_slli(s, ddest, dsrc1, shamt);
}

def_EHelper(srli){
	uint32_t shamt = (id_src2->imm)&0x1f;
	rtl_srli(s, ddest, dsrc1, shamt);
}

def_EHelper(srai){
	uint32_t shamt = (id_src2->imm)&0x1f;
	rtl_srai(s, ddest, dsrc1, shamt);
}

def_EHelper(add){
	rtl_add(s, ddest, dsrc1, dsrc2);
}

def_EHelper(sub){
	rtl_sub(s, ddest, dsrc1, dsrc2);
}

def_EHelper(sll){
	rtl_sll(s, ddest, dsrc1, dsrc2);
}	

def_EHelper(slt){
	int32_t temp=*dsrc1;
	int32_t temp2=*dsrc2;
	if(temp<temp2)
		rtl_li(s, ddest, 1);
	else
		rtl_li(s, ddest, 0);
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

def_EHelper(srl){
	rtl_srl(s, ddest, dsrc1, dsrc2);
}

def_EHelper(sra){
	rtl_sra(s, ddest, dsrc1, dsrc2);
}

def_EHelper(or){
	rtl_or(s, ddest, dsrc1, dsrc2);
}

def_EHelper(and){
	rtl_and(s, ddest, dsrc1, dsrc2);
}

def_EHelper(mul){
	rtl_mulu_lo(s, ddest, dsrc1, dsrc2);
}

def_EHelper(mulh){
	rtl_muls_hi(s, ddest, dsrc1, dsrc2);
}

def_EHelper(div){
	rtl_divs_q(s, ddest, dsrc1, dsrc2);
}

def_EHelper(rem){
	rtl_divs_r(s, ddest, dsrc1, dsrc2);
}
