#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  		kbd->keydown = ((inl(KBD_ADDR))&0x8000)?true:false;
  		kbd->keycode = (inl(KBD_ADDR))&0xff;
}
