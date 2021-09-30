typedef struct watchpoint {
	int NO;
	char *expr;
	uint32_t value;
	bool used;
	struct watchpoint *next;

	/* TODO: Add more members if necessary */

} WP;
