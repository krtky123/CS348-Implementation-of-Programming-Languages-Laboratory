/* 	Code written by Kartikay Goel, 180101033
	Programming Assignment-3 ,CS348 
	Three files are attached: 180101033_lex.l and lex.yy.c and 180101033_input.txt
	To test the lexical analyser, type the following the cmd:
		flex 180101033_lex.l
		gcc lex.yy.c
		./a.out
	You can change the input file read at line 145 of this file
*/

%{ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Token definitions */
int T_PROGRAM = 1;
int T_VAR = 2;
int T_BEGIN = 3;
int T_END = 4;
int T_END_DOT = 5;
int T_INTEGER = 6;
int T_FOR = 7;
int T_READ = 8;
int T_WRITE = 9;
int T_TO = 10;
int T_DO = 11;
int T_SEMICOLON = 12;
int T_COLON = 13;
int T_COMMA = 14;
int T_ASSIGN = 15;
int T_PLUS = 16;
int T_MINUS = 17;
int T_MULT = 18;
int T_DIV = 19;
int T_OPEN_BRACKET = 20;
int T_CLOSE_BRACKET = 21;
int T_ID = 22;
int T_INT = 23;

typedef union 
{
	int num;
	char *str;
} yylType; /* used to record token type "int" and "id" */

yylType yyval;

struct hashTable
{
	char *val;
	bool type; // id-true |  int-false
	bool present;
};

int lcount=1;
int HASHSIZE=10009;

struct hashTable HASHTABLE[10009];

/* input file name*/
char* fname; 

char* allocate_space(int size){
	return (char *)malloc(size);
}

// djb2 hash function
int hash(char *str1)
{
	char* str = allocate_space(strlen(str1) + 1);
	char* str_ = allocate_space(strlen(str1) + 1);
    unsigned long hash = 0;
    int c;

    while (c = *str++){
        hash += hash * 33;
    }
    int c1;
    while (c1 = *str_++){
        hash += c1;
    }
    hash = hash %(unsigned long) HASHSIZE;
    return (int)hash;
}

void update_hashtable(char *str, int hashVal, bool type){
	HASHTABLE[hashVal].type = type; // set type false for int
	HASHTABLE[hashVal].val = allocate_space(strlen(str)+1); // allocate space for storing the value
	HASHTABLE[hashVal].present = true; // mark present
	strcpy(HASHTABLE[hashVal].val, str); // assign the value
	return;
}


int install_num(char *str)
{
	int hashVal = hash(str); // get the hash value
	if(HASHTABLE[hashVal].present){return -1;} // if already present then collision

	update_hashtable(str,hashVal,false);
	return hashVal;
}

int install_id(char *str)
{
	int hashVal = hash(str); // get the hash value
	if(HASHTABLE[hashVal].present){return -1;} // if already present then collision
	update_hashtable(str,hashVal,true);
	return hashVal;
}


%}

%option noyywrap

%%

PROGRAM 			{return T_PROGRAM;}
VAR 				{return T_VAR;}
BEGIN  				{return T_BEGIN;}
END 				{return T_END;}
"END."          	{return T_END_DOT;}
INTEGER         	{return T_INTEGER;}
FOR 				{return T_FOR;}	
READ 				{return T_READ;}
WRITE 				{return T_WRITE;}
TO  				{return T_TO;}
DO 					{return T_DO;}
";" 				{return T_SEMICOLON;}
":" 				{return T_COLON;}
"," 				{return T_COMMA;}
":="  				{return T_ASSIGN;}
"+" 				{return T_PLUS;}
"-" 				{return T_MINUS;}
"*" 				{return T_MULT;}
DIV 				{return T_DIV;}
"(" 				{return T_OPEN_BRACKET;}
")"					{return T_CLOSE_BRACKET;}

[A-Z]([A-Z0-9])* 	{
						yyval.str = (char *)malloc(strlen(yytext) + 1);
						strcpy(yyval.str, yytext);
						install_id(yytext);
						return T_ID;
					}

[0-9]+				{
						yyval.num = atoi(yytext);
						install_num(yytext);
						return T_INT;
					}

[\t]				{}
" "					{}
"\n"				{lcount++;}
.					{}

%%

int main(){
	fname = "180101033_input.txt"; // set the name of the input file
	yyin = fopen(fname,"r"); // open the input file
	if(yyin)
	{
		
	}
	else
	{
		exit(1);
	}
	printf("Line\t\tToken\t\tToken Specifier\n");
	int s=0;
	int temp;
	while(s=yylex())
	{ // read the input file

		switch(s)
		{
			case 22: 		
				if(lcount-temp == 0) 
				{
					printf("\t\t22\t\t^%s\n",yyval.str);
				}
				else
				{
					printf("%d\t\t22\t\t^%s\n",lcount,yyval.str);
				}
				temp = lcount; 
				break;
			case 23: 		
				if(lcount-temp == 0) 
				{
					printf("\t\t23\t\t#%d\n",yyval.num);
				}
				else
				{
					printf("%d\t\t23\t\t#%d\n",lcount,yyval.num);
				}
				temp = lcount; 
				break;

			default:	
				if(lcount-temp == 0)
				{
					printf("\t\t%d\n",s); 
				}	
				else
				{
					printf("%d\t\t%d\n",lcount,s); 
				}
				temp = lcount; 
				break;
		}
	}
	return 1;
}
