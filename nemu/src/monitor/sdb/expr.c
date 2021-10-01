#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum {
	TK_NOTYPE = 256, TK_EQ, TK_NEQ, TK_NUMBER, TK_HEX, TK_REG, TK_NEG, TK_AND, TK_OR,TK_DEREF, TK_GEQ, TK_LEQ, 

	/* TODO: Add more token types */

};

static struct rule {
	const char *regex;
	int token_type;
	int priority;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +", TK_NOTYPE,10},    // spaces
	{"\\(", '(',10},
	{"\\)", ')',10},
	{"\\$[$,a-z][0-9]+",TK_REG,10},
	{"0x[0-9,a-f,A-F]+",TK_HEX,10},
	{"[0-9]+", TK_NUMBER,10}, // number
	{"\\+", '+',4},		    // plus
	{"-", '-',4},			// minus
	{"\\*", '*',5},			// multiply
	{"/", '/',5 },			// divide
	{">=", TK_GEQ,3},
	{"<=",TK_LEQ,3},
	{"<",'<',3},
	{">",'>',3},
	{"==", TK_EQ,2},	  // equal
	{"!=", TK_NEQ,2},
	{"&&", TK_AND,1},
	{"\\|\\|", TK_OR,0},
	{"!",'!',6},
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for (i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if (ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
	int priority;
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;

	nr_token = 0;

	while (e[position] != '\0') {
		/* Try all rules one by one. */
		for (i = 0; i < NR_REGEX; i ++) {
			if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
						i, rules[i].regex, position, substr_len, substr_len, substr_start);

				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array `tokens'. For certain types
				 * of tokens, some extra actions should be performed.
				 */

				switch (rules[i].token_type) {
					case TK_NOTYPE:
						break;
					case '+':
					case '-':
					case '*':
					case '/':
					case '(':
					case ')': 
						tokens[nr_token].type=rules[i].token_type;
						tokens[nr_token].priority=rules[i].priority;
						nr_token++;
						break;
					case TK_NUMBER:
					case TK_HEX:
						tokens[nr_token].type=rules[i].token_type;
						tokens[nr_token].priority=rules[i].priority;
						strncpy(tokens[nr_token].str,substr_start,substr_len);
						tokens[nr_token].str[substr_len]='\0';
						nr_token++;
						break;
					case TK_REG:
						tokens[nr_token].type=rules[i].token_type;
						tokens[nr_token].priority=rules[i].priority;
						strncpy(tokens[nr_token].str,substr_start+1,substr_len-1);
						tokens[nr_token].str[substr_len-1]='\0';
						nr_token++;
						break;

					case TK_EQ:
					case TK_NEQ:
					case '>':
					case TK_GEQ:
					case '<':
					case TK_LEQ:
					case TK_AND:
					case TK_OR:
						tokens[nr_token].type=rules[i].token_type;
						tokens[nr_token].priority=rules[i].priority;
						nr_token++;
						break;


					default: TODO();
				}

				break;
			}
		}

		if (i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true;
}

int check_parentheses(int p,int q);
int find_dominant_op(int p,int q);
uint32_t eval(int p,int q);


word_t expr(char *e, bool *success) {
	if (!make_token(e)) {
		*success = false;
		return 0;
	}

	/* TODO: Insert codes to evaluate the expression. */
	else{
		*success=true;

		int i;
		for(i=0;i<nr_token;i++){
			if(tokens[i].type=='-' &&(i == 0 || (tokens[i - 1].type != TK_NUMBER && tokens[i - 1].type != TK_HEX && tokens[i - 1].type != TK_REG && tokens[i-1].type!='(' &&tokens[i-1].type!=')'))){
				tokens[i].type=TK_NEG;
				tokens[i].priority=6;
			}
			if(tokens[i].type=='*' &&(i == 0 || (tokens[i - 1].type != TK_NUMBER && tokens[i - 1].type != TK_HEX && tokens[i - 1].type != TK_REG&&tokens[i-1].type!='('&&tokens[i-1].type!=')' ))){
				tokens[i].type = TK_DEREF;
				tokens[i].priority=6;
			}
		}

		uint32_t res=eval(0,nr_token-1);
		return res;

		//TODO();

		//return 0;
	}
}


int check_parentheses(int p,int q){
	int check=0;
	int index=q;
	for(int i=p;i<=q;i++){
		if (tokens[i].type=='('){
			check++;
		}
		else if(tokens[i].type==')'){
			check--;
			if(check==0&&i!=q){
				index=i;
			}
		}
		if(check<0){
			return -1;
		}
	}
	if(check==0&&index==q&&tokens[p].type=='('&&tokens[q].type==')'){
		return 1;
	}
	else if(check==0){
		return 0;
	}
	else{
		return -1;
	}

}

int find_dominant_op(int p,int q){
	int start=p;
	int end=q;
	int dominant_op=p;
	int min_priority=tokens[p].priority;
	while(start<=end){
		if (tokens[start].type=='('){
			int i;
			for(i=start+1;i<=end;i++){
				if (tokens[i].type==')'){
					break;
				}
			}
			start=i+1;
			continue;
		}
		else if (tokens[start].priority<=min_priority){
			dominant_op=start;
			min_priority=tokens[start].priority;
		}
		start++;
	}
	return dominant_op;
}

#include<stdio.h>
#include<stdlib.h>

extern word_t paddr_read(paddr_t addr,int len);

uint32_t eval(int p,int q) {
	if (p > q) {
		/* Bad expression */
		return 0;
	}
	else if (p == q) {
		/* Single token.
		 * For now this token should be a number.
		 * Return the value of the number.
		 */
		switch(tokens[p].type){
			case TK_NUMBER:{
							   uint32_t number;
							   number=(uint32_t)atoi(tokens[p].str);
							   return number;
						   }
			case TK_HEX:{
							uint32_t hex_number;
							hex_number=(uint32_t)strtol(tokens[p].str,NULL,16);
							return hex_number;
						}
			case TK_REG:{
							uint32_t reg_content;
							bool check_success=true;
							reg_content=isa_reg_str2val(tokens[p].str,&check_success);
							if(check_success==true){
								return reg_content;
							}
							else{
								assert(0);
							}
						}
			case TK_NEG:
			case TK_DEREF:
						return 0;
			default:TODO();

		}
	}
	else if (check_parentheses(p, q) == 1 ) {
		/* The expression is surrounded by a matched pair of parentheses.
		 * If that is the case, just throw away the parentheses.
		 */
		return eval(p + 1, q - 1);
	}
	else if (check_parentheses(p,q)==-1){
		printf("Wrong expresssion\n");
		assert(0);
	}
	else{
		int op = find_dominant_op(p,q); 
		uint32_t val1 = eval(p, op - 1);
		uint32_t val2 = eval(op + 1, q);
		//printf("%d\n",op);
		switch (tokens[op].type) {
			case '+': return val1 + val2;
			case '-': return val1 - val2;
			case '*': return val1 * val2;
			case '/': return val1 / val2;
			case TK_NEG: {
							 int index_neg=op;
							 while(tokens[index_neg].type==TK_NEG&&index_neg>=0){
								 val2=-val2;
								 index_neg--;
							 }

							 return val2;
						 }
			case TK_DEREF:{
							  int index_deref=op;
							  while(tokens[index_deref].type==TK_DEREF&&index_deref>=0){
								  val2=paddr_read(val2,4);
								  index_deref--;
							  }
							  return val2;
						  }
			case TK_AND:return val1 && val2;
			case TK_OR:return val1 || val2;
			case TK_EQ:return val1 == val2;
			case TK_NEQ:return val1 != val2;
			case TK_LEQ:return val1 <= val2;
			case TK_GEQ:return val1 >= val2;
			case '<':return val1 < val2;
			case '>':return val1 > val2;
			case '!':return !val2;
			default: assert(0);
		}
	}

}
