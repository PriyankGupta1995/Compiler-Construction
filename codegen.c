// Batch Number 42
// Saurabh Anand  2013A7PS139P
// Priyank Gupta  2013A7PS060P


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "parser.h"
#include "symbolTable.h"
#include "ircode.h"
#include "codegen.h"

void move_eax(FILE *fp, int i,identifier_list t)
{
  if(t->id_type == 3)
    fprintf(fp, "\tmov eax, [%s]\n",t->id_name);
  else
    fprintf(fp, "\tmov eax, [%s%s]\n",t->record_name ,t->id_name);
}

void move_ebx(FILE *fp, int i,identifier_list t)
{
  if(t->id_type == 3)
    fprintf(fp, "\tmov ebx, [%s]\n",t->id_name);
  else
    fprintf(fp, "\tmov ebx, [%s%s]\n",t->record_name ,t->id_name);
}

void allocate_memory(quadruple_node *quad,int quad_size ,FILE *fp ,symbol_table_ptr st)
{
  int i=0 ;

  while(i<quad_size)
  {

    if(quad[i].arg1 == NULL);
    else if(quad[i].arg1->id_type == 3)
      fprintf(fp, "%10s: dd %s\n",quad[i].arg1->id_name,quad[i].arg1->record_name);

      if(quad[i].arg2 == NULL);
      else if(quad[i].arg2->id_type == 3)
        fprintf(fp, "%10s: dd %s\n",quad[i].arg2->id_name,quad[i].arg2->record_name);

        if(quad[i].result == NULL);
        else if(quad[i].result->id_type == 3)
          fprintf(fp, "%10s: dd %s\n",quad[i].result->id_name,quad[i].result->record_name);

    i++;
  }
  i = 0;
  while(i<temp_no)
  {
    fprintf(fp, "%8s%d: dd 0\n","temp",i);
    i++;
  }
  identifier_list t1 = st->array[0]->head ;
  identifier_list t2 = st->array[0]->input_par ;
  identifier_list t3 = st->array[0]->output_par ;
  identifier_list t4 = st->head;

  while(t1 != NULL)
  {
    fprintf(fp, "%8s%s: dd 0\n",t1->record_name,t1->id_name);
    t1 = t1->next;
  }
  while(t2!= NULL)
  {
    fprintf(fp, "%8s%s: dd 0\n",t2->record_name,t2->id_name);
    t2 = t2->next;
  }
  while(t3 != NULL)
  {
    fprintf(fp, "%8s%s: dd 0\n",t3->record_name,t3->id_name);
    t3 = t3->next;
  }
  while(t4 != NULL)
  {
    fprintf(fp, "%8s%s: dd 0\n",t4->record_name,t4->id_name);
    t4 = t4->next;
  }
  fprintf(fp, "L_read: dd \"%%d\", 0\n");
  fprintf(fp, "L_write: dd \"Value = %%d\",10 , 0\n");
  fprintf(fp, "L_enter: dd \"Enter number: \" ,10, 0\n");
}

void generate_text(quadruple_node *quad,int quad_size ,FILE *fp)
{
  int i=0;
  int flag1= -1,flag2 =-1;
  int label1= -1,label2 =-1;
  int flag3 = -1,label3 = -1;
  int flag4 = -1,label4 = -1;

  while(i<quad_size)
  {

    if(i == flag1 && flag2 != -1)
    {
      fprintf(fp, "\tjmp L%d\n",label_no);
      label2 = label_no;
      label_no ++;
      fprintf(fp, "L%d:",label1);
      flag1 = -1;
    }
    else if(i == flag1 && flag2 == -1)
    {
      fprintf(fp, "L%d:",label1);
      flag1 = -1;
    }
    else if(i == flag2)
    {
      fprintf(fp, "L%d:",label2);
      flag2 = -1;
    }
    else if(i == flag3)
    {
      fprintf(fp, "\tjmp L%d\n",label3);
      fprintf(fp, "L%d:",label4);
      flag3 = -1;
    }


    if(quad[i].opcode == TK_READ)
    {
      fprintf(fp, "\tmov rdi, L_enter\n");
      fprintf(fp, "\tmov rax, 0\n");
      fprintf(fp, "\tcall printf\n\n");

      fprintf(fp, "\tmov rsi, %s%s\n",quad[i].arg1->record_name,quad[i].arg1->id_name);
      fprintf(fp, "\tmov rdi, L_read\n");
      fprintf(fp, "\tmov rax, 0\n");
      fprintf(fp, "\tcall scanf\n\n");
    }

    else if(quad[i].opcode == TK_WRITE)
    {
      fprintf(fp, "\tmov rsi, [%s%s]\n",quad[i].arg1->record_name,quad[i].arg1->id_name);
      fprintf(fp, "\tmov rdi, L_write\n");
      fprintf(fp, "\tmov rax, 0\n");
      fprintf(fp, "\tcall printf\n\n");
    }

    else if(quad[i].opcode == TK_PLUS)
    {
      move_eax(fp,i,quad[i].arg1);
      move_ebx(fp,i,quad[i].arg2);
      fprintf(fp, "\tadd eax, ebx\n");
      fprintf(fp, "\tmov dword [%s%s] ,eax\n\n",quad[i].result->record_name ,quad[i].result->id_name);
    }

    else if(quad[i].opcode == TK_MINUS)
    {
      move_eax(fp,i,quad[i].arg1);
      move_ebx(fp,i,quad[i].arg2);
      fprintf(fp, "\tsub eax, ebx\n");
      fprintf(fp, "\tmov dword [%s%s] ,eax\n\n",quad[i].result->record_name ,quad[i].result->id_name);
    }

    else if(quad[i].opcode == TK_MUL)
    {
      move_eax(fp,i,quad[i].arg1);
      move_ebx(fp,i,quad[i].arg2);
      fprintf(fp, "\tmov edx, 0\n");
      fprintf(fp, "\timul ebx\n");
      fprintf(fp, "\tmov dword [%s%s] ,eax\n\n",quad[i].result->record_name ,quad[i].result->id_name);
    }

    else if(quad[i].opcode == TK_DIV)
    {
      move_eax(fp,i,quad[i].arg1);
      move_ebx(fp,i,quad[i].arg2);
      fprintf(fp, "\tmov edx, 0\n");
      fprintf(fp, "\tidiv ebx\n");
      fprintf(fp, "\tmov dword [%s%s] ,eax\n\n",quad[i].result->record_name ,quad[i].result->id_name);
    }


    else if(quad[i].opcode == TK_ASSIGNOP)
    {
      move_eax(fp,i,quad[i].arg1);
      fprintf(fp, "\tmov dword [%s%s] ,eax\n\n",quad[i].result->record_name ,quad[i].result->id_name);
    }

    else if(quad[i].opcode == TK_LE)
    {
      move_eax(fp,i,quad[i].arg1);
      move_ebx(fp,i,quad[i].arg2);
      fprintf(fp, "\tcmp eax, ebx\n");
      fprintf(fp, "\tjle L%d\n",label_no);
      label1 = label_no;
      label_no ++;
      fprintf(fp, "\tmov dword [%s%s] ,0\n",quad[i].result->record_name ,quad[i].result->id_name);
      fprintf(fp, "\tjmp L%d\n",label_no);
      fprintf(fp, "L%d:\tmov dword [%s%s] ,1\n\n",label1 ,quad[i].result->record_name ,quad[i].result->id_name);
      fprintf(fp, "L%d:",label_no);
      label_no ++;
    }

    else if(quad[i].opcode == TK_LT)
    {
      move_eax(fp,i,quad[i].arg1);
      move_ebx(fp,i,quad[i].arg2);
      fprintf(fp, "\tcmp eax, ebx\n");
      fprintf(fp, "\tjl L%d\n",label_no);
      label1 = label_no;
      label_no ++;
      fprintf(fp, "\tmov dword [%s%s] ,0\n",quad[i].result->record_name ,quad[i].result->id_name);
      fprintf(fp, "\tjmp L%d\n",label_no);
      fprintf(fp, "L%d:\tmov dword [%s%s] ,1\n\n",label1 ,quad[i].result->record_name ,quad[i].result->id_name);
      fprintf(fp, "L%d:",label_no);
      label_no ++;
    }

    else if(quad[i].opcode == TK_GE)
    {
      move_eax(fp,i,quad[i].arg1);
      move_ebx(fp,i,quad[i].arg2);
      fprintf(fp, "\tcmp eax, ebx\n");
      fprintf(fp, "\tjge L%d\n",label_no);
      label1 = label_no;
      label_no ++;
      fprintf(fp, "\tmov dword [%s%s] ,0\n",quad[i].result->record_name ,quad[i].result->id_name);
      fprintf(fp, "\tjmp L%d\n",label_no);
      fprintf(fp, "L%d:\tmov dword [%s%s] ,1\n\n",label1 ,quad[i].result->record_name ,quad[i].result->id_name);
      fprintf(fp, "L%d:",label_no);
      label_no ++;
    }

    else if(quad[i].opcode == TK_GT)
    {
      move_eax(fp,i,quad[i].arg1);
      move_ebx(fp,i,quad[i].arg2);
      fprintf(fp, "\tcmp eax, ebx\n");
      fprintf(fp, "\tjg L%d\n",label_no);
      label1 = label_no;
      label_no ++;
      fprintf(fp, "\tmov dword [%s%s] ,0\n",quad[i].result->record_name ,quad[i].result->id_name);
      fprintf(fp, "\tjmp L%d\n",label_no);
      fprintf(fp, "L%d:\tmov dword [%s%s] ,1\n\n",label1 ,quad[i].result->record_name ,quad[i].result->id_name);
      fprintf(fp, "L%d:",label_no);
      label_no ++;
    }

    else if(quad[i].opcode == TK_EQ)
    {
      move_eax(fp,i,quad[i].arg1);
      move_ebx(fp,i,quad[i].arg2);
      fprintf(fp, "\tcmp eax, ebx\n");
      fprintf(fp, "\tje L%d\n",label_no);
      label1 = label_no;
      label_no ++;
      fprintf(fp, "\tmov dword [%s%s] ,0\n",quad[i].result->record_name ,quad[i].result->id_name);
      fprintf(fp, "\tjmp L%d\n",label_no);
      fprintf(fp, "L%d:\tmov dword [%s%s] ,1\n\n",label1 ,quad[i].result->record_name ,quad[i].result->id_name);
      fprintf(fp, "L%d:",label_no);
      label_no ++;
    }

    else if(quad[i].opcode == TK_NE)
    {
      move_eax(fp,i,quad[i].arg1);
      move_ebx(fp,i,quad[i].arg2);
      fprintf(fp, "\tcmp eax, ebx\n");
      fprintf(fp, "\tjne L%d\n",label_no);
      label1 = label_no;
      label_no ++;
      fprintf(fp, "\tmov dword [%s%s] ,0\n",quad[i].result->record_name ,quad[i].result->id_name);
      fprintf(fp, "\tjmp L%d\n",label_no);
      fprintf(fp, "L%d:\tmov dword [%s%s] ,1\n\n",label1 ,quad[i].result->record_name ,quad[i].result->id_name);
      fprintf(fp, "L%d:",label_no);
      label_no ++;
    }

    else if(quad[i].opcode == TK_IF)
    {
      move_eax(fp,i,quad[i].arg1);
      fprintf(fp, "\tcmp eax, 0\n");
      fprintf(fp, "\tje L%d\n",label_no);
      if(quad[i].arg2 == NULL)
      {
        flag1 = atoi(quad[i].result->id_name);
        //printf("flag1 : %d\n",flag1);
        label1 = label_no;
      }
      else
      {
        flag1 = atoi(quad[i].arg2->id_name);
        printf("flag1 : %d\n",flag1);
        label1 = label_no;
        flag2 = atoi(quad[i].result->id_name);
        //printf("flag2 : %d\n",flag2);
      }
      label_no++;

    }
    else if(quad[i].opcode == TK_WHILE)
    {
      move_eax(fp,i,quad[i].arg1);
      fprintf(fp, "\tcmp eax, 0\n");
      fprintf(fp, "\tje L%d\n",label_no);
      label4 = label_no;
      flag3 = atoi(quad[i].result->id_name);
      //printf("flag3 : %d\n",flag3);
      label_no++;
    }
    else if(quad[i].opcode == TK_INPUT)
    {
      fprintf(fp, "L%d:",label_no);
      label3 = label_no;
      label_no++;
    }


    i++;
  }

}

void print_nasm(quadruple_node *quad,int quad_size, char *outfile ,symbol_table_ptr st)
{
  //printf("%d\n",temp_no);
  FILE *fp = fopen(outfile,"w+");
  label_no = 0;
	//printf("entered print block: \n");
  //fprintf(fp ,"%21s %7s %20s %20s %30s %8s %20s \n\n" ,"LEXEMECURRENTNODE" ,"

  fprintf(fp ,"\n\tsection .data\n\n");
  allocate_memory(quad,quad_size,fp , st);
  fprintf(fp ,"\n\tsection .text\n\n");
  fprintf(fp ,"\textern printf\n");
  fprintf(fp ,"\textern scanf\n");
  fprintf(fp ,"\tglobal main\n");
  fprintf(fp ,"main:\n");
  generate_text(quad,quad_size,fp);
  fprintf(fp ,"\tmov rax,0\n");
  fprintf(fp ,"\tret\n");

  printf("\n\nAssembly code has been successfully generated in the file %s\n\n",outfile);
  fclose(fp);
}
