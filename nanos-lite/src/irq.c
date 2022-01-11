#include <common.h>

extern void do_syscall(Context *c);

static Context* do_event(Event e, Context* c) {
	switch (e.event) {
		case 1: Log("yiled OK");break;
		case 2: printf("do_event %d\n",c->GPR1);do_syscall(c);break;
		default: panic("Unhandled event ID = %d", e.event);
	}

	return c;
}

void init_irq(void) {
	Log("Initializing interrupt/exception handler...");
	cte_init(do_event);
}
