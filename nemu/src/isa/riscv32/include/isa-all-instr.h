#include <cpu/decode.h>
#include "../local-include/rtl.h"

#define INSTR_LIST(f) f(lui) f(lw) f(sw) f(inv) f(nemu_trap) f(auipc) f(addi) f(jal)

def_all_EXEC_ID();
