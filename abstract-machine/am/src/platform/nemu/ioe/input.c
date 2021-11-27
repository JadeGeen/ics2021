#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
	if(inl(KBD_ADDR)==AM_KEY_NONE){
  		kbd->keydown = 0;
  		kbd->keycode = AM_KEY_NONE | 0;
  		}
  	else{
  		kbd->keydown = 1;
  		kbd->keycode = inl(KBD_ADDR) | KEYDOWN_MASK;
  		}
  
}
