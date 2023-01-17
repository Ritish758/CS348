%{ 
    // IMPORTS header files
    #include <ctype.h> 
    #include <stdio.h> 
    #include <stdlib.h>
    #include <stdint.h>
    #include "headers.h"

    // struct to store variable declaration lists
    struct DeclarVariables {
       int size;
       YYSTYPE varlist[500];
    } tempDeclarVariables = {0};                // DeclarVariables object to store variable declaration lists

    // global variables to store results and debugging
    int semanticErrorCount;                        // 1 if error is encountered, 0 otherwise
    int syntaxErrorCount;                           // 1 if error is encountered, 0 otherwise
    int isSemantic;
    int flag=1,flag1=1;
    int flag2=1;

    // Common identifiers for int and real tokens and default 
    int defaultIdentifier;
    int intIdentifier;
    int realIdentifier;
    
    // Clears decvar after variable declaration stmt finishes
    void clearDecVar(struct DeclarVariables* decVar){
        decVar->size=0;                         // Sets size to 0
        return;
    }
    
    
    // Inserts variable's symbol table pointer into decvars list
    void insertDecVar(struct DeclarVariables* decVar, YYSTYPE varptr){
        if(decVar->size == 500) {              // Checks if full
            printf("DECVAR FULL\n");
            return;
        }
        decVar->size = decVar->size +1;                             // Increments size
         decVar->varlist[decVar->size-1]=varptr;       // Inserts into decvar
    }
    
    void terminalprints(){  // used for debugging and flag updates
      flag=1;
      flag1=1;
      flag2=1;
    
    }

    // Checks if variable is declared
    int isDeclared(YYSTYPE var){
        struct Node* node = (struct Node*)var;      // Gets symbol table node entry from pointer
        if(node->datatype == 0){return 0;}
        else{return 1;}               // returns 1 if variable is declared 
    }

    // returns type of the variable pointed by var
    YYSTYPE getType(YYSTYPE var){
        struct Node* node = (struct Node*)var;                              // Gets symbol table node entry from pointer
        if(node->datatype!=1 && node->datatype!=2){return (YYSTYPE)(&defaultIdentifier); }
        else if(node->datatype == 1) return (YYSTYPE)(&intIdentifier);           // If datatyupe is int, returns int identifier
        else if(node->datatype == 2) return (YYSTYPE)(&realIdentifier);     // Else, return real identifier      
        else{return (YYSTYPE)(&defaultIdentifier); }                           
    }

    // Flag to check if inside body or in declaration
    int inBody;


    // Extending visibility as these are declared in .l file
    extern int yylex();
    extern int line;          // Stores current line no
    extern FILE *yyin;          // File ptr for input file
    extern void printhashtable();

    // returns name of variable from symbol table
    
    char* getname(struct Node* node){  // get name as char* from node
        return node->symName;
    }
    
    char* getName(YYSTYPE var){
        struct Node* node = (struct Node*)var;      // Gets symbol table node entry from pointer 
        return getname(node);                      // Returns symbol's name
    }

    // Declaration of yyerror, takes error string as input
    void yyerror(const char* s){
        printf("Line = %d  ->  %s\n", line,s);     // Prints error with line number
        if(isSemantic==0){syntaxErrorCount++;}   // increase syntax count if no semantic error 
        else {semanticErrorCount++;}            // increase semantic error else
        isSemantic = 0;  // update if sementic error or not
        
    }


    // Gives error if variable not declared
    int checkDeclared(YYSTYPE var){
        if(isDeclared(var)){return 1;}
        else{              // Checks if variable is declared
            char buff[400];                 // Buffer to store error message
            sprintf(buff, "semantic error, variable \'%s\' not declared", getName(var));    // Putting error message into buff
            isSemantic = 1;
            yyerror(buff); return 0;       // Showing the error message
        }
    }
    
    // Gives error if iterator's type is not int
    int checkTypeIterator(YYSTYPE iteratorType, YYSTYPE iid){
      if(iteratorType==iid){return 1;}
        if(iteratorType != iid){
            isSemantic = 1;
            yyerror("semantic error, iterator in FOR loop should be an INTEGER variable"); 
            return 0;
        }
        return 1;           // Returns 1 if no error
    }


    // Gives error if types var1, var2 don't match
    int compareTypes(YYSTYPE var1, YYSTYPE var2){
        if(var1 == (YYSTYPE)(&defaultIdentifier)){ return 1;}   // if anyone is deafult then no error 
        if(var2 == (YYSTYPE)(&defaultIdentifier)) {return 1;}
        if(var1==var2){return 1;}   // if both types are equal then no error
        isSemantic = 1;
        yyerror("semantic error, type mismatch of Real and Integer");       // Showing Type Mismatch semantic error
        return 0;
    }


%} 


// Macro to display verbose syntax errors
%define parse.error verbose
%start prog             // Setting start symbol ie prog



// Declaring tokens and assigning integer codes given in assignment
%token PROGRAM 1                 
%token VAR 2
%token BEGIN_TOKEN 3 
%token END 4 
%token ENDDOT 5 
%token INTEGER 6 
%token REAL 7 
%token FOR 8
%token READ 9 
%token WRITE 10 
%token TO 11 
%token DO 12 
%token SEMICOLON 13 
%token COLON 14
%token COMMA 15 
%token ASSIGNMENT 16 
%token PLUS 17 
%token MINUS 18 
%token MULT 19 
%token DIV 20 
%token OPEN_BRACKET 21
%token CLOSE_BRACKET 22 
%token IDENTIFIER 23
%token INTEGER_VALUE 24 
%token REAL_VALUE 25
%token UNKNOWN_SYMBOL



// Setting the mathematical operators as left associative
%left PLUS MINUS
%left MULT DIV          // Setting higher preference of MULT and DIV operators



%%

// Pascal grammar production rules as mentioned in Assignment
prog : PROGRAM progname VAR declist beginbody stmtlist ENDDOT 
        ;

progname : IDENTIFIER {flag1++;}     // programname is identifier
            ;

beginbody : BEGIN_TOKEN { inBody = 1;}      // moving inside body
        ;

declist : dec | declist SEMICOLON dec {flag2++;}    // declist automata
            ;
dec : idlist COLON type { // If declaration is found, set the datatype of variables
            int* typeIdentifier = (int*)$3;     // Get type from the value $3
            if(typeIdentifier == (&intIdentifier) || typeIdentifier == (&realIdentifier)){
                int i=0;
                while(i<tempDeclarVariables.size) {               // Iterating through the variable declaration list
                    struct Node* node = (struct Node*)(tempDeclarVariables.varlist[i]); // Getting symbol table node pointer
                    if(node->datatype != 0) {   
                        isSemantic = 1;                                // Giving error is variable already declared
                        char buff[400];
                        sprintf(buff, "semantic error, redeclaration of variable \'%s\'", node->symName);
                        yyerror(buff);
                    }
                    // Setting appropriate datatypes inside symbol table
                    if(typeIdentifier==(&intIdentifier)) node->datatype = 1;
                    else if(typeIdentifier==(&realIdentifier)) node->datatype = 2;   // assuming datatype of int is 1 and of real is 2 and all other data types are unknown
                    
                    i++;
                }
            } 
            flag1++;
            clearDecVar(&tempDeclarVariables);          // Clearing the temp declaration list
        }
        | error {
            flag1++;
            clearDecVar(&tempDeclarVariables);          // Clearing the temp declaration list if error occured in dec
        }
        ;
type : INTEGER  { flag2++; $$ = (YYSTYPE)(&intIdentifier); }         // Passing type identifiers forward for real and int
        | REAL  { flag2++;  $$ = (YYSTYPE)(&realIdentifier); }
        ;
idlist : IDENTIFIER     {
                if(!inBody) insertDecVar(&tempDeclarVariables, $1);               // If inside body, check if variable is declared
                else checkDeclared($1); 
                flag++;       // Otherwise, insert in declist
            }
        | idlist COMMA IDENTIFIER {
                if(!inBody) insertDecVar(&tempDeclarVariables, $3);                // If inside body, check if variable is declared
                else checkDeclared($3);      // Otherwise, insert in declist
                flag++;
            }
        ;
stmtlist : stmt | stmtlist SEMICOLON stmt  | stmtlist error stmt  {flag1++;}  // stmtlist automata
            ;
stmt : assign | read | write | for | error  {flag++;}  // stmt automata
        ;
assign : IDENTIFIER ASSIGNMENT exp { if(checkDeclared($1))          // Checking if variable is declared and comparing types
                                        compareTypes(getType($1), $3); flag1++;} 
        ;
exp :  term {$$ = $1;}                                                  // Passing type identifiers ahead
        | exp PLUS term { if(!compareTypes($1, $3)) $$ = (YYSTYPE)(&defaultIdentifier);             // Passing type if no error occured
                            else $$=$1; }                                                           // Otherwise use the defaultidentifier
        | exp MINUS term { if(!compareTypes($1, $3)) $$ = (YYSTYPE)(&defaultIdentifier);            // Passing type if no error occured
                            else $$=$1; }                                                           // Otherwise use the defaultidentifier
        ;

term : factor {$$ = $1;}
        | term MULT factor { if(!compareTypes($1, $3)) $$ = (YYSTYPE)(&defaultIdentifier);          // Passing type if no error occured
                            else $$=$1; }                                                           // Otherwise use the defaultidentifier
        | term DIV factor { if(!compareTypes($1, $3)) $$ = (YYSTYPE)(&defaultIdentifier);          // Passing type if no error occured
                            else $$=$1; }                                                         // Otherwise use the defaultidentifier
        ;
factor : IDENTIFIER { if(!checkDeclared($1)) $$ = (YYSTYPE)(&defaultIdentifier); else $$=getType($1); }        // Checks if variable is declared and forards type into $$
        | INTEGER_VALUE {$$ = (YYSTYPE)(&intIdentifier); flag2++;}   // If factor is an integer value, set type to INT
        | REAL_VALUE {$$ = (YYSTYPE)(&realIdentifier); flag1++;}     // If factor is an real value, set type to real
        | OPEN_BRACKET exp CLOSE_BRACKET {$$ = $2; flag++;}
        ;
read : READ OPEN_BRACKET idlist CLOSE_BRACKET {flag++;}   // read syntax
        ;
write : WRITE OPEN_BRACKET idlist CLOSE_BRACKET  {flag++;}   // write syntax
        ;
for : FOR indexexp DO body | FOR indexexp error body {flag++;}  // for syntax
        ;
indexexp : IDENTIFIER ASSIGNMENT exp TO exp { if(checkDeclared($1)){        // Check if iterator variable is declared
                                                 if(checkTypeIterator((YYSTYPE)&intIdentifier, getType($1))){
                                                    if(compareTypes($3, $5)){
                                                       compareTypes(getType($1), $3);  // Checks if types match
                                                    }
                                                 }
                                                 
                                          }
                                         flag++;       
                                    }
            ;
body : stmt | BEGIN_TOKEN stmtlist END  {flag=1;}   // body syntax
        ;


%% 


void file_error(const char* s){  // file error if input file not found
   printf("%s file not found",s);
   exit(0);
}
  

   
int main(int argc, char* argv[])   // main function
{   
    if(argc<=1){
       printf("Input File not given as commandline argument");
       exit(0);
    }    
    if(argc > 1){                               // Gets file as argument from cmd line
    	FILE *fileptr = fopen(argv[1], "r");
    	if(fileptr)
    		yyin = fileptr;       		                  // Makes yyin equal to file ptr
    	else{
    	   file_error(argv[1]);
    	}	
    }
    printf("\nParsing started...\n\n");
    yyparse();                                  // Starts parsing
    printf("\nParsing Completed\n");
    // Printing appropriate messages on completion of parser
    if(semanticErrorCount + syntaxErrorCount == 0){printf("Input in file %s is accepted\nNo semantic and syntax errors were found.\n",argv[1]);} // If no error encountered, prints "Successful"
    else {
        printf("Input in file \'%s\' is rejected.\n",argv[1]);
        printf("Total syntax errors = %d \n",syntaxErrorCount);
        printf("Total semantic errors = %d \n",semanticErrorCount);
        printf("All errors are listed above\n");
    }
    // printing hashtable of .l file in hashtable.txt
    printhashtable();
    return 0;
}  
