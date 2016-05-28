// Batch Number 42
// Saurabh Anand  2013A7PS139P
// Priyank Gupta  2013A7PS060P


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbolTable.h"

char sem_err[50][100];
int sem_err_ptr = 0;
//stores fer semantic errors;


symbol_table_ptr init_symbol_table(abstractSyntaxTree A)
{

	//printf("%30s\t%30s\t%30s\t%20s\n","Lexeme","Type","scope","offset");
	//first count the number of functions in the abstractSyntaxTree tree and according make an array of
	//that many symbol tables
	//printf("entered symbol table creation block\n");
	

	abstractSyntaxTree temp = A -> child[0];
	int num_of_functions = 0;
	int j;
	while (temp->symbol != TK_EPSILON) // you will have to rewrite condition depending on the ast
	{
		num_of_functions++;
		temp = temp-> child[1];
	} 

	num_of_functions++; //for main which is compuslsory
	//printf("Number of functions : %d\n",num_of_functions);
	symbol_table_ptr st;
	st=(symbol_table_ptr)malloc(sizeof(struct symbol_table));
	// symbol table initial values
	st-> num_of_function = num_of_functions;
	st->array = (funcptr*)malloc(num_of_functions*sizeof(funcptr));
	/*for (i=0;i<st->num_of_function;i++)
	{

	}*/
	st->head = NULL;
	st->rp = NULL;
	st-> num_of_records = 0;


	abstractSyntaxTree funca = A -> child[0];
	int i=0;

	st->array[st->num_of_function-1]=init_main_function_table(st->num_of_function-1,st,A->child[1]->child[0]);//points to <stmts> of mainfunction
	//printf("done\n");
	while (funca -> symbol != TK_EPSILON)
	{
	//	printf("function initialization\n");
		st->array[i]=init_func_symbol_table(i,st,funca->child[0]);//i is the scope of function being sent
		//printf("name of function : %s and if global null : %s\n",st->array[i]->func_name,st->head == NULL?"yes":"no");
		i++;
		funca = funca->child[1];


	}
	//printf("main function initialization\n");
	//printf("%s:",tokenName[A->child[1]->child[0]->symbol]);
	//st->array[i]=init_main_function_table(i,st,A->child[1]->child[0]);//points to <stmts> of mainfunction

	//printf("ended\n");
	//printf("%s\n",st->head->id_name);

	////////////////////////////////////
	//asiign appropriate record name to return parameters
	for (i=0;i<st->num_of_function;i++)
	{
		identifier_list y1 = st->array[i]->return_par;

		while (y1 != NULL)
		{
			int typee = type_function_var1(st,y1,i);
			if (typee != -1)
			{
				y1->id_type = typee;
				if (typee == 2)
				{
					char tmp11[200];
					get_record_name(st,y1,i);
					/*if (strcmp(tmp11," ") != 0)
					{
						y1->record_name = tmp11;
					}
					else
					{
						printf("Line %d : Record variable %s is not declared \n",y1->line_no,y1->id_name);
					}*/
				}
			}
			else
			{
				sprintf(sem_err[sem_err_ptr++],"Line %d : The variable %s is not declared\n",y1->line_no,y1->id_name);

			}
			y1 = y1->next;

		}
	}
	//printf("yeah\n");



	



	//initialize offset before returning st

	st = init_offset(st);
	
	return st;

}

void print_sem_errors()
{
	int j;
	printf("\n\n\n\n");
	for (j=0;j<sem_err_ptr;j++)
	{
		printf("%s",sem_err[j]);
	}
}

funcptr init_func_symbol_table(int i,symbol_table_ptr st,abstractSyntaxTree funca)
{
	// i here is scope

	funcptr funcvar;
	int chk;
	int foffset=0;

	/////////////////////////////////////////////////////////////////////////

	identifier_list k,k1;
	char** type;

	type=(char**)malloc(3*sizeof(char*));
	type[0]=malloc(10*sizeof(char));
	type[1]=malloc(10*sizeof(char));
	type[2]=malloc(10*sizeof(char));
	strcpy(type[0],"int");
	strcpy(type[1],"real");
	strcpy(type[2],"record");
	////////////////////////////////////////////////////////////////////////
	funcvar = (funcptr)malloc(sizeof(struct func));

	funcvar->func_scope = i;
	strcpy(funcvar->func_name,funca->value);
	funcvar->head = NULL;
	funcvar->input_par = NULL;
	funcvar->output_par = NULL;
	//funcvar->return_list = NULL;
	//funcvar->num_of_records = 0;
	//funcvar->rp = NULL;

	
	abstractSyntaxTree funca1 = funca->child[0]->child[0];//funca->child[0] is <input_par>
	while (funca1->symbol != TK_EPSILON)
	{
		if (funcvar -> input_par == NULL)
		{

			//funcvar->input_par = add_to_table(funca1,i);
			k1 = add_to_table(funca1,i,st);// 0 indicate that it is not global
			//chk = chk_in_function1(k1,i,st,0,funcvar);
			//printf("chk comp going to check: k1 ka naam :%s\n",k1->id_name);

			chk = chk_in_function(k1,i,st,0,funcvar);
			//printf("value returned from chk is : %d\n",chk);
			if (chk == 0)
			{
				/*k1-> offset = foffset;
				foffset += incrementoffset(k1);*/

				funcvar->input_par = k1;
				k=funcvar -> input_par;

				//printf("%30s\t%30s\t%30s\t%20d\n",k->id_name,type[k->id_type],funca->value,k->offset);
			}
			else
			{
				//sprintf(sem_err[sem_err_ptr++],"Line %d : The identifier <%s> is declared more than once.\n",k1->line_no,k1->id_name);
				//now
			}
		}
		else
		{
			identifier_list t1=funcvar->input_par;
			while (t1->next !=NULL)
			{
				t1 = t1 -> next;
			}
			//t1-> next= add_to_table(funca1,i);
			k1 = add_to_table(funca1,i,st);
			chk = chk_in_function(k1,i,st,0,funcvar);
			if (chk == 0)
			{
				/*k1-> offset = foffset;
				foffset += incrementoffset(k1);*/

				t1-> next = k1;
				k=t1->next;

				//printf("%30s\t%30s\t%30s\t%20d\n",k->id_name,type[k->id_type],funca->value,k->offset);
			}
			else
			{
				//sprintf(sem_err[sem_err_ptr++],"Line %d : The identifier <%s> is declared more than once.\n",k1->line_no,k1->id_name);
			//now
			}
		}
		funca1 = funca1 ->child[1];
	}

	//<input_par> case handled above
	//output_var handled below
	int output_flag = 0;
	if (funca->child[1]->symbol != TK_EPSILON)
	{
	funca1 = funca->child[1]->child[0];
	output_flag = 1; //this flag checks if output_par is null or not
	}

	while (funca1->symbol != TK_EPSILON && (output_flag == 1))
	{
		if (funcvar -> output_par == NULL)
		{
			//funcvar->output_par = add_to_table(funca1,i); // i is scope
			k1 = add_to_table(funca1,i,st);
			chk = chk_in_function(k1,i,st,0,funcvar);
			if (chk == 0)
			{
				/*k1-> offset = foffset;
				foffset += incrementoffset(k1);*/

				funcvar->output_par = k1;

				k=funcvar -> output_par;
				//printf("%30s\t%30s\t%30s\t%20d\n",k->id_name,type[k->id_type],funca->value,k->offset);
			}
			else
			{
				//sprintf(sem_err[sem_err_ptr++],"Line %d : The identifier <%s> is declared more than once.\n",k1->line_no,k1->id_name);
			//now
			}
		}
		else
		{
			identifier_list t1=funcvar->output_par;
			while (t1-> next !=NULL)
			{
				t1 = t1 -> next;
			}
			//t1->next = add_to_table(funca1,i);
			k1 = add_to_table(funca1,i,st);
			chk = chk_in_function(k1,i,st,0,funcvar);
			if (chk == 0)
			{
				/*k1-> offset = foffset;
				foffset += incrementoffset(k1);*/

				t1 -> next = k1;

				k=t1->next;

				//printf("%30s\t%30s\t%30s\t%20d\n",k->id_name,type[k->id_type],funca->value,k->offset);

			}
			else
			{
				//sprintf(sem_err[sem_err_ptr++],"Line %d : The identifier <%s> is declared more than once.\n",k1->line_no,k1->id_name);
			//now
			}
			
		}
		funca1 = funca1->child[1];
	}

	//returnn list case handling started

	funca1 = funca->child[2]->child[3];//funca->child[2] is <stmts>
	while (funca1->symbol != TK_EPSILON)
	{
		if (funcvar -> return_par == NULL)
		{

			//funcvar->input_par = add_to_table(funca1,i);
			k1 = add_to_table(funca1,i,st);// 0 indicate that it is not global
			//chk = chk_in_function1(k1,i,st,0,funcvar);
			//printf("chk comp going to check: k1 ka naam :%s\n",k1->id_name);
			chk = 0;
			//chk = chk_in_function(k1,i,st,0,funcvar);
			//printf("value returned from chk is : %d\n",chk);
			if (chk == 0)
			{
				/*k1-> offset = foffset;
				foffset += incrementoffset(k1);*/

				funcvar->return_par = k1;
				k=funcvar -> return_par;

				//printf("%30s\t%30s\t%30s\t%20d\n",k->id_name,type[k->id_type],funca->value,k->offset);
			}
			/*else
			{
				sprintf(sem_err[sem_err_ptr++],"Line %d : The identifier <%s> is declared more than once.\n",k1->line_no,k1->id_name);

			}*/
		}
		else
		{
			identifier_list t1=funcvar->return_par;
			while (t1->next !=NULL)
			{
				t1 = t1 -> next;
			}
			//t1-> next= add_to_table(funca1,i);
			k1 = add_to_table(funca1,i,st);
			//chk = chk_in_function(k1,i,st,0,funcvar);
			chk = 0;
			if (chk == 0)
			{
				/*k1-> offset = foffset;
				foffset += incrementoffset(k1);*/

				t1-> next = k1;
				k=t1->next;

				//printf("%30s\t%30s\t%30s\t%20d\n",k->id_name,type[k->id_type],funca->value,k->offset);
			}
			/*else
			{
				sprintf(sem_err[sem_err_ptr++],"Line %d : The identifier <%s> is declared more than once.\n",k1->line_no,k1->id_name);
			}*/
		}
		funca1 = funca1 ->child[0];
	}


	

	//return list case handling ends

	//write for <declarations

	funca1 = funca->child[2]->child[1];

	//funca1 is <declarations>

	int global_flag = 0;//checks if variable global or not
	while (funca1-> symbol != TK_EPSILON)
	{
		global_flag = 0;
		if (funca1 -> child[0] -> child[1]->symbol == TK_GLOBAL)
		{
			global_flag = 1;
		}

		if (global_flag == 0)
		{
			if (funcvar -> head == NULL)
			{
				//funcvar->head = add_to_table(funca1->child[0],i); // i is scope
				k1 = add_to_table(funca1->child[0],i,st);
				chk = chk_in_function(k1,i,st,0,funcvar);
				if (chk == 0)
				{
					/*k1-> offset = foffset;
				foffset += incrementoffset(k1);*/

					funcvar -> head = k1;

					k=funcvar -> head;

					//printf("%30s\t%30s\t%30s\t%20d\n",k->id_name,type[k->id_type],funca->value,k->offset);

				}
				else
				{
					//sprintf(sem_err[sem_err_ptr++],"Line %d : The identifier <%s> is declared more than once.\n",k1->line_no,k1->id_name);
					//now
				}
				
			}
			else
			{
				identifier_list t1=funcvar->head;
				while (t1-> next!=NULL)
				{
					t1 = t1 -> next;
				}
				//t1->next = add_to_table(funca1->child[0],i);

				k1 = add_to_table(funca1->child[0],i,st);
				chk = chk_in_function (k1,i,st,0,funcvar);
				if (chk == 0)
				{
					/*k1-> offset = foffset;
				foffset += incrementoffset(k1);*/


					t1 -> next = k1;

					k=t1 -> next;

					//printf("%30s\t%30s\t%30s\t%20d\n",k->id_name,type[k->id_type],funca->value,k->offset);

				}
				else
				{
					//sprintf(sem_err[sem_err_ptr++],"Line %d : The identifier <%s> is declared more than once.\n",k1->line_no,k1->id_name);
					//now
				}
				
			}
		

		}
		else
		{
			//what if variable is global
			if (st -> head == NULL)
			{
				//st->head = add_to_table(funca1->child[0],i); // i is scope
				k1 = add_to_table(funca1->child[0],i,st);
				chk = chk_in_function(k1,i,st,1,funcvar);//scope -1 implies ki it is global variable
				if (chk == 0)
				{

					st-> head = k1;

					k=st -> head;

				//printf("%30s\t%30s\t%30s\t%20s\n",k->id_name,type[k->id_type],"global","-");

				}
				else
				{
					
					//sprintf(sem_err[sem_err_ptr++],"Line %d : Variable <%s> being a global variable, cannot be declared more than once.\n",k1->line_no,k1->id_name);
				//now
				}

				
			}
			else
			{
				identifier_list t1=st->head;
				while (t1->next !=NULL)
				{
					t1 = t1 -> next;
				}
				//t1->next = add_to_table(funca1->child[0],i);
				k1 = add_to_table(funca1->child[0],i,st);
				chk = chk_in_function(k1,i,st,1,funcvar);
				if (chk == 0)
				{
					t1 -> next = k1;
					k=t1 -> next;

					//printf("%30s\t%30s\t%30s\t%20s\n",k->id_name,type[k->id_type],"global","-");

				}
				else
				{
					//sprintf(sem_err[sem_err_ptr++],"Line %d : Variable <%s> being a global variable, cannot be declared more than once.\n",k1->line_no,k1->id_name);
				//now

				}
				
			}


		}

		funca1 = funca1 -> child[1];
	}

	//check for declaration module ended

	//return stmt field of symbol table filling started


	//return stmt field of symbol table filling ended

	//check for <type definition> module starts
	funca1 = funca->child[2]->child[0];//<typeDefinitions>
	abstractSyntaxTree t3;
	while (funca1->symbol != TK_EPSILON)
	{
		recptr tmprec = (recptr)malloc(sizeof(struct record_declaration));
		tmprec->head = NULL;
		tmprec->off=0;
		strcpy(tmprec->record_name,funca1->child[0]->value);


		t3 =funca1->child[0]->child[0];
		if (tmprec -> head == NULL)
		{
			tmprec-> head = add_to_table(t3->child[0],i,st);//also sending recordid
			if (tmprec -> head->id_type == 0)
			{
				tmprec->off = (tmprec->off*10)+(2);
			}
			else if (tmprec -> head->id_type == 1)
			{
				tmprec->off = (tmprec->off*10)+4;
			}
			else
			{
				printf("unexpected error in type definition block\n");
			}
		}
		else
		{
			identifier_list temp = tmprec->head;
			while (temp -> next != NULL)
			{
				temp = temp -> next;
			}
			temp -> next = add_to_table(t3->child[0],i,st);//also sending recordid

			if (temp->next->id_type == 0)
			{
				tmprec->off = (tmprec->off*10)+(2);
			}
			else if (temp->next->id_type == 1)
			{
				tmprec->off = (tmprec->off*10)+4;
			}
			else
			{
				printf("unexpected error in type definition block\n");
			}
		}
		//recptr t6;

		////////////////////////////////////////////////////////////////////////////////////////////
		//now second type definition delat with
		if (tmprec -> head == NULL)
		{
			tmprec-> head = add_to_table(t3->child[1],i,st);//also sending recordid

			if (tmprec -> head->id_type == 0)
			{
				tmprec->off = (tmprec->off*10)+(2);
			}
			else if (tmprec -> head->id_type == 1)
			{
				tmprec->off = (tmprec->off*10)+4;
			}
			else
			{
				printf("unexpected error in type definition block\n");
			}
		}
		else
		{
			identifier_list temp = tmprec->head;
			while (temp -> next != NULL)
			{
				temp = temp -> next;
			}
			temp -> next = add_to_table(t3->child[1],i,st);//also sending recordid

			if (temp->next->id_type == 0)
			{
				tmprec->off = (tmprec->off*10)+(2);
			}
			else if (temp->next->id_type == 1)
			{
				tmprec->off = (tmprec->off*10)+4;
			}
			else
			{
				printf("unexpected error in type definition block\n");
			}
		}
		t3 = t3->child[2];
		while (t3->symbol != TK_EPSILON)
		{
			if (tmprec -> head == NULL)
			{
				tmprec-> head = add_to_table(t3->child[0],i,st);//also sending recordid
				if (tmprec -> head->id_type == 0)
				{
				tmprec->off = (tmprec->off*10)+(2);
				}
				else if (tmprec -> head->id_type == 1)
				{
				tmprec->off = (tmprec->off*10)+4;
				}
				else
				{
				printf("unexpected error in type definition block\n");
				}
			}
			else
			{
				identifier_list temp = tmprec->head;
				while (temp -> next != NULL)
				{
					temp = temp -> next;
				}
				temp -> next = add_to_table(t3->child[0],i,st);//also sending recordid
				if (temp -> next->id_type == 0)
				{
				tmprec->off = (tmprec->off*10)+(2);
				}
			else if (temp -> next->id_type == 1)
				{
				tmprec->off = (tmprec->off*10)+4;
				}
			else
				{
				printf("unexpected error in type definition block\n");
				}
			}
			t3 = t3->child[1];
		}
		recptr t6;
		st->num_of_records++;
		if (st-> rp == NULL)
		{
			st->rp = tmprec;
		}
		else
		{
			t6=st->rp;
			while (t6->next != NULL)
			{
				t6=t6->next;
			}
			t6->next = tmprec;
		}

		funca1 = funca1->child[1];
	}
	//check for <type definitions> module ends
	return funcvar;


}


funcptr init_main_function_table(int i,symbol_table_ptr st,abstractSyntaxTree funca)
{
	int foffset = 0;


	/////////////////////////////////////////////////////////////////////////

	identifier_list k,k1;
	int chk;
	char** type;

	type=(char**)malloc(3*sizeof(char*));
	type[0]=malloc(10*sizeof(char));
	type[1]=malloc(10*sizeof(char));
	type[2]=malloc(10*sizeof(char));
	strcpy(type[0],"int");
	strcpy(type[1],"real");
	strcpy(type[2],"record");
	////////////////////////////////////////////////////////////////////////
	// i here is scope
	//funca is <stmts>
	//printf("in main function initialization function\n");

	funcptr funcvar;
	funcvar = (funcptr)malloc(sizeof(struct func));

	funcvar->func_scope = i;
	//printf("the function name is : %20s\n",tokenName[funca->symbol]);
	strcpy(funcvar->func_name,"_main");
	funcvar->head = NULL;
	funcvar->input_par = NULL;
	funcvar->output_par = NULL;
	funcvar->return_par = NULL;
	//funcvar->num_of_records = 0;
	//funcvar->rp = NULL;



	abstractSyntaxTree funca1;
	//printf("chk return\n");
	//////////////////////////////////////
	//check for return par
	funca1 = funca->child[3];//
	while (funca1->symbol != TK_EPSILON)
	{
		if (funcvar -> return_par == NULL)
		{

			//funcvar->input_par = add_to_table(funca1,i);
			k1 = add_to_table(funca1,i,st);// 0 indicate that it is not global
			//chk = chk_in_function1(k1,i,st,0,funcvar);
			//printf("chk comp going to check: k1 ka naam :%s\n",k1->id_name);
			chk = 0;
			//chk = chk_in_function(k1,i,st,0,funcvar);
			//printf("value returned from chk is : %d\n",chk);
			if (chk == 0)
			{
				/*k1-> offset = foffset;
				foffset += incrementoffset(k1);*/

				funcvar->return_par = k1;
				k=funcvar -> return_par;

				//printf("%30s\t%30s\t%30s\t%20d\n",k->id_name,type[k->id_type],funca->value,k->offset);
			}
			/*else
			{
				sprintf(sem_err[sem_err_ptr++],"Line %d : The identifier <%s> is declared more than once.\n",k1->line_no,k1->id_name);

			}*/
		}
		else
		{
			identifier_list t1=funcvar->return_par;
			while (t1->next !=NULL)
			{
				t1 = t1 -> next;
			}
			//t1-> next= add_to_table(funca1,i);
			k1 = add_to_table(funca1,i,st);
			//chk = chk_in_function(k1,i,st,0,funcvar);
			chk = 0;
			if (chk == 0)
			{
				/*k1-> offset = foffset;
				foffset += incrementoffset(k1);*/

				t1-> next = k1;
				k=t1->next;

				//printf("%30s\t%30s\t%30s\t%20d\n",k->id_name,type[k->id_type],funca->value,k->offset);
			}
			/*else
			{
				sprintf(sem_err[sem_err_ptr++],"Line %d : The identifier <%s> is declared more than once.\n",k1->line_no,k1->id_name);
			}*/
		}
		funca1 = funca1 ->child[0];
	}
	/////////////////////////////////////////module ends

	//printf("chk fir tD\n");
	//check for <type definition> module starts
	funca1 = funca->child[0];//<typeDefinitions>
	abstractSyntaxTree t3;
	while (funca1->symbol != TK_EPSILON)
	{
		recptr tmprec = (recptr)malloc(sizeof(struct record_declaration));
		tmprec->head = NULL;
		strcpy(tmprec->record_name,funca1->child[0]->value);


		t3 =funca1->child[0]->child[0];
		if (tmprec -> head == NULL)
		{
			tmprec-> head = add_to_table(t3->child[0],i,st);//also sending recordid

			if (tmprec -> head->id_type == 0)
				{
				tmprec->off = (tmprec->off*10)+(2);
				}
				else if (tmprec -> head->id_type == 1)
				{
				tmprec->off = (tmprec->off*10)+4;
				}
				else
				{
				printf("unexpected error in type definition block\n");
				}

		}
		else
		{
			identifier_list temp = tmprec->head;
			while (temp -> next != NULL)
			{
				temp = temp -> next;
			}
			temp -> next = add_to_table(t3->child[0],i,st);//also sending recordid

			if (temp->next->id_type == 0)
				{
				tmprec->off = (tmprec->off*10)+(2);
				}
				else if (temp->next->id_type == 1)
				{
				tmprec->off = (tmprec->off*10)+4;
				}
				else
				{
				printf("unexpected error in type definition block\n");
				}
		}


		////////////////////////////////////////////////////////////////////////////////////////////
		//now second type definition delat with
		if (tmprec -> head == NULL)
		{
			tmprec-> head = add_to_table(t3->child[1],i,st);//also sending recordid
			if (tmprec -> head->id_type == 0)
				{
				tmprec->off = (tmprec->off*10)+(2);
				}
				else if (tmprec -> head->id_type == 1)
				{
				tmprec->off = (tmprec->off*10)+4;
				}
				else
				{
				printf("unexpected error in type definition block\n");
				}
		}
		else
		{
			identifier_list temp = tmprec->head;
			while (temp -> next != NULL)
			{
				temp = temp -> next;
			}
			temp -> next = add_to_table(t3->child[1],i,st);//also sending recordid

			if (temp->next->id_type == 0)
				{
				tmprec->off = (tmprec->off*10)+(2);
				}
				else if (temp->next->id_type == 1)
				{
				tmprec->off = (tmprec->off*10)+4;
				}
				else
				{
				printf("unexpected error in type definition block\n");
				}
		}
		t3 = t3->child[2];
		while (t3->symbol != TK_EPSILON)
		{
			if (tmprec -> head == NULL)
			{
				tmprec-> head = add_to_table(t3->child[0],i,st);//also sending recordid

				if (tmprec -> head->id_type == 0)
				{
				tmprec->off = (tmprec->off*10)+(2);
				}
				else if (tmprec -> head->id_type == 1)
				{
				tmprec->off = (tmprec->off*10)+4;
				}
				else
				{
				printf("unexpected error in type definition block\n");
				}
			}
			else
			{
				identifier_list temp = tmprec->head;
				while (temp -> next != NULL)
				{
					temp = temp -> next;
				}
				temp -> next = add_to_table(t3->child[0],i,st);//also sending recordid

				if (temp->next->id_type == 0)
				{
				tmprec->off = (tmprec->off*10)+(2);
				}
				else if (temp->next->id_type == 1)
				{
				tmprec->off = (tmprec->off*10)+4;
				}
				else
				{
				printf("unexpected error in type definition block\n");
				}
			}
			t3 = t3->child[1];
		}
		
		st->num_of_records++;
		if (st-> rp == NULL)
		{
			st->rp = tmprec;
		}
		else
		{
			recptr t6=st->rp;
			while (t6->next != NULL)
			{
				t6=t6->next;
			}
			t6->next = tmprec;
		}

		funca1 = funca1->child[1];
	}
	//check for <type definitions> module ends

	//printf("sf\n");
	
	//write for <declarations

	funca1 = funca->child[1];

	//funca1 is <declarations>

	int global_flag = 0;//checks if variable global or not
	while (funca1-> symbol != TK_EPSILON)
	{
		global_flag = 0;
		if (funca1 -> child[0] -> child[1]->symbol == TK_GLOBAL)
		{
			//printf("yo global value : %s\n",funca1->child[0]->value);
			global_flag = 1;
		}

		if (global_flag == 0)
		{
			//printf("yeah\n");
			if (funcvar -> head == NULL)
			{
				//printf("head is null\n");
				//funcvar-> head = add_to_table(funca1->child[0],i); // i is scope
				k1 = add_to_table(funca1->child[0],i,st);
				chk = chk_in_main_function(k1,i,st,0,funcvar);///changes made here
				if (chk == 0)
				{
					/*k1-> offset = foffset;
				foffset += incrementoffset(k1);*/

					funcvar -> head = k1;
					k=funcvar -> head;

				//printf("%30s\t%30s\t%30s\t%20d\n",k->id_name,type[k->id_type],"_main",k->offset);

				}
				else
				{
					printf("Line %d : The identifier <%s> is declared more than once.\n",k1->line_no,k1->id_name);

				}




				
				//printf("%s\n",funcvar->head->id_name);
			}
			else
			{
				identifier_list t1=funcvar->head;
				while (t1-> next !=NULL)
				{
					t1 = t1 -> next;
				}
				//t1->next = add_to_table(funca1->child[0],i);

				k1 = add_to_table(funca1->child[0],i,st);
				chk = chk_in_main_function(k1,i,st,0,funcvar);
				if (chk == 0)
				{
					/*k1-> offset = foffset;
				foffset += incrementoffset(k1);*/

					t1 -> next = k1;
					k=t1->next;

					//printf("%30s\t%30s\t%30s\t%20d\n",k->id_name,type[k->id_type],"_main",k->offset);

				}
				else
				{
					
				
					//sprintf(sem_err[sem_err_ptr++],"Line %d : The identifier <%s> is declared more than once.\n",k1->line_no,k1->id_name);

				
				}

				
			}
		//funca = funca ->child[1];

		}
		else
		{
			//printf("global\n");
			
			//what if variable is global
			if (st -> head == NULL)
			{
				//st->head = add_to_table(funca1->child[0],i); // i is scope
				//printf("global11111\n");
				k1 = add_to_table(funca1->child[0],i,st);
				//printf("%s\n",k1->id_name);

				chk = chk_in_main_function(k1,i,st,1,funcvar);//-1 becox this is global variable
				//printf("chk result is : %d\n",chk);

				if (chk == 0)
				{
					st -> head = k1;
					k=st -> head;

				//printf("%30s\t%30s\t%30s\t%20d\n",k->id_name,type[k->id_type],"global",k->offset);

				}
				else
				{
					//printf("error dude\n");
					//sprintf(sem_err[sem_err_ptr++],"Line %d : Variable <%s> being a global variable, cannot be declared more than once.\n",k1->line_no,k1->id_name);
				
				}

				
			}
			else
			{
				identifier_list t1=st->head;
				while (t1->next !=NULL)
				{
					t1 = t1 -> next;
				}
				//t1->next = add_to_table(funca1->child[0],i);
				k1 = add_to_table(funca1->child[0],i,st);
				chk = chk_in_main_function(k1,i,st,1,funcvar);// -1 bcox global var
				if (chk == 0)
				{
					t1 -> next = k1;
					k=t1 -> next;

				//printf("%30s\t%30s\t%30s\t%20d\n",k->id_name,type[k->id_type],"global",k->offset);

				}
				else
				{
					//sprintf(sem_err[sem_err_ptr++],"Line %d : Variable <%s> being a global variable, cannot be declared more than once.\n",k1->line_no,k1->id_name);
				

				}

				
			}


		}

		funca1 = funca1 -> child[1];
	}

	//check for declaration module ended



	
	return funcvar;


}


identifier_list add_to_table(abstractSyntaxTree a2,int i,symbol_table_ptr st)// i is scope
{
	//find out how to verify records

	identifier_list tmp;
	tmp = (identifier_list)malloc(sizeof(struct identifier));
	tmp-> id_scope = i;
	tmp->id_type = -1;
	strcpy(tmp->record_name," ");
	strcpy(tmp->id_name," ");
	tmp->next = NULL;
	tmp->line_no = -1;
	tmp->offset = -1;
	//tmp->record_attribute =0;

	if (a2-> symbol == TK_ID)
	{
		//tmp->check_record = 0;
		/*strcpy(tmp->record_name,"");*/
		strcpy(tmp->id_name,a2->value);
		//tmp->next = NULL;
		tmp->line_no = a2 -> line_no;

		if (a2->child[0]->symbol == TK_INT)
		{
			tmp->id_type = 0;			
		}
		else if (a2->child[0]->symbol == TK_REAL)
		{
			tmp->id_type = 1;			
		}
		else if (a2->child[0]->symbol == TK_RECORDID)
		{
			tmp->id_type = 2;
			strcpy(tmp->record_name,a2->child[0]->value);
		}
		else if (a2->child[0]->symbol == TK_EPSILON || a2->child[0]->symbol == TK_ID)//for return_par case only mostly
		{
			tmp->id_type = -1;
			//tmp -> id_type = type_function_var1(st,a2,i);
			/*if (tmp -> id_type == -1)
			{
				printf("Line %d : The variable %s is not declared\n",tmp->line_no,tmp->id_name);
			}
			else
			{
				if (tmp->id_type == 2)
				{

					strcpy(tmp->record_name,get_record_name(st,a2,i));

				}
			}*/
		}

	}
	else if (a2-> symbol == TK_FIELDID)
	{

		strcpy(tmp->id_name,a2->value);
		tmp->line_no = a2 -> line_no;
		//tmp->record_attribute = 1;
		if (a2->child[0]->symbol == TK_INT)
		{
			tmp->id_type = 0;
		}
		else if (a2->child[0]->symbol == TK_REAL)
		{
			tmp->id_type = 1;
		}

	}	
	return tmp;
}

void print_symbol_table(symbol_table_ptr st)
{
	//printf("hello\n");
	identifier_list temp = st->head,k;
	char** type;

	type=(char**)malloc(3*sizeof(char*));
	type[0]=malloc(10*sizeof(char));
	type[1]=malloc(10*sizeof(char));
	type[2]=malloc(10*sizeof(char));
	strcpy(type[0],"int");
	strcpy(type[1],"real");
	strcpy(type[2],"record");
	

	printf("%30s\t%30s\t%30s\t%20s\n","Lexeme","Type","scope","offset");
	

	/*printf("record declarations\n");

	recptr t6 = st->rp;
	while (t6!=NULL)
	{
		printf("record name: %s\n",t6->record_name);
		printf(" record ids :%d\n",t6->off);
		k=t6->head;
		while (k!= NULL)
		{
			printf("%30s\t%30s\t%30s\t%20d\n",k->id_name,type[k->id_type],"global",k->offset);
			k=k->next;
		}
		t6 = t6->next;
			
	}

	printf("rec dela ends\n");*/
	



	int j=0;int dummy,new;
	for (j=0;j<(st->num_of_function);j++)
	{
		funcptr t1=st->array[j];
		//printf("currently in function: %s\n",t1->func_name);
		//printf("input parameters of this function are:\n");
		k=t1->input_par;
		while (k!=NULL)
		{
			//printf("1\n");
			//printf(" k ka id type is : %d\n",k->id_type);
			if (k->id_type == 2)
			{
				
				dummy = getRecordType(st,k);
				
				new = 0;
				while (dummy >0)
				{
					new = new*10+dummy%10;
					dummy /= 10;
				}
				char arr[30];
				strcpy(arr,"");
				
				//printf("%30s\t\t\t",k->id_name);
				
				while (new > 0)
				{
					int ty = (new%10)/2-1;
					strcat(arr,type[ty]);
					strcat(arr," ");
					
					//printf("%s ",type[ty]);
					new /= 10;
				}
				
				printf("%30s\t%30s\t%30s\t%20d\n",k->id_name,arr,t1->func_name,k->offset);
			}
			else
				printf("%30s\t%30s\t%30s\t%20d\n",k->id_name,type[k->id_type],t1->func_name,k->offset);
			k=k->next;
		}
		//printf("output parameters of this function are:\n");
		k = t1->output_par;
		while (k!=NULL)
		{
			//printf("2\n");
			if (k->id_type == 2)
			{
				dummy = getRecordType(st,k);
				
				new = 0;
				while (dummy >0)
				{
					new = new*10+dummy%10;
					dummy /= 10;
				}
				char arr[30];
				strcpy(arr,"");
				
				//printf("%30s\t\t\t",k->id_name);
				
				while (new > 0)
				{
					int ty = (new%10)/2-1;
					strcat(arr,type[ty]);
					strcat(arr," ");
					
					//printf("%s ",type[ty]);
					new /= 10;
				}
				
				printf("%30s\t%30s\t%30s\t%20d\n",k->id_name,arr,t1->func_name,k->offset);
			}
			else
				printf("%30s\t%30s\t%30s\t%20d\n",k->id_name,type[k->id_type],t1->func_name,k->offset);
			k=k->next;
		}

		//printf("other declarations of this function are:\n");
		/*printf("printing return\n");
		k = t1->return_par;
		while (k!=NULL)
		{
			printf("%30s\t%30s\t%30s\t%20d %30s\n",k->id_name,type[k->id_type],t1->func_name,k->offset,k->record_name);
			k=k->next;
		}

		printf("close printing of return\n");*/


		k = t1->head;
		while (k!=NULL)
		{
			//printf("11\n");
			
			if (k->id_type == 2)
			{
				dummy = getRecordType(st,k);
				
				new = 0;
				while (dummy >0)
				{
					new = new*10+dummy%10;
					dummy /= 10;
				}
				char arr[30];
				strcpy(arr,"");
				
				//printf("%30s\t\t\t",k->id_name);
				
				while (new > 0)
				{
					int ty = (new%10)/2-1;
					strcat(arr,type[ty]);
					strcat(arr," ");
					
					//printf("%s ",type[ty]);
					new /= 10;
				}
				
				printf("%30s\t%30s\t%30s\t%20d\n",k->id_name,arr,t1->func_name,k->offset);
			}
			else
				printf("%30s\t%30s\t%30s\t%20d\n",k->id_name,type[k->id_type],t1->func_name,k->offset);
			/*if ((k->id_type) == 2)
			{
			printf("the record name is : %s\n",k->record_name);
			}*/
			k=k->next;
		}
		
		

	}
	//printf("yeah\n");

	temp = st -> head;

	while (temp != NULL)
	{
		//printf("there are global\n");
		/*if (strcmp(temp->id_name,"d5b") == 0)
			{
				printf("its record name is :%s\n",temp->record_name);
			}*/

		if (temp->id_type == 2)
		{
			dummy = getRecordType(st,temp);
				
				new = 0;
				while (dummy >0)
				{
					new = new*10+dummy%10;
					dummy /= 10;
				}
				char arr[30];
				strcpy(arr,"");
				
				//printf("%30s\t\t\t",k->id_name);
				
				while (new > 0)
				{
					int ty = (new%10)/2-1;
					strcat(arr,type[ty]);
					strcat(arr," ");
					
					//printf("%s ",type[ty]);
					new /= 10;
				}
				
				printf("%30s\t%30s\t%30s\t%20s\n",temp->id_name,arr,"global","-");
		}
		else
			printf("%30s\t%30s\t%30s\t%20s\n",temp->id_name,type[temp->id_type],"global","-");
		
		temp = temp->next;
	}
	//printf("og\n";)
}

symbol_table_ptr init_offset(symbol_table_ptr st)
{
	//symbol_table_ptr st = st1;
	funcptr funcvar;
	int i;
	for (i=0;i<(st->num_of_function);i++)
	{
		int tmpoff=0;
		identifier_list temp;
		funcvar = st->array[i];
		temp = funcvar -> input_par;

		while (temp != NULL)
		{
			temp -> offset = tmpoff;
			if (temp -> id_type == 0)
			{
				tmpoff = tmpoff + 2;

			}
			else if(temp-> id_type == 1)
			{
				tmpoff = tmpoff + 4;

			}
			else if (temp -> id_type == 2)
			{
				tmpoff = tmpoff + getRecordOffset(st,temp); 
			}
			temp = temp->next;
		}
		temp = funcvar -> output_par;

		while (temp != NULL)
		{
			temp -> offset = tmpoff;
			if (temp -> id_type == 0)
			{
				tmpoff = tmpoff + 2;

			}
			else if(temp-> id_type == 1)
			{
				tmpoff = tmpoff + 4;

			}
			else if (temp -> id_type == 2)
			{
				tmpoff = tmpoff + getRecordOffset(st,temp); 
			}

			temp = temp->next;
		}

		temp = funcvar -> head ;

		while (temp != NULL)
		{
			temp -> offset = tmpoff;
			if (temp -> id_type == 0)
			{
				tmpoff = tmpoff + 2;

			}
			else if(temp-> id_type == 1)
			{
				tmpoff = tmpoff + 4;

			}
			else if (temp -> id_type == 2)
			{
				tmpoff = tmpoff + getRecordOffset(st,temp); 
			}

			temp = temp->next;
		}

	}

	return st;
}



int chk_in_function(identifier_list k,int scope,symbol_table_ptr st,int chkglobal,funcptr funcvar)
{
	//return 0;
	//if scope is -1 it implies k is global so check in all possibility
	//printf("entered chk\n");

	int chk=0;
	int i;
	//funcptr funcvar;
	identifier_list temp;
	if (chkglobal ==0)
	{
		//printf("oget\n");
		//funcvar = st->array[scope];
		//printf("ok\n");

		//chking in global
		//printf("1\n");

		temp = st->head;
		while (temp != NULL)
		{
			if (strcmp(temp->id_name,k->id_name) == 0)
			{
				chk = 1;
				sprintf(sem_err[sem_err_ptr++],"Line %d : Variable <%s> is a global variable and cannot be declared again.\n",k->line_no,k->id_name);
				return chk;
			}
			temp = temp -> next;			
		}
		//printf("2\n");



		//printf("21\n");
		temp = funcvar->input_par;
		//printf("22\n");
		//printf("23\n");
		//printf("value of temp's name is : %s\n",temp->id_name);
		//printf("hello\n");

		while (temp != NULL)
		{
			//printf("24\n");
			if (strcmp(temp->id_name,k->id_name) == 0)
			{
				chk = 1;
				sprintf(sem_err[sem_err_ptr++],"Line %d : Variable <%s> cannot be declared again.\n",k->line_no,k->id_name);
				return chk;
			}
			temp = temp -> next;
			
			
		}

		//printf("3\n");

		temp = funcvar -> output_par;
		while (temp != NULL)
		{
			if (strcmp(temp->id_name,k->id_name) == 0)
			{
				chk = 1;
				sprintf(sem_err[sem_err_ptr++],"Line %d : Variable <%s> cannot be declared again.\n",k->line_no,k->id_name);
				return chk;
			}
			temp = temp -> next;
			
			
		}

		//printf("4\n");

		temp = funcvar -> head;
		while (temp != NULL)
		{
			if (strcmp(temp->id_name,k->id_name) == 0)
			{
				chk = 1;
				sprintf(sem_err[sem_err_ptr++],"Line %d : Variable <%s> cannot be declared again.\n",k->line_no,k->id_name);
				return chk;
			}
			temp = temp -> next;
			
			
		}

		//printf("5\n");

		


	}
	else if (chkglobal == 1)
	{
		// identifier is global so chek in global list as well as all functions
		temp = st->head;
		while (temp != NULL)
		{
			if (strcmp(temp->id_name,k->id_name) == 0)
			{
				chk = 1;
				sprintf(sem_err[sem_err_ptr++],"Line %d : Variable <%s> is global and cannot be declared again.\n",k->line_no,k->id_name);
				return chk;
			}
			temp = temp -> next;
			
			
		}

		for (i=0;i<scope;i++)// while loop only runs till scope
		{
			temp = st->array[i]->head;
			while (temp != NULL)
			{
			if (strcmp(temp->id_name,k->id_name) == 0)
			{
				chk = 1;
				sprintf(sem_err[sem_err_ptr++],"Line %d : Variable <%s> cannot be declared again.\n",k->line_no,k->id_name);
				return chk;
			}
			temp = temp -> next;
			
			
			}

			temp = st->array[i]->input_par;
			while (temp != NULL)
			{
			if (strcmp(temp->id_name,k->id_name) == 0)
			{
				chk = 1;
				sprintf(sem_err[sem_err_ptr++],"Line %d : Variable <%s> cannot be declared again.\n",k->line_no,k->id_name);
				return chk;
			}
			temp = temp -> next;
			
			
			}




			temp = st->array[i]->output_par;
			while (temp != NULL)
			{
			if (strcmp(temp->id_name,k->id_name) == 0)
			{
				chk = 1;
				sprintf(sem_err[sem_err_ptr++],"Line %d : Variable <%s> cannot be declared again.\n",k->line_no,k->id_name);
				return chk;
			}
			temp = temp -> next;
			
			
			}
		}

		chk = 0;
		return chk;

	}
	else
	{
		chk = 0;
		return chk;
	}

	//return 0;
	// -1 implies global variable
}


int chk_in_main_function(identifier_list k,int scope,symbol_table_ptr st,int chkglobal,funcptr funcvar)
{
	//printf("hi\n");
	//return 0;
	//if scope is -1 it implies k is global so check in all possibility
	//printf("entered chk\n");

	int chk=0;
	int i;
	//funcptr funcvar;
	identifier_list temp;
	if (chkglobal ==0 || chkglobal == 1)
	{

		temp = st->head;
		while (temp != NULL)
		{
			if (strcmp(temp->id_name,k->id_name) == 0)
			{
				chk = 1;
				sprintf(sem_err[sem_err_ptr++],"Line %d : Variable <%s> is a global variable and cannot be declared again.\n",temp->line_no,temp->id_name);

				return chk;
			}
			temp = temp -> next;			
		}
		

		temp = funcvar -> head;
		while (temp != NULL)
		{
			if (strcmp(temp->id_name,k->id_name) == 0)
			{
				chk = 1;
				sprintf(sem_err[sem_err_ptr++],"Line %d : Variable <%s> cannot be declared again.\n",temp->line_no,temp->id_name);
				return chk;
			}
			temp = temp -> next;
			
			
		}

		//printf("5\n");

		


	}
	else if (chkglobal == 1)
	{
		
	}
	/*else if (chkglobal == 1)
	{
		// identifier is global so chek in global list as well as all functions
		temp = st->head;
		while (temp != NULL)
		{
			if (strcmp(temp->id_name,k->id_name) == 0)
			{
				chk = 1;
				return chk;
			}
			temp = temp -> next;
			
			
		}

		for (i=0;i<scope;i++)// while loop only runs till scope
		{
			temp = st->array[i]->head;
			while (temp != NULL)
			{
			if (strcmp(temp->id_name,k->id_name) == 0)
			{
				chk = 1;
				return chk;
			}
			temp = temp -> next;
			
			
			}

			temp = st->array[i]->input_par;
			while (temp != NULL)
			{
			if (strcmp(temp->id_name,k->id_name) == 0)
			{
				chk = 1;
				return chk;
			}
			temp = temp -> next;
			
			
			}




			temp = st->array[i]->output_par;
			while (temp != NULL)
			{
			if (strcmp(temp->id_name,k->id_name) == 0)
			{
				chk = 1;
				return chk;
			}
			temp = temp -> next;
			
			
			}
		}

		chk = 0;
		return chk;

	}*/
	else
	{
		chk = 0;
		return chk;
	}

	return 0;
	// -1 implies global variable
}




int chk_in_function1(identifier_list k,int scope,symbol_table_ptr st,int chkglobal,funcptr funcvar)
{
	//return 0;
	//if scope is -1 it implies k is global so check in all possibility
	//printf("entered chk\n");

	int chk=0;
	int i;
	//funcptr funcvar;
	identifier_list temp;
	if (chkglobal ==0 || chkglobal == 1)
	{
		//printf("oget\n");
		//funcvar = st->array[scope];
		//printf("ok\n");

		//chking in global
		printf("1\n");

		temp = st->head;
		while (temp != NULL)
		{
			if (strcmp(temp->id_name,k->id_name) == 0)
			{
				chk = 1;
				return chk;
			}
			temp = temp -> next;			
		}
		printf("2\n");



		printf("21\n");
		temp = st->array[scope]->input_par;
		printf("22\n");
		printf("value of temp's name is : %s\n",temp->id_name);

		while (temp != NULL)
		{
			printf("24\n");
			if (strcmp(temp->id_name,k->id_name) == 0)
			{
				chk = 1;
				return chk;
			}
			temp = temp -> next;
			
			
		}

		printf("3\n");

		temp = funcvar -> output_par;
		while (temp != NULL)
		{
			if (strcmp(temp->id_name,k->id_name) == 0)
			{
				chk = 1;
				return chk;
			}
			temp = temp -> next;
			
			
		}

		printf("4\n");

		temp = funcvar -> head;
		while (temp != NULL)
		{
			if (strcmp(temp->id_name,k->id_name) == 0)
			{
				chk = 1;
				return chk;
			}
			temp = temp -> next;
			
			
		}

		printf("5\n");

		


	}
	else if(chkglobal == 1) // hk in all functions as well
	{
		// identifier is global so chek in global list as well as all functions
		temp = st->head;
		while (temp != NULL)
		{
			if (strcmp(temp->id_name,k->id_name) == 0)
			{
				chk = 1;
				return chk;
			}
			temp = temp -> next;
			
			
		}

		for (i=0;i<scope;i++)// while loop only runs till scope
		{
			temp = st->array[i]->head;
			while (temp != NULL)
			{
			if (strcmp(temp->id_name,k->id_name) == 0)
			{
				chk = 1;
				return chk;
			}
			temp = temp -> next;
			
			
			}

			temp = st->array[i]->input_par;
			while (temp != NULL)
			{
			if (strcmp(temp->id_name,k->id_name) == 0)
			{
				chk = 1;
				return chk;
			}
			temp = temp -> next;
			
			
			}




			temp = st->array[i]->output_par;
			while (temp != NULL)
			{
			if (strcmp(temp->id_name,k->id_name) == 0)
			{
				chk = 1;
				return chk;
			}
			temp = temp -> next;
			
			
			}
		}

		chk = 0;
		return chk;

	}
	else
	{
		chk = 0;
		return chk;
	}

	//return 0;
	// -1 implies global variable
}


int incrementoffset(identifier_list k1)
{
	if (k1-> id_type == 0)
	{
		return 2;
	}
	else if (k1 -> id_type == 1)
	{
		return 4;
	}
	else
	{
		printf("handle record id");
		return 0;
	}
}



int getRecordOffset(symbol_table_ptr st,identifier_list temp)
{
	recptr t6 = st->rp;
	int offset1=0;

	while (t6 != NULL)
	{
		if (strcmp(t6->record_name,temp->record_name) == 0)
		{
			
			int tmpl=t6->off;
			while (tmpl>0)
			{
				offset1+= tmpl%10;
				tmpl/=10;
			}
			return offset1;
		}
		t6 = t6->next;

	}

	printf("No declaration for record <%s>\n",temp->record_name);
	return offset1;
}


int getRecordType(symbol_table_ptr st,identifier_list temp)
{
	recptr t6 = st->rp;
	//int offset1=0;
	//printf("23\n");

	while (t6 != NULL)
	{
		if (strcmp(t6->record_name,temp->record_name) == 0)
		{
			
			
			return t6->off;
		}
		t6 = t6->next;

	}

	printf("No declaration for record <%s>\n",temp->record_name);
	return 0;
}

int type_function_var1(symbol_table_ptr st,identifier_list temp,int scope) 
{
	//printf("compiler\n");
	//checks for type of id in func inputpar, output par , function ids and global ids

	identifier_list tmp = st->head;

	while (tmp != NULL)
	{
		if (strcmp(temp->id_name,tmp->id_name) == 0)
		{
			return tmp->id_type;
		}
		tmp = tmp -> next;
	}
	funcptr funcvar;
	funcvar = st->array[scope];
	tmp = funcvar -> head;
	while (tmp != NULL)
	{
		if (strcmp(temp->id_name,tmp->id_name)==0)
		{
			return tmp->id_type;
		}
		tmp = tmp->next;
	}
	tmp = funcvar -> input_par;
	while (tmp != NULL)
	{
		if (strcmp(temp->id_name,tmp->id_name) == 0)
		{
			return tmp->id_type;
		}
		tmp = tmp -> next;
	}
	tmp = funcvar -> output_par;
	while (tmp != NULL)
	{
		if (strcmp(temp->id_name,tmp->id_name) == 0)
		{
			return tmp->id_type;
		}
		tmp = tmp -> next;
	}

	return -1; //-1 means no variable declaration found in symbol table

}


void get_record_name(symbol_table_ptr st,identifier_list temp,int scope)
{
	int flag = 0;
	identifier_list tmp = st->head;

	while (tmp != NULL && flag == 0)
	{
		if (strcmp(temp->id_name,tmp->id_name) == 0)
		{
			flag = 1;
			strcpy(temp->record_name,tmp->record_name);
			//return tmp->record_name;
		}
		tmp = tmp -> next;
	}
	funcptr funcvar;
	funcvar = st->array[scope];
	tmp = funcvar -> head;
	while (tmp != NULL && flag == 0)
	{
		if (strcmp(temp->id_name,tmp->id_name)==0)
		{
			flag = 1;
			strcpy(temp->record_name,tmp->record_name);
		}
		tmp = tmp->next;
	}
	tmp = funcvar -> input_par;
	while (tmp != NULL && flag == 0)
	{
		if (strcmp(temp->id_name,tmp->id_name) == 0)
		{
			flag = 1;
			strcpy(temp->record_name,tmp->record_name);
		}
		tmp = tmp -> next;
	}
	tmp = funcvar -> output_par;
	while (tmp != NULL && flag == 0)
	{
		if (strcmp(temp->id_name,tmp->id_name) == 0)
		{
			flag = 1;
			strcpy(temp->record_name,tmp->record_name);
		}
		tmp = tmp -> next;
	}

	if (flag == 0)
		sprintf(sem_err[sem_err_ptr++],"Line %d : The variable %s is not declared\n",temp->line_no,temp->id_name);

	//return " "; //-1 means no variable declaration found in symbol table

}
