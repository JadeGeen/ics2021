#include <isa.h>
#include <cpu/cpu.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "sdb.h"
#include "watchpoint.h"

static int is_batch_mode = false;

void init_regex();
void init_wp_pool();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}


static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);

#include <stdio.h>
#include <stdlib.h>

static int cmd_si(char *args){
	if (args==NULL){
		cpu_exec(1);
	}
	else{
		int N;
		N = atoi(args);
		cpu_exec(N);
	}
	return 0;
}

static int cmd_info(char *args){
	if (args[0]=='r'){
		isa_reg_display();
	}
	else if(args[0]=='w'){
		extern void print_wp();
		print_wp();
	}
	return 0;
}

static int cmd_x(char *args){
	char *arg1 = strtok(NULL," ");
	int N;
	N = atoi(arg1);
	char *arg2 = strtok(NULL," ");
	extern  word_t paddr_read(paddr_t addr,int len);
	uint32_t addr;
	bool valid=NULL;
	addr = expr(arg2,&valid);
	if(valid==true){
	for (int i=0;i<N;i++){
		uint32_t data;
		data=paddr_read(addr,4);
		printf("0x%08x"":""0x%08x\n",addr,data);
		addr=addr+4;
	}
	}
	return 0;

}

static int cmd_p(char *args){
	bool valid = NULL;
	uint32_t res=expr(args,&valid);
	printf("%u\n",res);
	return 0;

}

static int cmd_w(char *args){
	extern WP* new_wp(char *expr,uint32_t value);
	bool valid=NULL;
	uint32_t value=expr(args,&valid);
	if (valid==true){
		WP* new=new_wp(args,value);
		printf("The index of watchpoint is %d\n",new->NO);
	}
	else{
		printf("Expression invalid\n");
	}
	return 0;
}

static int cmd_d(char *args){
	int N=atoi(args);
	extern void free_wp(int N);
	free_wp(N);
	return 0;
}

static struct {
  const char *name;
  const char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si", "Let the program execute N instuctions in a single step and then pause execution. When N is not given, the default is 1.", cmd_si},
  {"info", "info r: print register status,info w:print watching point status", cmd_info },
  {"x", "x N EXPR: evaluate the expression, using the result as the staring memory address", cmd_x },
  {"p", "p EXPR: evaluate the expression", cmd_p },
  {"w", "w EXPR: suspend the program execution when the value of the expression changes", cmd_w },
  {"d","d N: delete the monitor point labeled N", cmd_d},

  /* TODO: Add more commands */

};

#define NR_CMD ARRLEN(cmd_table)

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void sdb_set_batch_mode() {
  is_batch_mode = true;
}

void sdb_mainloop() {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef CONFIG_DEVICE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) {
			nemu_state.state=NEMU_QUIT;
			return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}

void init_sdb() {
  /* Compile the regular expressions. */
  init_regex();

  /* Initialize the watchpoint pool. */
  init_wp_pool();
}
