// Batch Number 42
// Saurabh Anand  2013A7PS139P
// Priyank Gupta  2013A7PS060P

#ifndef DATA
#define DATA


int no_of_nonterminals;

int no_of_tokens; // check if you are including dollar or not
int parse_table[51][54];
int no_of_rules_of_grammar;
char** tokenName ;


int num_parse_errors;
char parse_errors[50][100];


typedef struct grammar_node* grammar;
typedef struct rule* rule_ptr;
typedef struct node* list;
typedef struct _ParseTree *parseTree ;
typedef struct _ParseTree *abstractSyntaxTree;
typedef struct _stack *stack ;

struct grammar_node
{
	//char grammar_node_name [35];
	int num_of_rules;
	rule_ptr grammar_node_head;
	int* first;
	int first_len;
	int* follow;
	int follow_len;
};
//move eps to struct grammar_node (think why?)

struct rule
{
	int rule_number; // may remove this
	int len_rule;
	int* rule_head;
	int eps;
	list first;
	rule_ptr next_rule;
};

struct node
{
	int num;
	struct node* next_node;
};

struct _ParseTree
{
	int symbol;
	parseTree parent;
	parseTree child[10];
	int childnum;
	int counterChild;
	int ruleNo;
	char value[200];
	int line_no;
};



struct _stack
{
	int value;
	stack next;
};

#endif

