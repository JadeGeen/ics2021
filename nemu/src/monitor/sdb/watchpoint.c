#include "sdb.h"

#define NR_WP 32

typedef struct watchpoint {
	int NO;
	char expr[32];
	uint32_t value;
	bool used;
	struct watchpoint *next;

	/* TODO: Add more members if necessary */

} WP;

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
	int i;
	for (i = 0; i < NR_WP; i ++) {
		wp_pool[i].NO = i;
		wp_pool[i].next = &wp_pool[i + 1];
	}
	wp_pool[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP* new_wp(char *expr, uint32_t value){
	if(free_==NULL){
		assert(0);
	}
	else{
		WP *newwatchpoint=free_;
		free_=free_->next;
		newwatchpoint->next=NULL;
		strcpy(newwatchpoint->expr,expr);
		newwatchpoint->value=value;
		newwatchpoint->used=true;
		if(head==NULL){
			head=newwatchpoint;
		}
		else{
			newwatchpoint->next=head;
			head=newwatchpoint;
		}
		return newwatchpoint;
	}

}

void free_wp(int N){
	WP *p=head;
	if(head==NULL){
		assert(0);
		printf("There is no watchpoint working\n");
	}
	else if(p->NO==N){
		head= head->next;
		p->value=0;
		//p->expr={ };
		p->next=free_;
		free_=p;
	}
	else{
		WP *q=head;
		p=p->next;
		while(p!=NULL){
			if(p->NO==N){
				q->next=p->next;
				p->used=false;
				p->value=0;
				//p->expr={ };
				p->next=free_;
				free_=p;
				printf("Successfully delete watchpoint %d\n",N);
				break;
			}
			p=p->next;
			q=q->next;
		}
		if(p==NULL){
			printf("Watchpoint %d does not exist\n",N);
		}
	}
}

extern word_t expr(char *e,bool *success);

void check_watchpoint(){
	WP *p;
	int checkcount=0;
	for(p=head;p!=NULL;p=p->next){
		bool success=NULL;
		uint32_t check=expr(p->expr,&success);
		if(check==p->value){
			continue;
		}
		else{
			checkcount++;
			Log("Expression has changed at watchpoint %d\n",p->NO);
		}
	}
	if(checkcount>0){
		nemu_state.state=NEMU_STOP;
	}
}

void print_wp(){
	WP *p=head;
	if(p==NULL){
		printf("There is no watchpoint working\n");
	}
	else{
		while(p!=NULL){
			printf("%d  %s  0x%08x\n",p->NO,p->expr,p->value);
			p=p->next;
		}
	}
}
