#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536] = {};
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

static int buf_index=0;

#include<time.h>

static uint32_t choose(uint32_t n){
	uint32_t x = rand()%n;
	return x;
}


static void gen(char x){
	if(buf_index<65535){
	switch(x){
		case '(':
		case ')':
			buf[buf_index]=x;
			buf_index++;
			break;
		default:break;
	}
	}
	return;
}

static void gen_num(){
	uint32_t newnum=choose(65535)+1;
	char newnumber[5];
	sprintf(newnumber,"%u",newnum);
	int length = strlen(newnumber);
	for (int i = 0;i<length;i++){
		if(buf_index>=65535){
			break;
		}
		buf[buf_index]=newnumber[i];
		buf_index++;
	}
	return;
}

static void gen_rand_op(){
	if(buf_index<65535){
	switch (choose(4)){
		case 0:
			buf[buf_index]='+';
			buf_index++;
			break;
		case 1:
			buf[buf_index]='-';
			buf_index++;
			break;
		case 2:
			buf[buf_index]='*';
			buf_index++;
			break;
		default:
			buf[buf_index]='/';
			buf_index++;
			break;
	}
	}
	return;
}

static void gen_rand_expr() {
	switch (choose(3)) {
		case 0: gen_num(); break;
		case 1: gen('('); gen_rand_expr(); gen(')'); break;
		default: gen_rand_expr(); gen_rand_op(); gen_rand_expr(); break;								   
	}
	buf[buf_index] = '\0';
	return;
}

int main(int argc, char *argv[]) {
	int seed = time(0);
	srand(seed);
	int loop = 1;
	if (argc > 1) {
		sscanf(argv[1], "%d", &loop);
	}
	int i;
	for (i = 0; i < loop; i ++) {
		gen_rand_expr();

		sprintf(code_buf, code_format, buf);

		FILE *fp = fopen("/tmp/.code.c", "w");
		assert(fp != NULL);
		fputs(code_buf, fp);
		fclose(fp);

		int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
		if (ret != 0) continue;

		fp = popen("/tmp/.expr", "r");
		assert(fp != NULL);

		int result;
		fscanf(fp, "%d", &result);
		pclose(fp);

		printf("%u %s\n", result, buf);
	}
	return 0;
}
