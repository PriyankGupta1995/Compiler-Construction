// Batch Number 42
// Saurabh Anand  2013A7PS139P
// Priyank Gupta  2013A7PS060P

#include <stdio.h>
#include <stdlib.h>

int label_no;

extern void print_nasm(quadruple_node *quad,int quad_size, char *outfile ,symbol_table_ptr st);
extern void allocate_memory(quadruple_node *quad,int quad_size ,FILE *fp ,symbol_table_ptr st);
extern void move_eax(FILE *fp, int i,identifier_list t);
extern void move_ebx(FILE *fp, int i,identifier_list t);
