// Batch Number 42
// Saurabh Anand  2013A7PS139P
// Priyank Gupta  2013A7PS060P

#ifndef DATA2
#define DATA2

typedef char* buffer;
typedef int buffersize;
typedef struct token tokenInfo;
struct token
{
	char value[200];
	int token_id;
	int line_no;
};


char* B;

int state[50][260];
int lexer_errors;
int stateToToken[45];
char **tokenName;
char **lookup;
int buffer_start_index;
int buffer_end_index;
int lnum;
int k;
int lexeme_index;
int current_state;
FILE* fp;

tokenInfo temp;

enum tokenEnum {
		TK_ASSIGNOP=0,
		TK_COMMENT=1,
		TK_FIELDID=2,
		TK_ID=3,
		TK_NUM=4,
		TK_RNUM=5,
		TK_FUNID=6,
		TK_RECORDID=7,
		TK_WITH=8,
		TK_PARAMETERS=9,
		TK_END=10,
		TK_WHILE=11,
		TK_INT=12,
		TK_REAL=13,
		TK_TYPE=14,
		TK_MAIN=15,
		TK_GLOBAL=16,
		TK_PARAMETER=17,
		TK_LIST=18,
		TK_SQL=19,
		TK_SQR=20,
		TK_INPUT=21,
		TK_OUTPUT=22,
		TK_SEM=23,
		TK_COLON=24,
		TK_DOT=25,
		TK_ENDWHILE=26,
		TK_OP=27,
		TK_CL=28,
		TK_IF=29,
		TK_THEN=30,
		TK_ENDIF=31,
		TK_READ=32,
		TK_WRITE=33,
		TK_RETURN=34,
		TK_PLUS=35,
		TK_MINUS=36,
		TK_MUL=37,
		TK_DIV=38,	
		TK_CALL=39,
		TK_RECORD=40,
		TK_ENDRECORD=41,
		TK_ELSE=42,
		TK_AND=43,
		TK_OR=44,
		TK_NOT=45,
		TK_LT=46,
		TK_LE=47,
		TK_EQ=48,
		TK_GT=49,
		TK_GE=50,
		TK_NE=51,
		TK_COMMA=52,
		TK_DOLLAR=53,
		TK_EPSILON =54
};

#endif
// till lexer.c
