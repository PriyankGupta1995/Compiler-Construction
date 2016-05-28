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

int return_quad_size()
{
  return (quad_size);
}


void print_tree(abstractSyntaxTree temp)
{
  int i=0;
  if(temp == NULL);

  //printf("Node is : %s\n",tokenName[temp->symbol]);

  while(i<temp->childnum)
  {
    print_tree(temp->child[i] );
    i++;
  }
}

identifier_list create_identifier(int type,char record_value[200], char id_value[200], int scope)
{
  identifier_list t = (identifier_list) malloc (sizeof(struct identifier)) ;
  t->id_type = type;
  t->id_scope = scope;
  t->line_no = -1;
  strcpy(t->record_name, record_value);
  strcpy(t->id_name, id_value);
  t->next = NULL;

  return t;
}

identifier_list create_copy(identifier_list a,identifier_list b)
{
  //printf("Hey\n");
  if(b == NULL)
    a = NULL;
  else
  {
    a = (identifier_list)  malloc (sizeof(struct identifier));
    //printf("Hey2\n");
    a->id_scope = b->id_scope;
    //printf("Hey3\n");

    a->id_type = b->id_type;
    a->line_no = b->line_no;
    strcpy(a->record_name ,b->record_name);
    strcpy(a->id_name ,b->id_name);
    a->next = NULL;
  }

  return a;
}

void create_quadruple(int opcode, identifier_list a,identifier_list b,identifier_list out)
{
  int i = quad_size;
  //printf("Hello\n");
  quad[i].opcode = opcode;
  //printf("Opcode is : %s\n", tokenName[quad[i].opcode]);
  quad[i].arg1 = create_copy(quad[i].arg1 , a);
  //printf("Hi\n");
  quad[i].arg2 = create_copy(quad[i].arg2 , b);
  quad[i].result = create_copy(quad[i].result , out);
  quad_size ++;
}

void print_quadruple()
{
  int i=0;
  //printf(" Operation Argument1 Argument2 Result\n");
  printf(" %20s %20s %20s %20s\n","Operation","Argument1", "Argument2","Result");

  while(i<quad_size)
  {

    printf(" %10s %10s %10s %10s  \n",tokenName[quad[i].opcode],quad[i].arg1 == NULL ? "NULL":quad[i].arg1->id_name,
    quad[i].arg2 == NULL ? "NULL":quad[i].arg2->id_name,quad[i].result == NULL? "NULL":quad[i].result->id_name);
    /////////////
    i++;
  }
}

identifier_list get_symtable_ptr(char record_value[200], char id_value[200],symbol_table_ptr st, int scope)
{
  identifier_list t1 = st->array[scope]->head ;
  identifier_list t2 = st->array[scope]->input_par ;
  identifier_list t3 = st->array[scope]->output_par ;
  identifier_list t4 = st->head;
  //printf("Type is : %d\n",t->id_type);
  //printf("Current record name : %s\n",t->record_name);
  //printf("Current id name : %s\n",t->id_name);
  //printf("Hello\n");

  while(t1 != NULL)
  {
    //printf("Current record name : %s\n",t1->record_name);
    //printf("Current id name : %s\n",t1->id_name);
    if (strcmp(t1->id_name,id_value) == 0 && strcmp(t1->record_name,record_value) == 0)
      return t1;
    t1 = t1->next;
  }

  while(t2 != NULL)
  {
    //printf("Current record name : %s\n",t2->record_name);
    //printf("Current id name : %s\n",t2->id_name);
    if (strcmp(t2->id_name,id_value) == 0 && strcmp(t2->record_name,record_value) == 0)
      return t2;
    t2 = t2->next;
  }

  while(t3 != NULL)
  {
    //printf("Current record name : %s\n",t3->record_name);
    //printf("Current id name : %s\n",t3->id_name);
    if (strcmp(t3->id_name,id_value) == 0 && strcmp(t3->record_name,record_value) == 0)
      return t3;
    t3 = t3->next;
  }

  while(t4 != NULL)
  {
    //printf("Current record name : %s\n",t2->record_name);
    //printf("Current id name : %s\n",t2->id_name);
    if (strcmp(t4->id_name,id_value) == 0 && strcmp(t4->record_name,record_value) == 0)
      return t4;
    t4 = t4->next;
  }
}

identifier_list eval_arithmetic(abstractSyntaxTree temp, symbol_table_ptr st,int scope)
{
  char temp_name[10];
  char name[10];
  identifier_list id1;
  identifier_list id2;
  identifier_list id3;

  if (temp->symbol == TK_EPSILON && temp->childnum == 0)
	{
		return NULL;
	}
	else if (temp->symbol == TK_EPSILON && temp->childnum>0)
	{
		return (eval_arithmetic(temp->child[0],st,scope));
	}

  if(temp->symbol == TK_NUM || temp->symbol == TK_RNUM )
  {
    //printf("Current expr is: %s\n",temp->value);
    sprintf(name , "%d", num_no);
    strcpy(temp_name,"num");
    strcat(temp_name,name);
    id3 = create_identifier(3,temp->value,temp_name,scope);
    num_no ++;
    return (id3);

  }

  else if(temp->symbol == TK_ID)
  {
    if(temp->child[0]->symbol == TK_EPSILON)
    {
      //printf("Current expr is: %s\n",tokenName[temp->symbol]);
      return (get_symtable_ptr(" ",temp->value, st, scope));
    }
    else
    {
      //printf("Current expr is: %s\n",tokenName[temp->symbol]);
      return (get_symtable_ptr(temp->value,temp->child[0]->value, st, scope));
    }
  }
  else if(strcmp(tokenName[temp->symbol],"<term>") == 0 || strcmp(tokenName[temp->symbol],"<arithmeticExpression>") == 0
          || temp->symbol == TK_PLUS || temp->symbol == TK_MINUS || temp->symbol == TK_MUL || temp->symbol == TK_DIV)
  {
    id1 = eval_arithmetic(temp->child[0],st,scope);
    id2 = eval_arithmetic(temp->child[1],st,scope);

    if(id1 == NULL && id2 != NULL)
      return (id2);
    else if(id1 !=NULL && id2 == NULL)
      return (id1);
    else
    {
      sprintf(name , "%d", temp_no);
      strcpy(temp_name,"temp");
      strcat(temp_name,name);
      id3 = create_identifier(id1->id_type," ",temp_name,scope);
      create_quadruple(temp->child[1]->symbol,id1,id2,id3);
      temp_no++;
      return (id3);
    }

  }
  else;

}

identifier_list eval_boolean(abstractSyntaxTree temp, symbol_table_ptr st,int scope)
{
  char temp_name[10];
  char name[10];
  identifier_list id1;
  identifier_list id2;
  identifier_list id3;

  if(temp->symbol == TK_NUM || temp->symbol == TK_RNUM )
  {
    //printf("Current expr is: %s\n",temp->value);
    sprintf(name , "%d", num_no);
    strcpy(temp_name,"num");
    strcat(temp_name,name);
    id3 = create_identifier(3,temp->value,temp_name,scope);
    num_no ++;
    return (id3);

  }

  else if(temp->symbol == TK_ID)
  {
    //printf("HI\n");
    return (get_symtable_ptr(" ",temp->value, st, scope));
  }
  else if(temp->symbol == TK_NOT)
  {
    id1 = eval_boolean(temp->child[0],st,scope);
    sprintf(name , "%d", temp_no);
    strcpy(temp_name,"temp");
    strcat(temp_name,name);
    id3 = create_identifier(id1->id_type," ",temp_name,scope);
    create_quadruple(temp->symbol,id1,NULL,id3);
    temp_no++;
    return (id3);
  }
  else
  {
    id1 = eval_boolean(temp->child[0],st,scope);
    //printf("Why\n");
    id2 = eval_boolean(temp->child[1],st,scope);
    sprintf(name , "%d", temp_no);
    strcpy(temp_name,"temp");
    strcat(temp_name,name);
    id3 = create_identifier(id1->id_type," ",temp_name,scope);
    create_quadruple(temp->symbol,id1,id2,id3);
    temp_no++;
    return(id3);
  }
}

void extract_stmt(abstractSyntaxTree temp,symbol_table_ptr st, int scope )
{
  identifier_list id;

  char name[10];
  char temp_name[10];

    //printf("Stmt is : %s\n" ,tokenName[temp->symbol]);

    if(temp->symbol == TK_ASSIGNOP)
    {
      //print_tree(temp);
      //printf("Sup\n");
      //printf("%d\n",temp->childnum);

      if(temp->child[0]->child[0]->symbol == TK_EPSILON)
      {
        //print_tree(temp->child[1]);
        //printf("Child1 : %s\n",tokenName[temp->child[1]->symbol]);
        //printf("Searching for %s\n",temp->child[0]->value);
        id = get_symtable_ptr(" ",temp->child[0]->value, st, scope);
        //printf("Sym  value : %s\n",id->id_name);
        //printf("HI\n");
        create_quadruple(temp->symbol,eval_arithmetic(temp->child[1],st,scope),NULL,id);
      }
      else
      {
        //printf("Searching for %s.%s\n",temp->child[0]->value ,temp->child[0]->child[0]->value);
        id = get_symtable_ptr(temp->child[0]->value,temp->child[0]->child[0]->value, st, scope);
        //printf("Sym table value : %s.%s",id->record_name,id->id_name);

        create_quadruple(temp->symbol,eval_arithmetic(temp->child[1],st,scope),NULL,id);
      }
    }
    //printf("Hi\n");*/

    if(temp->symbol == TK_READ)
    {

      if(temp->child[0]->child[0]->symbol == TK_EPSILON)
      {
        id = get_symtable_ptr(" ",temp->child[0]->value, st, scope);
        //printf("Sym  value : %s\n",id->id_name);
        create_quadruple(temp->symbol,id,NULL,NULL);
      }
      else
      {
        //printf("The record is : %s\n",temp->child[0]->value);
        //printf("The id is : %s\n",temp->child[0]->child[0]->value);
        id = get_symtable_ptr(temp->child[0]->value,temp->child[0]->child[0]->value, st, scope);
        //printf("Sym table value : %s",id->record_name);

        create_quadruple(temp->symbol,id,NULL,NULL);
      }
    }
    else if(temp->symbol == TK_WRITE)
    {

        if(temp->child[0]->symbol == TK_NUM || temp->child[0]->symbol == TK_RNUM )
        {
          //printf("Current expr is: %s\n",temp->value);
          sprintf(name , "%d", num_no);
          strcpy(temp_name,"num");
          strcat(temp_name,name);
          id = create_identifier(3,temp->child[0]->value,temp_name,scope);
          num_no ++;

          create_quadruple(temp->symbol,id,NULL,NULL);
        }


      else if(temp->child[0]->child[0]->symbol == TK_EPSILON)
      {
        id = get_symtable_ptr(" ",temp->child[0]->value, st, scope);
        //printf("Sym  value : %s\n",id->id_name);
        create_quadruple(temp->symbol,id,NULL,NULL);
      }
      else
      {
        //printf("The record is : %s\n",temp->child[0]->value);
        //printf("The id is : %s\n",temp->child[0]->child[0]->value);
        id = get_symtable_ptr(temp->child[0]->value,temp->child[0]->child[0]->value, st, scope);
        //printf("Sym table value : %s",id->record_name);
          create_quadruple(temp->symbol,id,NULL,NULL);
      }
    }

    else if(temp->symbol == TK_IF)
    {
      int jump1,jump2;
      //print_tree(temp->child[0]);
      id = eval_boolean(temp->child[0],st,scope);
      jump1 = quad_size;
      create_quadruple(temp->symbol,id,NULL,NULL);
      extract_stmt(temp->child[1],st,scope);
      extract_individual_stmts(temp->child[2],st,scope);

      if(temp->child[3]->symbol == TK_ENDIF)
      {
        sprintf(name , "%d", quad_size);
        quad[jump1].result = create_copy(quad[jump1].result,create_identifier(0," ",name,scope));
      }
      else if(temp->child[3]->symbol == TK_ELSE)
      {
        jump2 = quad_size;
        extract_stmt(temp->child[3]->child[0],st,scope);
        extract_individual_stmts(temp->child[3]->child[1],st,scope);
        sprintf(name , "%d", quad_size);
        quad[jump1].result = create_copy(quad[jump1].result,create_identifier(0," ",name,scope));
        sprintf(name , "%d", jump2);
        quad[jump1].arg2 = create_copy(quad[jump1].arg2,create_identifier(0," ",name,scope));
      }
    }
    else if(temp->symbol == TK_WHILE)
    {
      int jump;
      create_quadruple(21,NULL,NULL,NULL);
      id = eval_boolean(temp->child[0],st,scope);

      create_quadruple(temp->symbol,id,NULL,NULL);
      jump = quad_size-1;
      extract_stmt(temp->child[1],st,scope);
      extract_individual_stmts(temp->child[2],st,scope);
      sprintf(name , "%d", quad_size);
      quad[jump].result = create_copy(quad[jump].result,create_identifier(0," ",name,scope));
    }
    //printf("Bye");

}

void extract_individual_stmts(abstractSyntaxTree ast, symbol_table_ptr st,int scope)
{
  abstractSyntaxTree temp;
  while(ast->symbol != TK_EPSILON)
  {
    temp = ast->child[0];
    extract_stmt(temp,st,scope);
    ast = ast->child[1];
  }
}

quadruple_node * code_generation(abstractSyntaxTree ast, symbol_table_ptr st)
{
  int i=0;

  abstractSyntaxTree temp=ast;

  quad_size = 0;
  temp_no = 0;
  num_no = 0;

  ast = ast->child[0];

  while(ast->symbol != 54)
  {
      i++;
      ast = ast->child[1];
  }
  /*while(ast->parent != NULL)
    ast = ast->parent;*/
    ast = temp;

  temp = ast->child[1]->child[0]->child[2];//<otherStmts>
  //printf ("In main: %s\n",tokenName[temp->symbol]);
  //printf("Scope is : %d",i);
  extract_individual_stmts(temp ,st ,i);


  //printf("Quad size is : %d\n",quad_size);
  //print_quadruple();
  return (quad);
}
