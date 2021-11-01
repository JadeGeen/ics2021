#include <cpu/decode.h>
#include "../local-include/rtl.h"

#define INSTR_LIST(f) f(lui) f(lw) f(sw) f(inv) f(nemu_trap) f(auipc) f(addi) f(jal) f(jalr) f(add) f(sub) f(sll) f(slt) f(sltu) f(xor) f(or) f(sltiu) f(andi) f(srai) f(beq) f(bne) f(blt) f(sb) f(lbu)

def_all_EXEC_ID();
