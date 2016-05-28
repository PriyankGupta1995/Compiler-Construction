// Batch Number 42
// Saurabh Anand  2013A7PS139P
// Priyank Gupta  2013A7PS060P

#include <stdio.h>
#include <stdlib.h>
#include "ircodeDef.h"

quadruple_node * code_generation(abstractSyntaxTree ast, symbol_table_ptr st);      // main function that returns quadruple table
extern void extract_stmt(abstractSyntaxTree stmt,symbol_table_ptr st, int scope);   // to extract
extern identifier_list get_symtable_ptr(char record_value[200], char id_value[200],symbol_table_ptr st, int scope); // to get symbol table entries
extern void print_tree(abstractSyntaxTree temp);							// for printing ast tree
extern identifier_list create_identifier(int type,char record_value[200], char id_value[200], int scope); //creates new symbol table 															//entry	( for temporaries)
extern identifier_list create_copy(identifier_list a,identifier_list b);			       // creates copy of symbol table entry
extern void create_quadruple(int opcode, identifier_list a,identifier_list b,identifier_list out);     // to create new quadruple node
extern void print_quadruple();          							       // to print quadruple table
extern identifier_list eval_arithmetic(abstractSyntaxTree temp, symbol_table_ptr st,int scope);	       // to evaluate arithmetic expressions
extern identifier_list eval_boolean(abstractSyntaxTree temp, symbol_table_ptr st,int scope);	       // to evaluate boolean expressions
extern void extract_individual_stmts(abstractSyntaxTree ast, symbol_table_ptr st,int scope);	       // to extract <stmt> nodes from <otherStmts>
extern int return_quad_size();                                                                         // returns no of quadruple nodes
