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

extern int fs_open(const char *pathname, int flags, int mode);
extern size_t fs_read(int fd, void *buf, size_t len);
extern size_t fs_write(int fd, void *buf, size_t len);
extern size_t fs_lseek(int fd, size_t offset, int whence);
extern int fs_close(int fd);

void do_syscall(Context *c) {
	uintptr_t a[4];
	a[0] = c->GPR1;
	a[1] = c->GPR2;
	a[2] = c->GPR3;
	a[3] = c->GPR4;
	Log("syscall ID = %d, arg1 = %d, arg2 = %d, arg3 = %d", a[0], a[1], a[2], a[3]);
	switch (a[0]) {
		case SYS_exit://exit
			halt(0);
			c->GPRx = 0;
			break;
		case SYS_yield://yield
			yield();
			//printf("sys_yield\n");
			c->GPRx = 0;
			break;
		case SYS_open:
			c->GPRx = fs_open((const char*)a[1], a[2], a[3]); 
			break;
		case SYS_read:
			c->GPRx = fs_read(a[1], (void*)a[2], a[3]);
			break;
		case SYS_write:
			c->GPRx = sys_write(a[1], (char*)a[2], a[3]);
			break;
		case SYS_close:
			c->GPRx = fs_close(a[1]);
			break;
		case SYS_lseek:
			c->GPRx = fs_lseek(a[1], a[2], a[3]);
			break;
		case SYS_brk:
			c->GPRx = 0;
			break;
		default: panic("Unhandled syscall ID = %d", a[0]);
	}
}
