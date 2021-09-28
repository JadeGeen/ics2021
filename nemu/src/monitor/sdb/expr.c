#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum {
	TK_NOTYPE = 256, TK_EQ, TK_NUMBER, TK_NEG,

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

	{" +", TK_NOTYPE,0},    // spaces
	{"\\+", '+',1},		    // plus
	{"-", '-',1},			// minus
	{"\\*", '*',2},			// multiply
	{"/", '/',2 },			// divide
	{"[0-9]+", TK_NUMBER,0}, // number
	{"\\(", '(',0},
	{"\\)", ')',0},
	{"==", TK_EQ,0},	  // equal
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
						tokens[nr_token].type=TK_NUMBER;
						tokens[nr_token].priority=rules[i].priority;
						for(int j=0;j<substr_len;j++){
							tokens[nr_token].str[j]=substr_start[j];
						}
						nr_token++;
						break;
					case TK_EQ:
						tokens[nr_token].type=TK_EQ;
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

bool check_parentheses(int p,int q);
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
			if(tokens[i].type=='-'){
				if(i == 0 || tokens[i - 1].type == '+' || tokens[i - 1].type == '-' || tokens[i - 1].type == '*' || tokens[i - 1].type == '/'){
					tokens[i].type=TK_NEG;
					tokens[i].priority=3;
				}
			}
		}

		uint32_t res=eval(0,nr_token-1);
		return res;

		//TODO();

		//return 0;
	}
}


bool check_parentheses(int p,int q){
	int check=0;
	for(int i=p;i<=q;i++){
		if (tokens[i].type=='('){
			check++;
		}
		else if(tokens[i].type==')'){
			check--;
		}
		if(check<0){
			return false;
		}
	}
	if(check==0){
		return true;
	}
	else{
		return false;
	}

}

int find_dominant_op(int p,int q){
	int start=p+1;
	int end=q;
	int dominant_op=p;
	int min_priority=tokens[p].priority;
	while(start<end){
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
			start++;
		}
	}
	return dominant_op;
}

#include<stdio.h>
#include<stdlib.h>

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
		uint32_t number;
		number=(uint32_t)atoi(tokens[p].str);
		return number;
	}
	else if (check_parentheses(p, q) == true ) {
		/* The expression is surrounded by a matched pair of parentheses.
		 * If that is the case, just throw away the parentheses.
		 */
		if(tokens[p].type=='(' && tokens[q].type==')'){
			return eval(p + 1, q - 1);
		}
		else {
			int op = find_dominant_op(p,q); 
			uint32_t val1 = eval(p, op - 1);
			uint32_t val2 = eval(op + 1, q);

			switch (tokens[op].type) {
				case '+': return val1 + val2;
				case '-': return val1 - val2;
				case '*': return val1 * val2;
				case '/': return val1 / val2;
				case TK_NEG: return -val2;
				default: assert(0);
			}
		}
	}
	else{
		return 0;
	}
}
