%{
    // header files
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include "headers.h"
    #include "main.tab.h"

    // Decalring hashmap
    struct Node* hashmap[100];
    
    //Declaring global variables
    int line = 1;

    //Declaring function prototypes
    struct Node* install_id();
    struct Node* install_num();
    struct Node* install_real();
    void printhashtable();

%}
num (\+|-)?[0-9]+
ws [ \t]+
id [_A-Za-z][_A-Za-z0-9]*
real {num}(\.[0-9]+)?(E{num})?

%%
"PROGRAM" {  return PROGRAM; }
"VAR" {  return VAR; }
"BEGIN" {  return BEGIN_TOKEN; }
"END" {  return END; }
"END." {  return ENDDOT; }
"INTEGER" {  return INTEGER; }
"REAL" {  return REAL; }
"FOR" {  return FOR; }
"READ" {  return READ; }
"WRITE" {  return WRITE; }
"TO" {  return TO; }
"DO" {  return DO; }
";" {  return SEMICOLON; }
":" {  return COLON; }
"," {  return COMMA; }
":=" {  return ASSIGNMENT; }
"+" {  return PLUS; }
"-" {  return MINUS; }
"*" {  return MULT; }
"DIV" {  return DIV; }
"(" {  return OPEN_BRACKET; }
")" {  return CLOSE_BRACKET; }
"\n" {line++;}           // Counting current line number
{ws} {}
{id} {  struct Node* temp =install_id(); yylval = (YYSTYPE)temp;  return IDENTIFIER; }      // installing identifiers into Symboltable
{num} {  struct Node* temp = install_num(); yylval = (YYSTYPE)temp;  return INTEGER_VALUE; }   // installing integer constants into Symboltable
{real} {  struct Node* temp =install_real(); yylval = (YYSTYPE)temp; return REAL_VALUE; }   // installing real constants into Symboltable

. { return UNKNOWN_SYMBOL; }

%%

int stringcheck(char a[],char b[]){  // checking if 2 strings are same or not
    if(strcmp(a,b)!=0){return 0;}
    return 1;
}

// Searches for symbolname at index:in of Symboltable
struct Node* searchnode(int in, char symn[]){
    struct Node* temp = hashmap[in];
    for(;temp;){                                // Iterates through list till node is found
        if(stringcheck(temp->symName, symn) == 1){ return temp;}
        temp = temp->next;
    }
    return NULL;
}


// Helper fn to insert node at index:in of Symboltable
void insertnode(int in, struct Node* node){
    struct Node* temp = hashmap[in];
    node->next = temp;
    hashmap[in] = node;
}

// Creates new node and initializes it
struct Node* createNode(int tokenType, char symn[]){
    struct Node* temp = (struct Node*)(malloc(sizeof(struct Node)));    // Creates newnode through malloc
    strcpy(temp->symName, symn);                // Copies symbol name into required symname buffer
    temp->next = NULL;                              // Sets next ptr to null
    temp->datatype = 0;    
    temp->tokenType = tokenType;                    // Sets token type
    return temp;                                    
}


// Calculates hash value of symbolname
int gethash(char symn[]){
    int res=0;
    char c;
    for(;c = *symn++;){                 // Polynomial hash is calcuted
        res = res*256 + c;
        res%=100;
    }
    return res;
}

struct Node* present_already(char symn[],int hash){  // checks if node is already present in hashmap
 struct Node* temp = searchnode(hash,symn);
 if(!temp){return NULL;}
 return temp;
}


// Inserts the node for symbolname at appropriate posn of Symboltable
struct Node* insert_node(int tokenType, char symn[]){
   int hashvalue=0,finalvalue=0;
    int hsh = gethash(symn);    
    hashvalue = hsh%2;
    finalvalue = hsh/2;                       // Calculates hash of symbolname
    struct Node* temp = present_already(symn,hsh);
    if(!temp) {                          // Checks if symbolname is already present 
    temp = createNode(tokenType, symn);   // If not, creates new node
    insertnode(hsh, temp);                        // Insert node at index:hsh of Symboltable
   }
    return temp;
}
 
// install the identifier lexeme pointed to by yytext, into the symbol table and return its pointer
struct Node* install_id(){
    int token = IDENTIFIER;
    return (insert_node(token, yytext));    // calls insert_node fn
}

// install the Integer constant lexeme pointed to by yytext, into the symbol table and return its pointer
struct Node* install_num(){
    int token = INTEGER_VALUE;
    return (insert_node(token, yytext));    // calls insert_node fn
}

// install the Real constant lexeme pointed to by yytext, into the symbol table and return its pointer
struct Node* install_real(){
    int token = REAL_VALUE;
    return (insert_node(token, yytext));    // calls insert_node fn
}

void printhashtable(){   // print hashtable in hashtable.txt
 FILE* outptr = fopen("hashtable.txt","w");
 if(!outptr){   // if file could not be openned
  printf("Could not open file for hashtable print");
  return ;
 }
 for(int i=0; i<100; i++){  // iterating thourgh hashmap and its nodes
   struct Node* temp = hashmap[i];
   fprintf(outptr,"buketlist[%d] -> ",i);
   while(temp){
     fprintf(outptr,"{ %s, %d} -> ",temp->symName,temp->tokenType);
     temp=temp->next;
   }
   fprintf(outptr,"NULL\n");
 
 
 }


}

// defining the yywrap function
int yywrap(){
    return 1;
}
