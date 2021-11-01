#include <cpu/decode.h>
#include "../local-include/rtl.h"

#define INSTR_LIST(f) f(lui) f(lw) f(sw) f(inv) f(nemu_trap) f(auipc) f(addi) f(jal) f(jalr) f(add) f(sub) f(sll) f(slt) f(sltu) f(xor) f(or) f(and) f(mul) f(div) f(rem) f(sltiu) f(xori) f(andi) f(srai) f(beq) f(bne) f(blt) f(bge) f(sb) f(lb) f(lh) f(lbu) f(lhu)

def_all_EXEC_ID();
