/*36 T1
Shikhar Brajesh 2012A7PS114P
Girish Rathi 2012A7PS347P
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "ST_Lexical.h"
#include "hash_table.h"
#include "semantics.h"

int childx[987654];
int parentx[987654];
int rule[70][70];
symbol_table *symTab;

FILE *code;

typedef struct _node
{
	int value;
	char *str;
	int type;
	struct _node *child[10];
	int nc;
	struct _node *par;
	int rules_info;
	int child_no;
	int regnx[3];
	int c;
}node;

typedef struct parse_tree
{
	node *root;
}parse_tree;

node* create_node(char *str,int type)
{
	node *newnode;
	newnode=(node *)malloc(sizeof(node));
	newnode->nc=0;
	newnode->type=type;
	newnode->str=strdup(str);
	return newnode;
}
struct stack_ent
{
  node *data;
  struct stack_ent *next;
};

struct stack_t1
{
  struct stack_ent *head;
  size_t stackSize;    
};

struct stack_t1 *newStack1(void)
{
  struct stack_t1 *stack = malloc(sizeof *stack);
  if (stack)
  {
    stack->head = NULL;
    stack->stackSize = 0;
  }
  return stack;
}

void push1(struct stack_t1 *theStack, node *value)
{
   struct stack_ent *entry = malloc(sizeof *entry); 
    entry->data = value;
    entry->next = theStack->head;
    theStack->head = entry;
    theStack->stackSize++;
}
node *top1(struct stack_t1 *theStack)
{
  if (theStack && theStack->head)
    return theStack->head->data;
  else
    return NULL;
}
void pop1(struct stack_t1 *theStack)
{
  if (theStack->head!= NULL)
  {
    struct stack_ent *tmp = theStack->head;
    theStack->head = theStack->head->next;
    theStack->stackSize--;
  }
}



typedef struct rulesList
{
	char str[10][30];
	int size;
}rules_list;

rules_list RL[105];

typedef struct _elementP {
    char *string;
     int value;
    struct _elementP *next;
   
} elementP;

typedef struct _hash_tableP {
    int size;       //the size of the table 
    elementP **table; // the table elements 
} hash_tableP;


hash_tableP *create_hash_tableP(int size)
{
    hash_tableP *new_table;
    
    if (size<1) return NULL; /* invalid size for table */

    /* Attempt to allocate memory for the table structure */
    if ((new_table = malloc(sizeof(hash_tableP))) == NULL) {
        return NULL;
    }
    
    /* Attempt to allocate memory for the table itself */
    if ((new_table->table = malloc(sizeof(elementP *) * size)) == NULL) {
        return NULL;
    }

    /* Initialize the elements of the table */
    int i=0;
    for(i=0; i<size; i++) new_table->table[i] = NULL;

    /* Set the table's size */
    new_table->size = size;

    return new_table;
}


unsigned int hashP(hash_tableP *hashtable, char *str)
{
    unsigned int hashval;
    hashval = 0;
    for(; *str != '\0'; str++) hashval = *str + (hashval << 5) - hashval;
    return hashval % hashtable->size;
}


int lookup_stringP(hash_tableP *hashtable, char *str)
{
    elementP *list;
    unsigned int hashval = hashP(hashtable, str);
     for(list = hashtable->table[hashval]; list != NULL; list = list->next) {
        if (strcmp(str, list->string) == 0) return list->value;
    }
    return -1;
}
int add_stringP(hash_tableP *hashtable, char *str,int val)
{
    elementP *new_list;
    elementP *current_list;
    unsigned int hashval = hashP(hashtable, str);
    if ((new_list = malloc(sizeof(elementP))) == NULL) return 1;
    
    new_list->string = strdup(str);
    new_list->value=val;
    new_list->next=hashtable->table[hashval];
    hashtable->table[hashval] = new_list;
    return 0;
}

struct stack_entry {
  char *data;
  struct stack_entry *next;
};

struct stack_t
{
  struct stack_entry *head;
  size_t stackSize;  
                     
};

struct stack_t *newStack(void)
{
  struct stack_t *stack = malloc(sizeof *stack);
  if (stack)
  {
    stack->head = NULL;
    stack->stackSize = 0;
  }
  return stack;
}
char *copyString(char *str)
{
  char *tmp = malloc(strlen(str) + 1);
  if (tmp)
    strcpy(tmp, str);
  return tmp;
}
void push(struct stack_t *theStack, char *value)
{
 	 struct stack_entry *entry = malloc(sizeof *entry); 
  
    entry->data = copyString(value);
    entry->next = theStack->head;
    theStack->head = entry;
    theStack->stackSize++;
}

char *top(struct stack_t *theStack)
{
  if (theStack && theStack->head)
    return theStack->head->data;
  else
    return NULL;
}

void pop(struct stack_t *theStack)
{
  if (theStack->head != NULL)
  {
    struct stack_entry *tmp = theStack->head;
    theStack->head = theStack->head->next;
    free(tmp->data);
    free(tmp);
    theStack->stackSize--;
  }
}



int construct_nonterminal()
{
	char *nonterminal[]=
	{
"<program>",
"<structures>",
"<declarations>",
"<functions>",
"<mainfunc>",
"<structure>",
"<P>",
"<type>",
"<idlist>",
"<arrayinit>",
"<T>",
"<funcname>",
"<funcsignature>",
"<Y>",
"<block>",
"<tryblock>",
"<args>",
"<returntype>",
"<T2>",
"<statement>",
"<statements>",
"<assignstat>",
"<returnstat>",
"<iterativestat>",
"<outstat>",
"<arraystmt>",
"<functioncall>",
"<toSend>",
"<literal>",
"<S>",
"<ifstat>",
"<elsestat>",
"<instat>",
"<arraypart>",
"<Z>",
"<X>",
"<Exp>",
"<ORexp>",
"<condExp>",
"<ANDexp>",
"<F>",
"<G>",
"<equalityexp>",
"<H>",
"<equalOp>",
"<relationalexp>",
"<arithmeticexp>",
"<J>",
"<relOp>",
"<addexp>",
"<mulexp>",
"<B>",
"<C>",
"<bitexp>",
"<D>",
"<bitOp>",
"kuchbhinahihaiyahpe",
"<unaryexp>",
"<notexp>",
"<K>",
"<simple>",
"<notOp>",
"<integerliteral>",
"<booleanliteral>",
"<charliteral>",
"yehbhi_kuch_nahi_hai"
	}; 

}

int construct_terminal()
{
	char *terminal[]=
	{
	"$",
"create",
"constant",
"TK_Identifier",
"void",	
"function",	
"main",	
"try",
"catch",
"finally",	
"call",	
"return",	
"end",
"next",	
"if",	
"else",	
"while",	
"input",	
"output",	
"array",	
"#",
".",	
"inc",	
"dec",	
"int",	
"char",	
"boolean",	
"TK_Integer",	
"true",	
"false",
"TK_Character",	
"void",	
"{",	
"}",	
"(",	
")",	
"[",	
"]",	
",",	
":",
";",	
":=",	
"||",	
"&&",	
"==",	
"!=",	
">",	
"<",	
"<=",	
">=",	
"-",	
"+",	
"*",	
"/",	
"%",	
"&",	
"|",	
"!"
	}; 

}

void construct_rule()
{
	
	int i=0, j=0;
	for(i=0; i< 66; i++)
		for(j=0; j<58; j++){
			rule[i][j]=-1;
		}
char *rules[]=
{
"e",
"<structures> <declarations> <functions> <main-func>",
"<structure> <structures>",
"<P> <type> <id-list> ; <declarations>",
"<arrayinit>",
"function <type> <funcname> : <funcsignature> <Y> <functions>",
"main  <block>",
"create TK_Identifier { <declarations> };",
"constant",
"TK_Identifier <T>",
"int",
"char",
"boolean",
"<arraypart> TK_Identifier ;",
", <id-list>",
"( <args> )",
"<try-block>",
"try <block> catch <block> finally <block>",
"<block>",
"{<statements>}",
"<type> TK_Identifier <T2>",
"void T2",
"<type>",
"void",
", <args>",
"<declarations>",
"<assignstat>",
"<functioncall>",
"<returnstat>",
"end;",
"next;",
"<ifstat>",
"<iterativestat>",
"<instat>",
"<outstat>",
"<statement> <statements>",
"<arraystmt> := <Exp>;",
"call id(<toSend>);",
"<arraystmt> <S>",
"<literal> <S>",
", <toSend>",
"return <to return> ;",
"<arraystmt>",
"<literal>",
"if(<condExp>){ <statements> } <elsestat>",
"else { <statements> }",
"while ( <condExp> ){ <statements> }",
"input >> id;",
"output << id;",
"<arraypart> id ;",
"array ( <types> )[<arithmeticexp>] <Z>",
"[ <arithmeticexp> ] <Z>",
"id <X>",
"# id <X>",
"[ <arithmeticexp> ] <X>",
".id",
"<ORexp>",
"<ANDexp> <F>",
"|| <ANDexp> <F>",
"<equalityexp> <G>",
"&&<equalityexp> <G>",
"<relationalexp> <H>",
"<equalOp> <relationalexp>",
"==",
"!=",
"<arithmeticexp> <J>",
"<relOp> <arithmeticexp>",
">",
"<",
"<=",
">=",
"<addexp>",
"<mulexp> <B>",
"+ <mulexp> <B>",
"- <mulexp> <B>",
"<bitexp> <C>",
"* <mulexp> <C>",
"/ <mulexp> <C>",
"% <mulexp> <C>",
"<unaryexp> <D>",
"<bitOp> <bitexp> <D>",
"&",
"|",
"<notexp> <K>",
"inc",
"dec",
"<notOp> <simple>",
"<simple>",
"!",
"(<Exp>)",
"<integerliteral>",
"<booleanliteral>",
"<charliteral>",
"true",
"false",
"TK_Integer",
"TK_Character",
"TK_identifier"
};

rule[2][16]=0;
rule[0][1]=1;
rule[0][2]=1;
rule[0][5]=1;
rule[0][6]=1;
rule[0][9]=0;
rule[0][24]=1;
rule[0][25]=1;
rule[0][26]=1;
rule[1][1]=2;
rule[1][2]=0;
rule[1][3]=0;
rule[1][5]=0;
rule[1][6]=0;
rule[1][20]=0;
rule[1][24]=0;
rule[1][25]=0;
rule[1][26]=0;
rule[2][2]=3;
rule[2][3]=0;
rule[2][5]=0;
rule[2][6]=0;

//rule[2][11]=0;
rule[2][19]=4;
rule[2][20]=4;
rule[2][24]=3;
rule[2][25]=3;
rule[2][26]=3;
rule[2][33]=0;
rule[3][0]=0;
rule[3][5]=5;
rule[3][6]=0;
rule[4][6]=6;
rule[5][1]=7;
rule[6][2]=8;
rule[6][24]=0;
rule[6][25]=0;
rule[6][26]=0;
rule[7][24]=10;
rule[7][25]=11;
rule[7][26]=12;
rule[8][3]=9;
rule[8][40]=0;
rule[9][19]=13;
rule[10][38]=14;
rule[10][40]=0;
rule[11][3]=97;
rule[12][34]=15;
rule[13][7]=16;
rule[13][32]=18;
rule[14][32]=19;
rule[15][7]=17;
rule[16][24]=20;
rule[16][25]=20;
rule[16][26]=20;
rule[16][31]=21;
rule[17][24]=22;
rule[17][25]=22;
rule[17][26]=22;
rule[17][31]=23;
rule[18][35]=0;
rule[18][38]=24;
rule[19][2]=25;
rule[19][3]=26;


rule[19][22]=98;
rule[19][23]=98;
rule[59][27]=0;
rule[59][3]=0;
rule[59][22]=84;
rule[59][23]=85;



rule[19][10]=99;
rule[19][11]=28;
rule[19][12]=29;
rule[19][13]=30;
rule[19][14]=31;

rule[19][15]=-1;
rule[19][16]=32;
rule[19][17]=33;
rule[19][18]=34;
rule[19][19]=25;
rule[19][20]=26;
rule[19][24]=25;
rule[19][25]=25;
rule[19][26]=25;
rule[20][2]=35;
rule[20][3]=35;

rule[20][10]=35;
rule[20][11]=rule[20][12]=rule[20][13]=35;
rule[20][14]=35;

rule[20][16]=rule[20][17]=rule[20][18]=rule[20][19]=35;

rule[20][20]=35;
rule[20][22]=35;
rule[20][23]=35;


rule[20][24]=35;
rule[20][25]=35;
rule[20][26]=35;
rule[20][33]=0;
rule[21][3]=36;
rule[21][20]=36;
rule[22][11]=41;
rule[23][16]=46;
rule[24][18]=48;
rule[25][3]=52;
rule[25][20]=53;
rule[26][10]=37;
rule[27][27]=rule[27][28]=rule[27][29]=rule[27][30]=39;
rule[27][20]=38;
rule[27][3]=38;
rule[28][27]=90;
rule[28][28]=91;
rule[28][29]=91;
rule[28][30]=92;
rule[29][35]=0;
rule[29][38]=40;
rule[30][14]=44;
rule[31][33]=0;
rule[31][15]=45;
rule[31][16]=0;

rule[32][17]=47;
rule[33][19]=50;
rule[34][36]=51;
rule[34][3]=0;
rule[35][21]=55;
rule[35][21]=55;
rule[35][21]=55;

rule[35][22]=84;
rule[35][23]=85;
rule[35][35]=0;
rule[35][36]=54;
rule[35][38]=0;
rule[35][37]=0;
rule[2][17]=0;
rule[2][18]=0;
rule[27][31]=23;

//rule[2][14]=0;
//rule[31][22]=0;
//rule[65][40]=0;


int k;
for(k=40;k<=56;k++)
{
	rule[35][k]=0;
}
rule[36][3]=56;
rule[36][10]=27;
rule[36][20]=56;
for(k=27;k<=30;k++)
{
	rule[36][k]=56;
}
rule[36][34]=56;
rule[37][3]=57;
rule[37][20]=57;
for(k=27;k<=30;k++)
{
	rule[37][k]=57;
}
rule[37][34]=57;

rule[38][3]=56;
rule[38][20]=56;
for(k=27;k<=30;k++)
{
	rule[38][k]=56;
}
rule[38][34]=56;

rule[39][3]=59;
rule[39][20]=59;
for(k=27;k<=30;k++)
{
	rule[39][k]=59;
}
rule[39][34]=59;

rule[40][35]=0;
rule[40][40]=0;
rule[40][42]=58;

rule[41][35]=0;
rule[41][40]=0;
rule[41][42]=0;
rule[41][43]=60;

rule[42][3]=61;
rule[42][20]=61;
for(k=27;k<=30;k++)
{
	rule[42][k]=61;
}
rule[42][34]=61;

rule[43][35]=0;
rule[43][40]=0;
rule[43][42]=0;
rule[43][43]=0;
rule[43][44]=62;
rule[43][45]=62;

rule[44][44]=63;
rule[44][45]=64;

rule[45][3]=65;
rule[45][20]=65;
for(k=27;k<=30;k++)
{
	rule[45][k]=65;
}
rule[45][34]=65;

rule[46][3]=71;
rule[46][20]=71;
for(k=27;k<=30;k++)
{
	rule[46][k]=71;
}
rule[46][34]=71;

rule[47][35]=0;
rule[47][40]=0;
rule[47][42]=0;
rule[47][43]=0;
rule[47][44]=0;
rule[47][45]=0;
rule[47][47]=66;
rule[47][46]=66;
rule[47][48]=66;
rule[47][49]=66;

rule[48][46]=67;
rule[48][47]=68;
rule[48][48]=69;
rule[48][49]=70;

rule[49][3]=72;
rule[49][20]=72;
for(k=27;k<=30;k++)
{
	rule[49][k]=72;
}
rule[49][34]=72;

rule[50][3]=75;
rule[50][20]=75;
for(k=27;k<=30;k++)
{
	rule[50][k]=75;
}
rule[50][34]=75;
rule[59][30]=0;

rule[51][35]=0;
rule[51][40]=0;
rule[51][42]=0;
rule[51][43]=0;
rule[51][44]=0;
rule[51][45]=0;
rule[51][47]=0;
rule[51][46]=0;
rule[51][48]=0;
rule[51][49]=0;
rule[51][50]=74;
rule[51][51]=73;

rule[51][37]=0;

rule[52][35]=0;
rule[52][40]=0;
rule[52][42]=0;
rule[52][43]=0;
rule[52][44]=0;
rule[52][45]=0;
rule[52][47]=0;
rule[52][46]=0;
rule[52][48]=0;
rule[52][49]=0;
rule[52][50]=0;
rule[52][51]=0;
rule[52][52]=76;
rule[52][53]=77;
rule[52][54]=78;

rule[52][37]=0;

rule[54][35]=0;
rule[54][37]=0;

rule[54][40]=0;
rule[54][42]=0;
rule[54][43]=0;
rule[54][44]=0;
rule[54][45]=0;
rule[54][47]=0;
rule[54][46]=0;
rule[54][48]=0;
rule[54][49]=0;
rule[54][50]=0;
rule[54][51]=0;
rule[54][52]=0;
rule[54][53]=0;
rule[54][54]=0;
rule[54][55]=80;
rule[54][56]=80;

rule[53][57]=79;

rule[53][3]=79;
rule[53][20]=79;
for(k=27;k<=30;k++)
{
	rule[53][k]=79;
}
rule[53][34]=79;


rule[55][55]=81;
rule[55][56]=82;


rule[57][3]=83;
rule[57][20]=83;
for(k=27;k<=30;k++)
{
	rule[57][k]=83;
}
rule[57][34]=83;
rule[57][57]=83;


rule[58][3]=87;
rule[58][20]=87;
for(k=27;k<=30;k++)
{
	rule[58][k]=87;
}
rule[58][34]=87;
rule[58][57]=86;

rule[59][22]=84;
	rule[59][23]=85;
	rule[59][35]=0;
	rule[59][35]=0;
	rule[59][37]=0;
	
	rule[59][40]=0;
	rule[59][42]=0;
	rule[59][43]=0;
	rule[59][44]=0;
	rule[59][45]=0;
	rule[59][47]=0;
	rule[59][46]=0;
	rule[59][48]=0;
	rule[59][49]=0;
	rule[59][50]=0;
	rule[59][51]=0;
	rule[59][52]=0;
	rule[59][53]=0;
	rule[59][54]=0;
	rule[59][55]=0;
	rule[59][56]=0;

	rule[60][3]=42;
	rule[60][20]=42;
	rule[60][27]=43;
	rule[60][28]=43;
	rule[60][29]=43;
	rule[60][30]=43;
	rule[60][34]=89;
	rule[61][57]=88;
	rule[61][27]=87;
	rule[61][28]=87;
	rule[61][29]=87;
	rule[61][30]=87;
	rule[62][27]=95;
	rule[63][28]=93;
	rule[63][29]=94;
	rule[64][30]=96;
	rule[65][3]=42;
	rule[65][20]=42;
	rule[65][27]=43;
	rule[65][28]=43;
	rule[65][29]=43;
	rule[65][30]=43;	
}


void basic1(node *x)
{
	node *pare=x->par;
	if(x->child[0]==NULL)return;
		x->child[0]->rules_info=x->rules_info;
		x->child[0]->child_no=x->child_no;
		pare->child[x->child_no]=x->child[0];	
}

void dfs(node *x)
{
	if(x==NULL)return;

		if(strcmp(x->str,";")==0 || strcmp(x->str,")")==0 ||
		strcmp(x->str,"(")==0 || strcmp(x->str,",")==0 
			|| strcmp(x->str,":")==0 ){
	
		int i;
		for (i = 0; i < x->par->nc ; ++i)
		{
			if(x->par->child[i]==NULL)continue;
			if(strcmp(x->par->child[i]->str , x->str)==0) {
				x->par->child[i]=NULL;
			}
		}
		return;
	}


	node *pare;
	pare=x->par;
	int len=x->nc;
	int i;
	for (i = 0; i<len; ++i)
	{
		if( (x->child[i]!=NULL) && x->child[i]->type>=0 )
			dfs(x->child[i]);
	}
	if(x->rules_info==9 ||x->rules_info==92 || x->rules_info==96 )
	{
		basic1(x);
	}


	if(x->rules_info==43){ 
		for (i = 0; i < x->nc ; ++i)
			if(x->child[i]!=NULL){
				x->child[i]->rules_info=x->rules_info;
				x->child[i]->child_no=x->child_no;
				pare->child[x->child_no]=x->child[i];			
			}
		return;
	}
	if(x->rules_info==14)
	{
		pare=x->par;
		if(x->child[1]==NULL) return;
		x->child[1]->rules_info  = x->rules_info;
		x->child[1]->child_no    = x->child_no;
		pare->child[x->child_no] = x->child[1];	
	}
	if (x->rules_info==52)
	{
		if(x->child[1]==NULL)
			basic1(x);
	}
	if(  x->rules_info==53 || x->rules_info==84  )
	{
		basic1(x);
	}
	if(x->rules_info==35)
	{
		if(x->child[1]==NULL)
		{
			basic1(x);
		}
	}

	if( x->rules_info==18 ||
		x->rules_info==87 ||
		x->rules_info==25 || 
		x->rules_info==26 || 
		x->rules_info==27 || 
		x->rules_info==28 || 
		x->rules_info==29 ||
		x->rules_info==30 || 
		x->rules_info==31 ||
		x->rules_info==32 || 
		x->rules_info==33 ||
		x->rules_info==34 ||
		x->rules_info==42 ||
		x->rules_info==56 ||
		x->rules_info==71
		)
	{
		basic1(x);
		return;	
	}

	if( x->rules_info==89  )
	{
		pare=x->par;
		if(x->child[1]==NULL)return;
		x->child[1]->rules_info=x->rules_info;
		x->child[1]->child_no=x->child_no;
		pare->child[x->child_no]=x->child[1];	
	}
	if(	x->rules_info==79 ||
		x->rules_info==75 ||
		x->rules_info==72 ||
		x->rules_info==65 || 
		x->rules_info==61 ||
		x->rules_info==59 || 
		x->rules_info==57  
		)
	{
		if( x->child[1]!=NULL )
		{
			x->child[1]->child[0]=x->child[0];
			x->child[1]->rules_info=x->rules_info;
			x->child[1]->child_no=x->child_no;
			pare->child[x->child_no]=x->child[1];
		}	
		else
		{
			basic1(x);
		}
	}
	if( x->rules_info==66 ||
		x->rules_info==62 ||
		x->rules_info==60 ||
		x->rules_info==58 ||
		x->rules_info==73 ||
		x->rules_info==74 ||
		x->rules_info==76 ||
		x->rules_info==77 ||
		x->rules_info==78 ||
		x->rules_info==80 
		){
		if(x->child[0]!=NULL)
		{
			x->str=strdup(x->child[0]->str);
			x->child[0]=NULL;
		}
		return;
	}
	if(x->rules_info==6)
	{
		x->str=strdup(x->child[0]->str);
		x->child[0]=NULL;
		return;
	}
	if(    x->rules_info==5
	    || x->rules_info==41
	    || x->rules_info==44
	    || x->rules_info==50 
	    || x->rules_info==45
	    || x->rules_info==46
	    || x->rules_info==99
	    
	    ){

		x->str=strdup(x->child[0]->str);
		x->child[0]=NULL;
		return;
	}
	if (x->rules_info==36)
	{
		x->str=strdup(x->child[1]->str);
		x->child[1]=NULL;
	}
	if(x->rules_info==83){
		if(x->child[0]==NULL)
		{
			if(x->child[1]==NULL)return;
				x->child[1]->rules_info=x->rules_info;
				x->child[1]->child_no=x->child_no;
				pare->child[x->child_no]=x->child[1];
		}
		else
		{
			x->str=strdup(x->child[0]->str);
			x->child[0]=NULL;
		}
	}
	if(
	   x->rules_info==10 ||
	   x->rules_info==11 ||
	   x->rules_info==12 ||
	   x->rules_info==82 ||
	   x->rules_info==64 ||
	   x->rules_info==63 ||
	   x->rules_info==67 || 
	   x->rules_info==68 ||
	   x->rules_info==69 || 
	   x->rules_info==70 ||
	   x->rules_info==23 ||
	   x->rules_info==22 ||
	   x->rules_info==97 
	 ||x->rules_info==81  
	 ||x->rules_info==87 
	 ||x->rules_info==90
	 ||x->rules_info==95)
	{	
		basic1(x);
		return;
	}
}

int regno=0;
int generatecode1(node *x)
{
	int reg=0;
	ndfs(x->child[2]);

	if(strcmp(x->child[0]->str,"<arraystmt>")!=0){
		fprintf(code,"pop eax\n");
		fprintf(code,"mov %s,eax\n",x->child[0]->str);
	}
	else
	{
		fprintf(code,"lea ecx,%s\n",x->child[0]->child[0]->str);
		fprintf(code, "mov ebx,%s\n",x->child[0]->child[1]->child[1]->str );
		fprintf(code, "mov [ebx + ecx], eax\n");
	}

}


int ndfs(node *x)
{
	if(x==NULL)return;
	int len=x->nc;
	int i;
	for (i = 0; i<len; ++i)
	{
		if( x->child[i]!=NULL)
		{
			ndfs(x->child[i]);
		}
	}
	if(strcmp(x->str,"+")==0 
		|| strcmp(x->str,"-" )==0|| 
			strcmp(x->str,"*" )==0||
				 strcmp(x->str,"/")==0)
	{
			fprintf(code,"pop eax\n");
			fprintf(code,"pop ebx\n");
	
			if(strcmp(x->str,"+")==0)
				fprintf(code,"add eax,ebx\n");
	
			if(strcmp(x->str,"*")==0 )
				fprintf(code,"mul eax,ebx\n");
				fprintf(code,"push eax\n");
	}
	else
	{
			fprintf(code,"mov eax, %s\n",x->str );
			fprintf(code,"push eax\n");
	} 
}

void dfs1(node *x,node *par)
{
	if(x==NULL)return;
	int len=x->nc;
	int i;
	x->par=par;
	for (i = 0; i<len; ++i)
	{
		if(x->child[i]!=NULL){
			x->child[i]->child_no=i;
		}
	
		dfs1(x->child[i],x);
	}
}
int blox;
void dfsx(node *x)
{
	if(x==NULL)return;
	int len=x->nc;
	int i;
	for (i = 0; i<len; ++i)
	{
		if( x->child[i]!=NULL && x->child[i]->type!=-1)
		{
			dfsx(x->child[i]);
		}
		else
			x->child[i]=NULL;
	}
	printf("%s \n",x->str);
	
}

void dfs7(node *x,FILE *fp,int spaces)
{
	if(x==NULL)return;
	int len=x->nc;
	int i;
	int test=0;
	for(i=0;i<spaces;i++)
	{
		fprintf(fp," ");
	}
	fprintf(fp,"%s %d\n",x->str,x->type);
	for (i = 0; i<len; ++i)
	{
		if( x->child[i]!=NULL && x->child[i]->type!=-1)
		{
			dfs7(x->child[i],fp,spaces+1);
		}
		else
			x->child[i]=NULL;
	}
}
int typechecking;
void dfs_typecheck(symbol_table *S,node *x)
{
	int len=x->nc;
	int i;	

	for (i = 0; i<len; ++i)
	{
		if( x->child[i]!=NULL)
		{
			dfs_typecheck(S,x->child[i]);
		}
	}
	if( strcmp(x->str,"<arraystmt>")==0||
		strcmp(x->str,"<X>")==0 ||
		strcmp(x->str,"[")==0 ||
		
		strcmp(x->str,"]")==0 ||
		
		strcmp(x->str,"==")==0 ||
		strcmp(x->str,"call")==0||
		strcmp(x->str,">")==0 ||
		strcmp(x->str,"<")==0 ||
		strcmp(x->str,">=")==0||
		strcmp(x->str,"<=")==0||
		strcmp(x->str,":=")==0||
		strcmp(x->str,"*")==0 || 
	    strcmp(x->str,"+")==0 ||
	    strcmp(x->str,"-")==0 ||
	    strcmp(x->str,"|")==0 ||
	    strcmp(x->str,"&")==0)
	{
		return ;
	}
	if ( x->type==1 )
	{
		//printf("\n%s\n\n",x->str);
		int type_of_var=lookup_symbol(S,x->str);
		//printf("type:%d\n",typechecking);
		//printf("%d\n",type_of_var);
		if(typechecking==0)
		{
			typechecking=type_of_var;
		}

		else if(!(typechecking==1&&(type_of_var==1 || type_of_var==2 )))
		{
			typechecking=-4;
			return;
		}
		else if (typechecking==2 && type_of_var!=2)
		{
			typechecking=-4;
			return;	
		}
	}
	else
	{
		//printf("zzz%d\n",typechecking);
		if (typechecking==1)
		{
		//	printf("zz%d %s\n",x->type,x->str);
			if(x->type==2 || x->type==3 )
			{
				return;
			}
			else
			{
				typechecking=-4;
				return;
			}
		}
		if(x->type==3){
			if(typechecking!=2)
			{
				printf("errorin\n");
				typechecking=-4;
				return;
			}
		
		}
	}
}
int function_type;

void dfs3(node *x,FILE *fp,int spaces)
{
	if(strcmp(x->str,"function")==0)
	{	
		symbol_table *S=symTab;
	
		printf("%s\n", x->child[2]->str);
		int flag=overloading_check( S ,x->child[2]->str);
		if(flag==1){
			printf("invalid function name\n");
		}
		if(strcmp(x->child[1]->str ,"void")==0)
		function_type=0;
		if(strcmp(x->child[1]->str ,"int")==0)
		function_type=1;
		if(strcmp(x->child[1]->str ,"char")==0)
		function_type=2;
		if(strcmp(x->child[1]->str ,"boolean")==0)
		function_type=4;	
	}
	if(strcmp(x->str,"return")==0)
	{	

		int count=0;
		int number=blox;
		int rev=0; 
		while(number > 0)
		{
			rev=rev*10+number%10;
			number=number/10;
		}
		symbol_table *S=symTab;
		while(rev>0)
		{
			S=S->child[(rev%10)-1];
			//fprintf(fp,"\nbloccccckno:%d\n",S->blockCount);
			rev=rev/10;
		}
		if(x->child[1]->type==2)
		{	
			if(function_type!=1)
			{
				printf("error_in_return_type\n");
			}
		}
		else	if(x->child[1]->type==3)
		{
			if(function_type!=2)
			{
				printf("error_in_return_type\n");
			}
		}
		else if( strcmp(x->child[1]->str,"NULL")==0 )
		{
			if(function_type!=0)
			{
				printf("error_in_return_type\n");
			}
		}
		else if(strcmp(x->child[1]->str,"TRUE")==0 || strcmp(x->child[1]->str,"FALSE")==0 )
		{
			if(function_type!=4)
			{
				printf("error_in_return_type\n");
			}
		}
		else
		{
			int ret_type=lookup_symbol(S,x->child[1]->str);
			printf("%s\n",x->child[1]->str);
			printf("%d %d\n",ret_type,function_type);
			if(ret_type!=function_type)
			{
				printf("error_in_return_type\n");	
			}	
		}
	}
	if(strcmp(x->str,"{")==0)
	{	
		childx[blox]++;
		blox=blox*10+childx[blox];
		fprintf(fp,"\nblox_no%d \n\n",blox);
	}
	if ( strcmp(x->str,"}")==0)
	{
		blox=blox/10;

	}
	if(x==NULL)return;
	int len=x->nc;
	int i;
	int test=0;
	for(i=0;i<spaces;i++)
	{
		fprintf(fp," ");
	}
	if(strcmp(x->str,"inc")==0)
	{
		node *zz=x->par->child[1];
		int count=0;
		int number=blox;
		int rev=0; 
		while(number > 0)
		{
			rev=rev*10+number%10;
			number=number/10;
		}
		symbol_table *S=symTab;
		while(rev>0)
		{
			S=S->child[(rev%10)-1];
			//fprintf(fp,"\nbloccccckno:%d\n",S->blockCount);
			rev=rev/10;
		}
		int cal=4;
		cal= lookup_symbol(S,zz->str);
		if(!(cal==1 || cal==2 ))
		{
			printf("inc can onlu be used with char or int\n");
		}

	}

	if(x->par!=NULL)
	fprintf(fp,"%s %d\n",x->str,x->child_no);
	if(strcmp("<declarations>",x->str)==0)
	{	
		node *qq;
		
		if(x->child[0]==NULL){
			qq=x;
			x=x->child[1];
			if(strcmp(x->str,"int")==0) 
			{
				fprintf(code,"\t%s\tdd\t?\n",x->par->child[2]->str);
			}
			if(strcmp(x->str,"char")==0 )
			{	
				fprintf(code,"\t%s\tdw\t?\n",x->par->child[2]->str);
			}
			if(strcmp(x->str,"boolean")==0 )
			{
				fprintf(code,"\t%s\tdd\t?\n",x->par->child[2]->str);
			}
			x=qq;
		}
		else
		{
			qq=x;
			x=x->child[0]->child[0];

			if(strcmp(x->child[2]->str,"int")==0) 
			{
				fprintf(code,"\t%s\tdd\t%s\tdup(?)\n",qq->child[0]->child[1]->str,x->child[5]->str);
			}
			if(strcmp(x->child[2]->str,"char")==0 )
			{	
				fprintf(code,"\t%s\tdw\t%s\tdup(?)\n",qq->child[0]->child[1]->str,x->child[5]->str);
			}
			if(strcmp(x->child[2]->str,"boolean")==0 )
			{
				fprintf(code,"\t%s\tdd\t%s\tdup(?)\n",qq->child[0]->child[1]->str,x->child[5]->str);
			}
			x=qq;
		}
	}
	if(strcmp(x->str,":=")==0
	|| strcmp(x->str,">")==0
	|| strcmp(x->str,"<")==0 
	|| strcmp(x->str,">=")==0
	|| strcmp(x->str,"<=")==0
	|| strcmp(x->str,"==")==0)
	{
		int count=0;
		int number=blox;
		int rev=0; 
		while(number > 0)
		{
			rev=rev*10+number%10;
			number=number/10;
		}
		symbol_table *S=symTab;
		while(rev>0)
		{
			S=S->child[(rev%10)-1];
			rev=rev/10;
		}
		
		if(x->child[2]!=NULL && strcmp(x->child[2]->str,"<functioncall>")==0 )
		{
			int type_of_variable=lookup_symbol(symTab,x->child[2]->child[1]->str);
			int typeoftoreturned=lookup_symbol(S,x->child[0]->str);
			if(typeoftoreturned!=type_of_variable)
			printf("typecorret:-4\n");
			else{
			}	
			printf("zuma------------------%s\n",x->child[2]->child[1]->str);
		}
		else
		{
			typechecking=0;
			dfs_typecheck(S,x);
			printf("typecorret:%d\n",typechecking);
		}	
		
	}
	for (i = 0; i<len; ++i)
	{
		if( x->child[i]!=NULL)
		{
			dfs3(x->child[i],fp,spaces+1);
		}
	}
}

int labelno=0;
int flagx=0;
int stackzz[50];
int stackwhile[50];
int stackzzsize=0;
int stackwhilesize=0;
void dfs8(node *x,FILE *fp,int spaces)
{
	if(x==NULL)return;
	int len=x->nc;
	int i;
	int test=0;
	for(i=0;i<spaces;i++)
	{
		fprintf(fp," ");
	}
	
	fprintf(fp,"%s\n",x->str);	
	if(strcmp(x->str,":=")==0)
	{
	    generatecode1(x);
	    printf("\n");
	    dfsx(x);
	}
	if (strcmp(x->str,"}")==0)
	{
		if (flagx==5)
		{
			flagx=0;
			fprintf(code,"label%d:\n",labelno-1);
				
		}
		if (stackzzsize>0)
		{
			fprintf(code,"jmp label%d\n",stackzz[stackzzsize-1]+1);
			fprintf(code,"label%d:\n",stackzz[stackzzsize-1]);
			stackzzsize--;
		}
		if (stackwhilesize>=2)
		{

			fprintf(code,"jmp label%d\n",stackwhile[stackwhilesize-1]);
			fprintf(code,"label%d:\n",stackwhile[stackwhilesize-2]);
			stackwhilesize=stackwhilesize-2;
		}
	}

	if(strcmp(x->str,"else")==0)
	{
		flagx=5;
	}

	if(strcmp(x->str,"while")==0)
	{

		node* temp=x->child[2];
		fprintf(code,"label%d:\n",labelno++);
		stackwhile[stackwhilesize++]=labelno;
		stackwhile[stackwhilesize++]=labelno-1;
	

		if(strcmp("==",temp->str)==0)
		{
			fprintf(code, "mov eax,%s\n",temp->child[0]->str);
			fprintf(code, "mov ebx,%s\n",temp->child[1]->str);
			fprintf(code,"cmp eax,ebx\n" );
			fprintf(code, "jnz label%d\n",labelno++);
		}
		if(strcmp(">=",temp->str)==0)
		{
			fprintf(code, "mov eax,%s\n",temp->child[0]->str);
			fprintf(code, "mov ebx,%s\n",temp->child[1]->str);
			fprintf(code,"cmp eax,ebx\n" );
			fprintf(code, "jl label%d\n",labelno++);
		
		}
		if(strcmp("<=",temp->str)==0)
		{
			fprintf(code, "mov eax,%s\n",temp->child[0]->str);
			fprintf(code, "mov ebx,%s\n",temp->child[1]->str);
			fprintf(code,"cmp eax,ebx\n" );
			fprintf(code, "jg label%d\n",labelno++);
		}
		labelno++;
		flagx=1;	
	}
	if(strcmp(x->str,"if")==0)
	{
		node* temp=x->child[2];
		stackzz[stackzzsize++]=labelno;
		if(strcmp("==",temp->str)==0)
		{
			fprintf(code, "mov eax,%s\n",temp->child[0]->str);
			fprintf(code, "mov eax,%s\n",temp->child[1]->str);
			fprintf(code,"cmp eax,ebx\n" );
			fprintf(code, "jnz label%d\n",labelno++);
		}
		if(strcmp(">=",temp->str)==0)
		{
			fprintf(code, "mov eax,%s\n",temp->child[0]->str);
			fprintf(code, "mov eax,%s\n",temp->child[1]->str);
			fprintf(code,"cmp eax,ebx\n" );
			fprintf(code, "jl label%d\n",labelno++);
		
		}
		if(strcmp("<=",temp->str)==0)
		{
			fprintf(code, "mov eax,%s\n",temp->child[0]->str);
			fprintf(code, "mov eax,%s\n",temp->child[1]->str);
			fprintf(code,"cmp eax,ebx\n" );
			fprintf(code, "jg label%d\n",labelno++);
		}
		labelno++;
		flagx=1;
	}
	for (i = 0; i<len; ++i)
	{
		if( x->child[i]!=NULL && x->child[i]->type!=-1)
		{
			dfs8(x->child[i],fp,spaces+1);
		}
		else
			x->child[i]=NULL;
	}

}


int run_parsing_now(FILE *fp, struct stack_t* stack,hash_tableP *colum, hash_tableP *row)
{
	if (fseek(fp, 0, SEEK_SET)) 
	{
    	puts("Error seeking to start of file");
    	return 1;
  	}
	char input[100];
	fscanf(fp,"%s",input);	
	//printf("%s",input);
	int x,y;
	int line_number=1;
	parse_tree tree;	
	struct stack_t1 *pointer_stack=newStack1();
	tree.root=create_node("<program>",0);
	push1(pointer_stack,tree.root);
	printf("\nstart parsing\n");
	while(stack->stackSize!=0)
	{	
		if(strcmp(top(stack),"$")==0 && strcmp(input,"$"))
		{
			printf("done parsing\n");
			break;
		}
		while(strcmp(input,top(stack))==0)
		{
			if(strcmp(input,"TK_Identifier")==0){
				fscanf(fp,"%s",input);
				node *z=top1(pointer_stack);
				z->str=strdup(input);
				z->type=1;
				z->rules_info=-1;
			}
			if(strcmp(input,"TK_Integer")==0){
				fscanf(fp,"%s",input);		
				node *z=top1(pointer_stack);
				z->str=strdup(input);
				z->value=atoi(input);
				z->type=2;
				z->rules_info=-1;
			}	
			if(strcmp(input,"TK_Character")==0){
				fscanf(fp,"%s",input);		
				node *z=top1(pointer_stack);
				z->str=strdup(input);
				z->type=3;
				z->rules_info=-1;
			}	
			pop(stack);
			pop1(pointer_stack);
			if(strcmp(top(stack),"$")==0 && strcmp(input,"$")){
				printf("done parsing\n");
				goto parsing_done;
			}

			fscanf(fp,"%s",input);	
		}
		if(strcmp(top(stack),"$")==0 && strcmp(input,"$")){
			printf("done parsing\n");
			break;
		}
		x=lookup_stringP(row,top(stack));
		y=lookup_stringP(colum,input);		
		if(x==-1){
			printf("type1-------\nError\n--------\n");
			break;
		}
		if(y==-1){
			printf("type2-------\nError\n--------\n");
			break;
		}
		int rule_value=rule[x][y];
		printf("%d %d %d\n",x,y,rule_value);
		if(rule_value==-1)
		{
			printf("type3-------\nError\n--------\n");
			break;
		}
		if(rule_value==0)
		{
			pop(stack);
			node *z=top1(pointer_stack);
			z->type=-1;
			pop1(pointer_stack);
			continue;
		}
		int i;
		pop(stack);
		int l=RL[rule_value].size;
		node *x=top1(pointer_stack);
		x->nc=l;
		node *y;
		pop1(pointer_stack);
		x->rules_info=rule_value;
		for (i = 0; i <l; ++i)
		{
			push(stack,RL[rule_value].str[l-i-1]);
			y=create_node(RL[rule_value].str[l-i-1],0);
			y->rules_info=-1;
			x->child[l-i-1]=y;
			push1(pointer_stack,y);		
			
		}
	}
	parsing_done:;


	fprintf(code, ".model tiny\n.386\n.data\n");
	fp=fopen("AST_OUTPUT.TXT","w+");
	dfs1(tree.root,NULL);
	dfs7(tree.root,stdin,0);
	printf("-------------------\n\n");
	dfs(tree.root);

	dfs1(tree.root,NULL);
	//dfs7(tree.root,fp,0);
	dfs3(tree.root,fp,0);
	fprintf(code, ".code\n.startup\n");
	fp=fopen("FINAL_AST_OUTPUT.TXT","w+");
	dfs8(tree.root,fp,0);
	fprintf(code, "\n.exit\nend\n");
}
void parsing(FILE* fp1, hash_tableP *colum, hash_tableP *row)
{
	struct stack_t* stack1=newStack();
	char str[100];
	push(stack1,"$");
	push(stack1,"<program>");
	int i=0;
    FILE *fp = fopen ("qwerty.txt", "r+");
    int len=0;
    i=0;
    while(fscanf(fp,"%d",&len)!=EOF)
    {	
    	RL[i].size=len;

    	int j;
    	for (j = 0; j < len; ++j)
    	{
    		fscanf(fp,"%s",RL[i].str[j]);	
    	}
    	i++;
    }
    close(fp);
  //  printf("%s\n",RL[98].str[0]);
    run_parsing_now(fp1,stack1,colum,row);
}

char prev[100];
char word[100];


int isAlpha(char ch){
	if((ch>=97 && ch<='z') || (ch>='A' && ch<='Z') )
		return 1;
	else
		return 0;
}
int isNum(char ch)
{
	if((ch>='0' && ch<='9'))
		return 1;
	else
		return 0;
}
void print_word(FILE *fp,int len){
	int i;
	for (i = 0; i <len; ++i)
	{
		fprintf(fp,"%c",word[i]);
	}
	
}

int main(int argc, char const *argv[])
{
	symTab=create_symbol_tableGLOBAL(100);
	code=fopen("Code.asm","w+");
	int x;	
	x=0;
	char ch=getchar();
	hash_table *my_hash_table;
	int size_of_table = 197;
	my_hash_table = create_hash_table(size_of_table);
	add_string(my_hash_table,"function", "TK_Function");
    add_string(my_hash_table,"while", "TK_While");
    add_string(my_hash_table,"boolean", "TK_Bool");
    add_string(my_hash_table,"create", "TK_Create");
	add_string(my_hash_table,"int", "TK_Int");
	add_string(my_hash_table,"char", "TK_Char");
	add_string(my_hash_table,"input", "TK_Input");
	add_string(my_hash_table,"output", "TK_Output");
	add_string(my_hash_table,"if", "TK_If");
    add_string(my_hash_table,"else", "TK_Else");
    add_string(my_hash_table,"array", "TK_Array");
    add_string(my_hash_table,"true", "TK_True");
    add_string(my_hash_table,"false", "TK_False");
    add_string(my_hash_table,"return", "TK_Return");
    add_string(my_hash_table,"next", "TK_Next");
    add_string(my_hash_table,"end", "TK_End");
    add_string(my_hash_table,"try", "TK_Try");
    add_string(my_hash_table,"catch", "TK_Catch");
    add_string(my_hash_table,"finally", "TK_Finally");
    add_string(my_hash_table,"constant", "TK_Constant");
    add_string(my_hash_table,"call", "TK_Call");
    add_string(my_hash_table,"dec", "TK_Dec");
    add_string(my_hash_table,"inc", "TK_Inc");
    add_string(my_hash_table,"main", "TK_Main");
    add_string(my_hash_table,"void", "TK_Void");
    
    FILE * fp;
   	fp = fopen ("output.txt", "w+");
	//	ch=getchar();
	int lineNo=1;
	int blockNo=0;
	while(1)
	{
		if(ch==EOF)break;
		
		x=0;
		
		if(ch==' '|| ch=='\t'){
			
			ch=getchar();
			continue;
		}

		if(ch=='\n'){
			lineNo++;
			ch=getchar();
			continue;
		}

		
		
		if(isAlpha(ch)){
			word[x++]=ch;			
			ch=getchar();

			while(isAlpha(ch) || isNum(ch) )
			{
				word[x++]=ch;			
				ch=getchar();
			}
			word[x]='\0';
			element *ans=lookup_string(my_hash_table,word);
			if(ans!=NULL)
				strcpy(prev,ans->string);
			if(NULL==ans)
			{
				fprintf(fp,"TK_Identifier ");
				if(strcmp(prev,"int\0")!=0 && strcmp(prev,"char\0")!=0 && strcmp(prev,"void\0")!=0 
					&& strcmp(prev,"function\0")!=0 && strcmp(prev,"boolean\0")!=0 && strcmp(prev,"call\0")!=0)
					strcpy(prev,"use");
				if(strcmp("NULL",word)!=0 && strcmp("TRUE",word)!=0 && strcmp("FALSE",word)!=0)
					{
					printf("%s\t%s\t%d\t%d\n",prev, word, lineNo, symTab->blockCount);
					add_symbol_lexical(symTab, prev, word,lineNo,symTab->blockCount);
					}

			}
			
			print_word(fp,x);
			fprintf(fp," \n");
			continue;
		
		}
		 if(isNum(ch))
		{
			x=0;
			word[x++] = ch;			
			ch=getchar();	
			
			while(isNum(ch))
			{	
				word[x++] = ch;			
				ch=getchar();	
			}
			fprintf(fp,"TK_Integer ");
			print_word(fp,x);
			fprintf(fp," \n");
			continue;
		}
		else if(ch=='[' || ch==']' || 
				 ch=='(' ||
				ch==')' || ch==34  ||
				ch==39	|| ch==',' ||
				ch=='%' || ch=='#' )
		{
			
			if(ch==39)
			{
				ch=getchar();
				fprintf(fp,"TK_Character %c\n",ch);
				ch=getchar();	
					
				if(ch!=39){
					fprintf(fp,"error_using single_quo\n");
					close(fp);
					return 0;
				}
					
			//	ch=getchar();
			}else
			{
			fprintf(fp,"%c\n",ch);
			}
			ch=getchar();
			
			continue;	
		}

		else if(ch==';') 
		{
		
		fprintf(fp,"%c\n",ch);
		strcpy(prev,"use");
		ch=getchar();
		continue;	
		}
		else if(ch=='{') 
		{
			//blockNo++;
			blockNo=symTab->blockCount*10;
			//increase number of children as new scope opens
			int childNo=symTab->children++;
			//create the new scope
			symTab=create_symbol_table(symTab,100);
			//store pointer of new scope in parent scope
			symTab->prev->child[childNo]=symTab;
			//assign the unique blockCount to the new scope based on no of children in parent
			symTab->blockCount=blockNo + symTab->prev->children;
			printf("%d\n",symTab->blockCount);
			fprintf(fp,"%c\n",ch);
			ch=getchar();
			continue;	
		}
		else if(ch=='}') 
		{
			blockNo=symTab->blockCount/10;
		//	blockNo--;
			symTab=CloseScope(symTab);
			fprintf(fp,"%c\n",ch);
			ch=getchar();
			continue;
		}

		else
		{
			switch (ch)
			{

				case ':':
					ch=getchar();
					switch (ch)
					{
						case '=' :
							fprintf(fp,":=\n");
							break;
						default:
							fprintf(fp,":\n");
							continue;
					}
					break;
				case '=':
					ch=getchar();
					switch (ch)
					{
						case '=' :
							fprintf(fp,"==\n");
							break;
						default:
							fprintf(fp,"invalid symbol\n");
							continue;
					}
					break;
				case '<':
					ch=getchar();
					switch (ch)
					{
						case '=' :
							fprintf(fp,"<=\n");
							break;
						case '<' :
							fprintf(fp,"<<\n");
							break;	
						default :
							fprintf(fp,"<\n");
							continue;
					}
					break;
				case '>' :
					ch=getchar();
					switch (ch)
					{
						case '=' :
							fprintf(fp,">=\n");
							break;
						case '>':
						fprintf(fp,">> \n");
						break;
						default:
						fprintf(fp,">\n");
							continue;
					}
					break;
				case '!' :
					ch=getchar();
					switch (ch)
					{
						case '=' :
							fprintf(fp,"!= T\n");
						break;
						default:
							fprintf(fp,"! T\n");
							continue;
					}
					break;
				case '+' :
					ch=getchar();
					switch (ch)
					{
						case '+' :
							fprintf(fp,"++\n");
						break;
						default:
							fprintf(fp,"+\n");
							continue;
					}
					break;	

				case '-' :
					ch=getchar();
					switch (ch)
					{
						case '-' :
							fprintf(fp,"--\n");
						break;
						default:
							fprintf(fp,"-\n");
							continue;
					}
					break;	
				case '*' :
					fprintf(fp,"*\n");
					break;
					
				case '/' :
					fprintf(fp,"/\n");
					break;
				case '|' :
					ch=getchar();
					switch (ch)
					{
						case '|' :
							fprintf(fp,"||\n");
						break;
						default:
							fprintf(fp,"|\n");
							continue;
					}
					break;	

				case '&' :
					ch=getchar();
					switch (ch)
					{
						case '&' :
							fprintf(fp,"&&\n");
						break;
						default:
							fprintf(fp,"&\n");
							continue;
					}
					break;	
				
				case '#' :
					ch=getchar();
					fprintf(fp,"#\n");
					continue;
				break;
				
			}
			ch=getchar();
		}
		
	}
	if (fseek(fp, 0, SEEK_SET)) 
  	{
    	puts("Error seeking to start of file");
    	return 1;
  	}
	close(fp);
	printf("--------------------");
	

	//execute parser code now
	construct_terminal();
	construct_nonterminal();
    construct_rule();
	hash_tableP *colum,*row;
   // int size_of_table = 197;
    colum = create_hash_tableP(size_of_table);
    row= create_hash_tableP(size_of_table);
   // FILE * fp;
    fp = fopen ("terminal.txt", "r+");
    char str[100];
    int i=0;
    while(fscanf(fp,"%s",str)!=EOF)
    {
        add_stringP(colum,str,i++);
    }
    close(fp);
    fp = fopen ("int.txt", "r+");
    i=0;
    while(fscanf(fp,"%s",str)!=EOF)
    {
        add_stringP(row,str,i++);
    }
    close(fp);
    fp = fopen ("output.txt", "r+");
    if (fseek(fp, 0, SEEK_SET)) 
  	{
    puts("Error seeking to start of file");
    return 1;
  	}
  	//execute parser code now
   parsing(fp,colum,row);
	
	//execute AST generation code now

	//execute semantic analysis code now
   //printf("%d\n", symTab->blockCount );
   	type_check(symTab,3);
   	
 close(code);

	return 0;
}
