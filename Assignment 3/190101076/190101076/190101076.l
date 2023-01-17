

%{
  // header files
  #include<bits/stdc++.h>
  #include<fstream>
  using namespace std;
  
  // Node for storing token type and its name for hashmap which is same as symtable
  struct symNode{
      int token;     // token for its type
      string symname;      // string for its name
      struct symNode* next;   // next for handling collision in hashmap
  };
  
   // declaring global variables
  int line = 1;
  
  struct symNode* stpointer;
  
  // hashmap
  struct symNode* hashmap[20];
  
  // declaring functions
  struct symNode* install_id();
  struct symNode* install_num();
  
%}

num (\+|-)?[0-9]+
ws [ \t]+
id [_A-Za-z][_A-Za-z0-9]*


%%

"PROGRAM" {  return 1; }
"VAR" {  return 2; }
"BEGIN" {  return 3; }
"END" {  return 4; }
"END." {  return 5; }
"INTEGER" {  return 6; }
"FOR" {  return 7; }
"READ" {  return 8; }
"WRITE" {  return 9; }
"TO" {  return 10; }
"DO" {  return 11; }
";" {  return 12; }
":" {  return 13; }
"," {  return 14; }
":=" {  return 15; }
"+" {  return 16; }
"-" {  return 17; }
"*" {  return 18; }
"DIV" {  return 19; }
"(" {  return 20; }
")" {  return 21; }

"\n" {line++; }           // Counting line numbers
{ws} {}
{id} {  stpointer = install_id(); return 22; }      // installing identifiers into hashtable
{num} {  stpointer = install_num(); return 23; }   // installing integer constants into hashtable


%%


// insert node in hashmap
void insertnode(struct symNode* node, int index){
     struct symNode* temp = hashmap[index];  // getting hashmap node
     node->next = temp;  // instering it
    hashmap[index] = node;  // writing back to hashmap
}

// declaring manifests constants
int NUM_TT(){return 23;}
int ID_TT(){return 22;}

// search node in hashmap index
struct symNode* searchnode(string s, int index){
       struct symNode* temp = hashmap[index];  // getting hashmap node
       while(temp){ // iterating through it
         if(temp->symname == s){return temp;} // return if same is found
         temp=temp->next;
       }
       return NULL;
}
// declaring sizes
int getsize(){return 20;}

// get hash value for hashmap
int gethash(string s){
    int ans=0;
    for(auto x: s){
      ans=ans*256+x;
      ans=ans%(getsize());
    }
    if(ans<0){ans=ans+getsize();}
    return ans;
}


// create new node of type symnode
struct symNode* createNode(string s, int token){
   struct symNode* temp = (struct symNode*)(malloc(sizeof(struct symNode)));  // creating new using malloc
   temp-> next = NULL;   // declarations for new node
   temp-> token = token;
   temp-> symname = s;
   return temp;
}

//  insert node for symbol name and on its correct position
struct symNode* insert_node(string s, int token){
    int hash = gethash(s);       // get hash value
    struct symNode* temp = searchnode(s,hash);   // search node through hash value
    if(temp==NULL){  // if not found create new
    temp = createNode(s,token);
    insertnode(temp, hash);
    }
    return temp;
}

// install the identifier lexeme pointed to by yytext, into the hashtable and return it
struct symNode* install_id(){
     string s = yytext;
     int token = ID_TT();
     struct symNode* ans = insert_node(yytext,token);
    return ans;    // calls insert_node fn
}

// install the Integer constant lexeme pointed to by yytext, into the hashtable and return it
struct symNode* install_num(){
    string s = yytext;
     int token = NUM_TT();
     struct symNode* ans = insert_node(yytext,token);
    return ans;    // calls insert_node fn
}

void file_error(bool type){
    if(type){
      cout<<"Input File Not found\n Plz add input.txt with appropriate input in same directory\n";
    }
    else{
       cout<<"Error in creating output file\n";
    }
  exit(0);

}

// defining the yywrap function
int yywrap(){
    return 1;
}

int main(){

    FILE *inptr = fopen("input.txt","r");
    if(inptr){yyin = inptr;}
    else{file_error(1);}		
    FILE *outptr;
    outptr = fopen("output.txt","w");
    if(!outptr){file_error(0);}
    int token;
 
    fprintf(outptr,"Line          Token          Token Type          Token Specifier\n");
    while(token = yylex()){  // iterating all lines
       char st = 0;
       if(token == ID_TT()){   // if token is identifier
          st = '^';
          fprintf(outptr,"%-13d %-14s %-19d %c%-23s \n", line, yytext, token, st, yytext);
       }
       else if(token == NUM_TT()){  // if token is integer constant
          st = '#';
          fprintf(outptr,"%-13d %-14s %-19d %c%-23s \n", line, yytext, token, st, yytext);
       }
       else{   // all remaining cases
         fprintf(outptr,"%-13d %-14s %-19d \n", line, yytext, token);
       }
    
    }
    fprintf(outptr,"\n --------------------------------------------------------------------------\n");
    fprintf(outptr,"SYM TABLE\n");  // output hash table
    
    for(int i=0; i<20; i++){   // iterating through hash table
        fprintf(outptr,"bucket[%d] = ",i);
        struct symNode* temp = hashmap[i];
        while(temp != NULL){
            if(temp->token == ID_TT()){  // if token is identifier
               fprintf(outptr,"^");
            }
            else{  // if it is integer constant
               fprintf(outptr,"#");
            }
            fprintf(outptr,"%s -> ",(temp->symname).c_str());
            temp = temp -> next;
        
        }
        fprintf(outptr,"NULL\n");
    
    }

}


