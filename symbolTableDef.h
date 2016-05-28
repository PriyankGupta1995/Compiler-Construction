// Batch Number 42
// Saurabh Anand  2013A7PS139P
// Priyank Gupta  2013A7PS060P


#ifndef DATA1
#define DATA1

typedef struct func *funcptr;
typedef struct identifier *identifier_list;
typedef struct symbol_table *symbol_table_ptr;
typedef struct record_declaration *recptr;


struct identifier
{
	int id_type;// 0:int , 1:real , 2:recordid
	int id_scope;
	int line_no;
	//int record_attribute;
	char record_name[200];//check maximum size of record name
	char id_name[200];// check the maximum size of the identifier
	//newly addes
	int offset;
	identifier_list next;


};
struct func
{
	int func_scope;
	//int num_of_records;
	//recptr rp;
	char func_name[200];//check what is the maximum size of a function
	identifier_list head;
	identifier_list input_par;
	identifier_list output_par;
	identifier_list return_par;


};
struct symbol_table
{
	int num_of_records;
	recptr rp;
	
	int num_of_function;
	identifier_list head;
	funcptr *array;

};

struct record_declaration
{
	int off;
	char record_name[200];
	identifier_list head;
	recptr next;
};

#endif
