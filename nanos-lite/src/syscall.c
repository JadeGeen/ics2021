#include <common.h>
#include "syscall.h"
void do_syscall(Context *c) {
	uintptr_t a[4];
	a[0] = c->GPR1;
	Log("syscall %u",a[0]);
	switch (a[0]) {
		case 0://exit
			halt(0);
			c->GPRx = 0;
			break;
		case 1://yield
			yield();
			c->GPRx = 0;
			break;
		default: panic("Unhandled syscall ID = %d", a[0]);
	}
}
