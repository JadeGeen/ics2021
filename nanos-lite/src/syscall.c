#include <common.h>
#include "syscall.h"

int sys_write(int fd, char *buf, size_t count){
	if(fd==1||fd==2){
		size_t i = 0;
		for(;i<count;i++){
			putch(*buf);
			buf++;
		}
		return i;
	}
	return -1;
}

void do_syscall(Context *c) {
	uintptr_t a[4];
	a[0] = c->GPR1;
	a[1] = c->GPR2;
	a[2] = c->GPR3;
	a[3] = c->GPR4;
	printf("syscall %d\n",a[0]);
	switch (a[0]) {
		/*case 0://exit
			halt(0);
			c->GPRx = 0;
			break;*/
		case SYS_yield://yield
			//yield();
			printf("sys_yield\n");
			c->GPRx = 0;
			break;
		case SYS_write:
			c->GPRx = sys_write(a[1], (char*)a[2], a[3]);
			break;
		default: panic("Unhandled syscall ID = %d", a[0]);
	}
}
