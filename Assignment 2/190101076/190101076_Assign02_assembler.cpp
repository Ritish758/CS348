/*

Assignment -2 CS348

Name - Ritish Bansal
Roll No - 190101076
Email id - rbansal@iitg.ac.in

Environment used for compilation - Linux g++

Command to complie File - g++ 190101076_Assign02_assembler.cpp
Command to run file - ./a.out

Assumed that input.txt is present inside same directory and acting as input to this program.
After successfull run 4 files will be created intermediate.txt,listing.txt,output.txt,caret.txt.
Final Machine code without caret is inside output.txt.
Final Machine code with caret is inside caret.txt.
output.txt and listing.txt are required for linker_loader

*/

// header for all inbuilt functions
#include<bits/stdc++.h>

using namespace std;

// getting field length
int getlength_field(){
    return 10;
}

// getting operand length
int getlength_operand(){
     return 30;
}

// removing caret from end
string removecaret(string s){
   while(s.back()=='^'){s.pop_back();}
   return s;
}

//formatting number to hex and string
string numberformat(int n, int w, char p = '0',bool ok=true)
{
	stringstream temp;
	temp <<  std::hex << std::uppercase;
	temp << std::setfill(p) << std::setw(w) << n;
	return temp.str();
}

// removing character c from given string
string stringstrip(string s,char c= ' ')
{
	s.erase(remove(s.begin(), s.end(), c), s.end());
	return s;
}

// formatting given string to required format
string stringformat(string name, int width, char pad = ' ',bool ok=true)
{
	stringstream temp;
	temp << std::left << std::setfill(pad);
	temp << std::setw(width) << name;
	return temp.str();
}

// checking whether a given string a number or not
bool is_number(string s)
{
	for(int i=0; i<s.size(); i++){
	   if(s[i]<'0' || s[i]>'9'){return false;}
	}
	return true;
}

// apply bitmasks upto specific bits to a given value
void applybitsmask(int& value, int bts)
{
	if(value < 0)
	{
		int m = 0;
		int i=0;
		while(i<bts)
		{
			m |= 1;
			m = m << 1;
			i++;
		}
		m = m >> 1;
		value = value & m;
	}
}
//<-------------------------------------------------------------->

// map for storing optab 
map<string,pair<int,int>> opmap;

// loading optab 
void load_optab(){
    opmap["LDA"] = make_pair(3,0x00);
    opmap["LDX"] = make_pair(3,0x04);
    opmap["LDL"] = make_pair(3,0x08);
    opmap["LDB"] = make_pair(3,0x68);
    opmap["LDT"] = make_pair(3,0x74);
    opmap["STA"] = make_pair(3,0x0C);
    opmap["STX"] = make_pair(3,0x10);
    opmap["STL"] = make_pair(3,0x14);
    opmap["LDCH"] = make_pair(3,0x50);
    opmap["STCH"] = make_pair(3,0x54);
    opmap["ADD"] = make_pair(3,0x18);
    opmap["SUB"] = make_pair(3,0x1C);
    opmap["MUL"] = make_pair(3,0x20);
    opmap["DIV"] = make_pair(3,0x24);
    opmap["COMP"] = make_pair(3,0x28);
    opmap["COMPR"] = make_pair(2,0xA0);
    opmap["CLEAR"] = make_pair(2,0xB4);
    opmap["J"] = make_pair(3,0x3C);
    opmap["JLT"] = make_pair(3,0x38);
    opmap["JEQ"] = make_pair(3,0x30);
    opmap["JGT"] = make_pair(3,0x34);
    opmap["JSUB"] = make_pair(3,0x48);
    opmap["RSUB"] = make_pair(3,0x4C);
    opmap["TIX"] = make_pair(3,0x2C);
    opmap["TIXR"] = make_pair(2,0xB8);
    opmap["TD"] = make_pair(3,0xE0);
    opmap["RD"] = make_pair(3,0xD8);
    opmap["WD"] = make_pair(3,0xDC);
}

// return opcode with instruction length
pair<int, int> OPTAB(string opcode)
{	
	if(opmap.find(opcode)!=opmap.end()){return opmap[opcode];}
	
	return make_pair(-1, -1);
}

// struct for storing symbols with csects names
struct Symb
{
	string CSECT,name;
	Symb(string CSECT, string name) 
	{
		this -> CSECT = CSECT;
		this -> name = name;
	}
	Symb(){};
	bool operator<(const Symb& s) const
	{
		return make_pair(this -> CSECT, this -> name) < make_pair(s.CSECT, s.name);
	}
};

// for input and output of file handling
fstream fin, fout,fout2,fout3;

// for error in opening file
void file_error1(){
  cout<<"Error :";
  cout<<"Input File Not Found\n";
  exit(0);

}




// struct for address values and its type
struct Value
{
	int value;
	// type 0: absolute, 1: relative
	bool type;
	int length;
	Value(int value = 0, int length = 3, bool type = 1)
	{
		this -> value = value;
		this -> length = length;
		this -> type = type;
	}
};


// for error in creating new file
void file_error2(string name){
  cout<<"Error ;"; 
  cout<<"Creation Of"<<name<<" : Failed";
  exit(0);

}



// Symbol Table SYMTAB[(csect,name)] = address
map<Symb, Value> SYMTAB;

// getting undefined and duplicate symbols
void symbolerror(string s){
    cout<<s<<"\n";
    exit(0);
}


// Literal Pool LITTAB[(csect,name)] = address
map<Symb, Value> LITTAB;



// Some preprocessed constants in SYMTAB
void preprocess(string CSECT,bool ok=true,int val=3)
{
	// other mnemonics
	SYMTAB.insert({Symb(CSECT,"PC"),Value(8, 3, 0)});
	SYMTAB.insert({Symb(CSECT,"SW"),Value(9, 3, 0)});
	SYMTAB.insert({Symb(CSECT,"B" ),Value(3, 3, 0)});
	SYMTAB.insert({Symb(CSECT,"S" ),Value(4, 3, 0)});
	SYMTAB.insert({Symb(CSECT,"T" ),Value(5, 3, 0)});
	SYMTAB.insert({Symb(CSECT,"F" ),Value(6, 3, 0)});
	SYMTAB.insert({Symb(CSECT,"A" ),Value(0, 3, 0)});
	SYMTAB.insert({Symb(CSECT,"X" ),Value(1, 3, 0)});
	SYMTAB.insert({Symb(CSECT,"L" ),Value(2, 3, 0)});
}

// set of external refrences
set<string> EXTREF;

// struct for converting postfix to infix
struct ExpressionEval
{       
       // predence of operators
       int pred(string op)
	{
	       if(op == "*"){return 2;}
	       if(op == "/"){return 2;}
	       if(op == "+"){return 1;}
	       if(op == "-"){return 1;}
		return 0;
	}     
       // data in this struct and its constructor
       string CSECT;
	
	ExpressionEval(string CSECT)
	{
		this -> CSECT = CSECT;
	}
	
	// applying operation on pair of integers
	pair<int, int> apply_operator(pair<int, int> p1, pair<int, int> p2, string op)
	{       
	        int a=0,b=0;    
	        if(op == "/"){
	           a=p1.first/p2.first;
	           b=p1.second/p2.second;
	        }
		if(op == "+") {
		    a= p1.first+p2.first;
		    b=p1.second+p2.second;
		}
		if(op == "-") {
		   a = p1.first - p2.first;
		   b=p1.second - p2.second;
		}
		if(op == "*") {
		   a=p1.first*p2.first;
		   b=p1.second*p2.second;
		}
		
		return make_pair(a, b);
	}
	// check whether char are brackets
	bool check_brackets(char c){
	    if(c==')'){return true;}
	    if(c=='('){return true;}
	    return false;
	}
	// check whether char are operators
	bool check_opera(char c){
	    if(c=='*'){return true;}
	    if(c=='+'){return true;}
	    if(c=='-'){return true;}
	    if(c=='/'){return true;}
	    return false;
	
	}
	// check whether strings are operators
	bool check_opera(string c){
	    if(c=="*"){return true;}
	    if(c=="+"){return true;}
	    if(c=="-"){return true;}
	    if(c=="/"){return true;}
	    return false;
	
	}
        //for dividing the string between values and operators
	vector<string> tokenize(string exp)
	{
		int n = exp.size();
		vector<string> tkns;
		for(int i=0; i<n;)
		{
			if(check_brackets(exp[i]) || check_opera(exp[i]))
			{
				tkns.push_back(exp.substr(i, 1));
				i++;
			}
			else
			{
				int j = i;
				while(j < n && !(check_brackets(exp[j]) || check_opera(exp[j]))){j++;}
				tkns.push_back(exp.substr(i, j - i));
				i = j;
			}
			if(i>n){break;}
		}
		return tkns;
	}
	
	// evaluatinf the expression to get final value 

	pair<int, int> evaluate(string exp)
	{       
	       stack<pair<int, int>> operands;
		stack<int> type;
		stack<string> operators;

		vector<string> tokens = tokenize(exp);
		if(tokens.size()<0){return {0,0};}

		int n = tokens.size();
		int i=0;
		while(i<n)
		{
			
			if(is_number(tokens[i]))
			{       
			        int temp = stoi(tokens[i]);
			        pair<int,int> temp1 = make_pair(temp,temp);
				operands.push(temp1);
			}
			else if(tokens[i] == "("){operators.push("(");}
			else if(tokens[i] == ")")
			{
				while(!operators.empty() && operators.top() != "(")
				{        
				        string op = operators.top();
					operators.pop();
					
					pair<int, int> operand_2 = operands.top();
					operands.pop();

					pair<int, int> operand_1 = operands.top();
					operands.pop();
					
                                       if(op=="("|| op==")"){break;}
					operands.push(apply_operator(operand_1, operand_2, op));
				}
				if(!operators.empty()){operators.pop();}
			}
			else if(check_opera(tokens[i]))
			{
				while(!operators.empty() && pred(operators.top()) >= pred(tokens[i]))
				{       
				       string op = operators.top();
					operators.pop();
				
					pair<int, int> operand_2 = operands.top();
					operands.pop();

					pair<int, int> operand_1 = operands.top();
					operands.pop();

					

					operands.push(apply_operator(operand_1, operand_2, op));
				}

				operators.push(tokens[i]);
			}	
			else
			{       Symb temp(CSECT,tokens[i]);
				if(SYMTAB.find(temp) != SYMTAB.end())
				{
					int value = SYMTAB[temp].value;
					int type = SYMTAB[temp].type;
					operands.push(make_pair(type, value));
				}
				else
				{
					if(EXTREF.find(tokens[i]) == EXTREF.end()){}
					operands.push(make_pair(0, 0));
				}
			}
			i++;	
		}
		while(!operators.empty())
		{       
		        string op = operators.top();
			operators.pop();
			
			pair<int, int> operand_2 = operands.top();
			operands.pop();

			pair<int, int> operand_1 = operands.top();
			operands.pop();

			operands.push(apply_operator(operand_1, operand_2, op));
		}
		return operands.top();
	}
};


// line_instruct class
// for storing instruction of line 
struct line_instruct
{      // intialization of fields and flags 
	string label,opcode,operands;
	int length;
	bool i, n, p, b, e, x,literal;

	// default constructor
	line_instruct(string label = "", string opcode = "", string operands = "", int length = 3, bool i = 1, bool n = 1, bool p = 1, bool b = 0, int e = 0, int literal = 0, int x = 0)
	{       
	        this -> n = n;
		this -> i = i;
		this -> e = e;
		this -> b = b;
		this -> p = p;
		this -> x = x;
		this -> literal = literal;
		this -> length = length;
		this -> operands = operands;
		this -> label = label;
		this -> opcode = opcode;
		
	}
	
	// get label from struct
	string get_label(){
	 return this->label;
	}
	// check if character is ,
	bool checkcar(char c){
	   if(c==','){return true;}
	   return false;
	}
	
	// parse multiple operands to get them and eleiminate ,
	vector<string> get_operands()
	{
		vector<string> mult_operands;
		int i = 0;
		int l =  operands.size();
		for(;i<l;)
		{
			int j = i + 1;
			while(j < l && !checkcar(operands[j])){j++;}
			mult_operands.push_back(operands.substr(i, j - i));
			i = j + 1;
		}
		return mult_operands;
	}
	// get opcode from struct
	string get_opc(){
	   return this-> opcode;
	}

	// work in the flags for opcode
	void format_opcode(int& op,bool ok=true)
	{
	       if(length !=3 && length !=4){return ;}
		if(length == 3 || length == 4)
		{
			op |= i;
			op |= (n << 1);
		}
	}
	// get number of bits according to length
	int getbits(int temp){
	   if(temp==3){return 12;}
	   return 20;
	}

	// work in the flags for operands
	void format_operand(int& operand)
	{ 
		if(length == 3 || length == 4)
		{
			// apply mask for negative operand
			int mask = 0;
			for(int i = 0; i < getbits(length); i++)
			{
				mask |= 1;
				mask = mask << 1;
			}
			mask = mask >> 1;
			operand = operand & mask;


			if(e)
			{     
			        operand |= (e << 20);
				operand |= (p << 21);
				operand |= (b << 22);
				operand |= (x << 23);
			       if(e==0){return ;}
			}
			else
			{    
			       if(e==1){return ;}
			       operand |= (p << 13);
				operand |= (b << 14);
				operand |= (x << 15);
				
			}
		}
	}
	// return true if operands are not empty
	bool with_operands()
	{
	       if(operands == ""){return false;}
		return true;
	}


	// whether an line_instruct is comment or not
	bool with_comment()
	{
	        if(label == "."){return true;}
		return false;
	}

	
	// whether an line_instruct has multiple operands or not
	bool with_mult_operands()
	{
	        if(operands.find(",")==string::npos){return false;}
	        return true;
	}


	// whether an line_instruct has label or not
	bool with_label()
	{
		if(label != ""){return true;}
		return false;
	}
};

// get offset 
int getofset(bool ok){
   if(ok){return 0;}
   return getlength_field();

}
// check for indirect method
bool isindirect(char c){
   return (c=='@');
}
// check for immdediate method
bool isimmediate(char c){
   return (c=='#');
}

// takes a line from source code/intermediate code and tokenize the line
int split_line(string line, line_instruct& instr, bool input = true)
{
	string LOCCTR = "";

	int offset = getofset(input);

	// label
	instr.label = stringstrip(line.substr(offset, getlength_field()));
	
	
	// opcode
	instr.opcode = stringstrip(line.substr(offset + getlength_field() + 1, getlength_field()));

	// operands
	instr.operands = stringstrip(line.substr(offset + 2 * getlength_field() + 2, getlength_operand()));

	// extended format line_instruct
	if(line[offset + getlength_field()] == '+')
	{       
	        if(offset==0){instr.e=1;}
		instr.e = 1;
		instr.p = 0;
		instr.b = 0;
		instr.n = 1;
		instr.i = 1;
	}



	// operand types
	if(line[offset + 2 * getlength_field() + 1] != ' ')
	{
		char x = line[offset + 2 * getlength_field() + 1];
		// immediate
		if(x==' '){x=x;}
		if(isimmediate(x))
		{       
		        instr.n = 0;
			instr.i = 1;
			if(is_number(instr.operands))
			{
				instr.b = 0;
				instr.p = 0;
			}
			else{
			    instr.n = 0;
			    instr.i = 1;
			}
		}
		// indirect
		else if(x == '=')
		{
			instr.literal = true;
		}
		else if(isindirect(x))
		{
			instr.i = 0;
			instr.n = 1;
		}
	}


	// if reading intermediate file
	if(!input)
	{
		if(input==true){return -1;}
		LOCCTR = stringstrip(line.substr(0, getlength_field()));
		
		if(LOCCTR.size()>0){
			return stoi(LOCCTR, nullptr, 16);}
	}
	return -1;
}
// ---------------------------------------------------


// whether to start section or not
bool isstarting(string s){
   if(s=="START"){return true;}
   if(s=="CSECT"){return true;}
   return false;
}

// if it is end of program
bool isending(string s){
   if(s=="END"){return true;}
   return false;

}

// if given string is base or not
bool isbase(string c){
   if(c!="BASE"){return false;}
   return true;
}

// check sevral character like C,X 
bool checkC(char c){
  return (c=='C');

}

bool checkX(char c){
    return (c=='X');
}

// check whether line is external or not
bool isextrnal(string s){
    if(s=="EXTDEF"){return true;}
    if(s=="EXTREF"){return true;}
    return false;

}

// whether type of value is possible
bool checktype(int n){
  if(n==0){return true;}
  if(n==1){return true;}
  if(n==-1){return true;}
  return false;

}

// get first and second element from pair
int getfirst(pair<int,int> p){
     return p.first;
}

int getsecond(pair<int,int> p){
     return p.second;
}

// pass1 
void pass1()
{

	// some useful variables' initialisation
	int LOCCTR = 0, STADDR = 0, LENGTH;
	string CSECT = "";

	// reading input file
	while(fin.good())
	{
		string line;
		getline(fin, line);

		// end of file stop reading
		if(line.empty())
			break;

		// processing line into tokens
		if(line.size()<0){break;}
		line_instruct instr;
		split_line(line, instr);
		
		if(!instr.with_comment())
		{     if(line.size()==0){break;}
			if(isstarting(instr.opcode))
			{
				// enter the previous CSECT in the SYMTAB
				if(!CSECT.empty())
				{
					LENGTH = LOCCTR - STADDR;
					Symb temp(CSECT,CSECT);
					SYMTAB[temp]=Value(STADDR,LENGTH);
					
				}

				// initialize LOCCTR ,STADDR and CSECT
				if(!instr.with_operands())
					STADDR = 0;
				else
					STADDR = stoi(instr.operands, NULL, 16);

				LOCCTR = STADDR;
				CSECT = instr.label;

				// initialize some symbols for the csection
				preprocess(CSECT);

				// write to intermediate file
				fout << stringformat(numberformat(LOCCTR, 4), getlength_field());
				fout << line << '\n';
			}
			else if(instr.opcode == "END" || instr.opcode == "LTORG")
			{
				// write to intermediate file
				fout << stringformat("", getlength_field());
				fout << line << '\n';

				// Update LITTAB
				for(auto& x : LITTAB)
				{    if(x.second.value!=-1){continue;}
					if(x.second.value == -1)
					{
						// write to intermediate file
						fout << stringformat(numberformat(LOCCTR, 4), getlength_field());
						fout << stringformat("*", getlength_field()) << "=";
						fout << stringformat(x.first.name, getlength_field());
						fout << " " << stringformat("", getlength_operand()) << '\n';

						// Update literal address
						x.second.value = LOCCTR;

						// Update LOCCTR according to the literal type
						if(checkC(x.first.name.front()))
							LOCCTR += (x.first.name.length() - 3);
						else if(checkX(x.first.name.front()))
							LOCCTR += ((x.first.name.length() - 3)/2);
						else
							LOCCTR += 3;
					}
				}

				// enter the last CSECT in the SYMTAB
				if(isending(instr.opcode))
				{
					LENGTH = LOCCTR - STADDR;
					Symb temp(CSECT,CSECT);
					SYMTAB[temp]=Value(STADDR,LENGTH);
				}
			}
			else if(isbase(instr.opcode))
			{
				// write to intermediate file
				fout << stringformat("", getlength_field());
				fout << line << '\n';
			}
			else if(instr.opcode == "EQU")
			{
				if(instr.with_label())
				{       Symb ff(CSECT,instr.label);
					if(SYMTAB.find(ff) == SYMTAB.end())
					{
						if(instr.operands == "*")
						{
						        Symb temp(CSECT,instr.label);
							SYMTAB[temp]= Value(LOCCTR);

							// write to intermediate file
							fout << stringformat(numberformat(LOCCTR, 4), getlength_field());
							fout << line << '\n';
						}
						else if(is_number(instr.operands)) // assumption that number is in decimal format only
						{
						       Symb temp(CSECT,instr.label);
							SYMTAB[temp] = Value(stoi(instr.operands), 3, 0);

							// write to intermediate file
							fout << stringformat(numberformat(stoi(instr.operands), 4), getlength_field());
							fout << line << '\n';
						}
						else
						{
							// Evaluate the expression
							ExpressionEval e(CSECT);
							pair<int,int> temp=e.evaluate(instr.operands);
							int value = temp.second;
							int type = temp.first;
							if(!checktype(type))
							{
								symbolerror("Invalid Expression");
							}
							applybitsmask(value, 16);


							// write to intermediate file
							fout << stringformat(numberformat(value, 4), getlength_field());
							fout << line << '\n';
						}
					}
					else
					{
						symbolerror("duplicate symbol");
					}

				}
			}
			else if(isextrnal(instr.opcode))
			{
				// write to intermediate file
				fout << stringformat("", getlength_field());
				fout << line << '\n';
			}
			else
			{
				// write to intermediate file
				fout << stringformat(numberformat(LOCCTR, 4), getlength_field());
				fout << line << '\n';


				// Update LITTAB
				if(instr.literal)
				{       Symb temp(CSECT,instr.operands);
					if(LITTAB.find(temp) == LITTAB.end())	
					{
						LITTAB[temp] = Value(-1);
					}
				}

				// Update SYMTAB
				if(instr.with_label())
				{       Symb temp(CSECT, instr.label);
					if(SYMTAB.find(temp) == SYMTAB.end())
						SYMTAB[temp] = Value(LOCCTR);
					else
					{
						symbolerror("duplicate symbol");
					}
				}
				if(line.size()<0){break;}
				// Update LOCCTR
				if(OPTAB(instr.opcode) != make_pair(-1, -1))
				{      
					pair<int, int> format = OPTAB(instr.opcode);
					if(format.first==-1&&format.second==-1){break;}
					// extended line_instruct format
					if(instr.e)
						format.first++;

					instr.length = getfirst(format);	
					LOCCTR += format.first;
					if(format.first==-1&&format.second==-1){break;}
				}
				else if(instr.opcode == "RESB")
					LOCCTR += stoi(instr.operands);
				else if(instr.opcode == "RESW")
					LOCCTR += 3 * stoi(instr.operands);
				else if(instr.opcode == "WORD")
					LOCCTR += 3;	
				else if(instr.opcode == "BYTE")
				{
					if(checkC(instr.operands.front()))
						LOCCTR += (instr.operands.length() - 3);
					if(checkX(instr.operands.front()))
						LOCCTR += ((instr.operands.length() - 3)/2);
				}	
				else
				{
					symbolerror("Invalid Operation Code\n");
				}	
			}
		}
		else
		{
			// write to intermediate file 
			fout << line << '\n';
		}
	}

}
// check is it is start of program
bool isstart(string s){
   if(s=="START"){return true;}
   return false;
}

// check if length of opcode is 2 or not

bool checkop(int len){
   if(len==2){return true;}
   return false;
}

// check condition 1 for specific difference for litteral table
bool checkcon1(int a,int b){
   if(a-b >= -2048 && a-b <= 2047){return true;}
   return false;
}
// check condition 2 for specific difference for litteral table
bool checkcon2(int a,int b){
   if(a-b>=0 && a-b <= 4095){return true;}
   return false;

}
// check if both text code can be combined or not
bool checksum(int a,int b){
   if(a+b<=60){return true;}
   return false;

}
// check if string is in return state
bool checkstring(string s){
     if(s=="RESW"){return true;}
     if(s=="RESB"){return true;}
     return false;

}

// pass2 
void pass2()
{
	

	// useful variables' initialisation for printing output with caret(data structures with 1 subscript) and without caret(data structures simple)
	int LOCCTR = 0, BASE = 0, PC = 0, START = 0;
	string CSECT = "", PROGNAME = "", text = "",text1="";
	map<string, vector<string>> text_list, modification_list,text_list1, modification_list1;;
	map<string, string> header_list, end_list, define_list, refer_list,header_list1, end_list1, define_list1, refer_list1;;
	vector<string> CSECTS;
	
	

	// reading intermediate file
	while(fin.good())
	{
		string line;
		getline(fin, line);
		
		if(line.size()==0){break;}

		// end of file
		if(line.empty())
			break;

		// processing line into tokens
		line_instruct instr;
		LOCCTR = split_line(line, instr, false); 

		if(!instr.with_comment())
		{   if(line.size()<0){return ;}
			if(isstart(instr.opcode))
			{   
				// write listing for the line_instruct
				fout2 << line << '\n';

				// csection name
				CSECT = instr.label;
				PROGNAME = CSECT;
				CSECTS.push_back(instr.label);


				// starting address for the csection
				int STADDR = 0;
				if(!instr.with_operands())
					STADDR = 0;
				else
					STADDR = stoi(instr.operands, NULL, 16);

			
				// length of the csection
				int LENGTH = 0;
				Symb temp(CSECT,CSECT);
				if(SYMTAB.find(temp) != SYMTAB.end())
					LENGTH = SYMTAB[temp].length;

				// write the output machine code
				stringstream header;
				header << "H" << stringformat(CSECT, 6) << numberformat(STADDR, 6) << numberformat(LENGTH, 6);
				header_list.insert({CSECT, header.str()});
				
				stringstream header1;
				header1 << "H^" << stringformat(CSECT, 6)<<"^" << numberformat(STADDR, 6)<<"^" << numberformat(LENGTH, 6)<<"^";
				header_list1.insert({CSECT, header1.str()});
				
				 
			}
			else if(isending(instr.opcode))
			{
				// write listing for the line_instruct
				fout2 << line << '\n';

				// write the output machine code
				stringstream end; 
				Symb temp(CSECT,instr.operands);
				int first = SYMTAB[temp].value;
				end << "E";
				end_list.insert({CSECT, end.str()});
				
				stringstream end1;
				end1 << "E^";
				end_list1.insert({CSECT, end1.str()});
				

				end << numberformat(first, 6);
				end_list[PROGNAME] =  end.str();
				
				
				end1 << numberformat(first, 6);
				end_list1[PROGNAME] =  end1.str();
				if(line.size()<=0){break;}
			}
			else if(instr.opcode == "CSECT")
			{
				// write listing for the line_instruct
				if(line.size()==0){break;}
				fout2 << line << '\n';

				// write the left over output machine code of the previous CSECT
				if(text.size()>0)
				{
					int lenprogram = (text.size()) / 2;
					stringstream text_record;
					text_record << "T" << numberformat(START, 6) << numberformat(lenprogram, 2) << text;

					text_list[CSECT].push_back(text_record.str());
					
					stringstream text_record1;
					text_record1 << "T^" << numberformat(START, 6)<<"^" << numberformat(lenprogram, 2)<<"^" << text1<<"^";

					text_list1[CSECT].push_back(text_record1.str());
					
					text = "";
					text1 = "";
					START = 0;
				}

				stringstream end;
				end << "E";
				
				stringstream end1;
				end1 << "E^";
				
				end_list.insert({CSECT, end.str()});
				end_list1.insert({CSECT, end1.str()});


				// starting address, length of the new CSECT
				int STADDR = 0;
				CSECT = instr.label;
				

				int LENGTH = 0; 
				Symb temp(CSECT,CSECT);
				if(SYMTAB.find(temp) != SYMTAB.end())
					LENGTH = SYMTAB[temp].length;
					
                               CSECTS.push_back(CSECT);
				// write the output machine code
				stringstream header;
				header << "H" << stringformat(CSECT, 6) << numberformat(STADDR, 6) << numberformat(LENGTH, 6);
				header_list.insert({CSECT, header.str()});
				
				stringstream header1;
				header1 << "H^" << stringformat(CSECT, 6)<<"^" << numberformat(STADDR, 6)<<"^" << numberformat(LENGTH, 6)<<"^";
				header_list1.insert({CSECT, header1.str()});
				
			}
			else if(isextrnal(instr.opcode))
			{
				// write listing for the line_instruct
				fout2 << line << '\n';

				// write the output machine code
				vector<string> operands = instr.get_operands();
				if(instr.opcode == "EXTDEF")
				{
					stringstream def;
					stringstream def1;
					def << "D";
					def1 << "D^";
					for(string operand: operands)
					{
						def << stringformat(operand, 6);
						def1 << stringformat(operand, 6)<<"^";
						int address = 0;
						Symb temp(CSECT,operand);
						if(SYMTAB.find(temp) != SYMTAB.end())
							address = SYMTAB[temp].value;
						def << numberformat(address, 6); 
						def1 << numberformat(address, 6)<<"^";
					}
					define_list.insert({CSECT, def.str()});
					define_list1.insert({CSECT, def1.str()});
				}
				else
				{
					// clear EXTREF list and update according to new CSECT
					EXTREF.clear();
					for(auto x : operands)
						EXTREF.insert(x);

					
					stringstream refer;
					refer << "R";
					
					stringstream refer1;
					refer1 << "R^";
					
					for(string operand : operands){
						refer << stringformat(operand, 6);
						refer1 << stringformat(operand, 6)<<"^";}
					refer_list.insert({CSECT, refer.str()});
					refer_list1.insert({CSECT, refer1.str()});
				}
			}
			else if(isbase(instr.opcode))
			{
				// write listing for the line_instruct
				fout2 << line << '\n';

				// set BASE register location for base relative addressing
				Symb temp(CSECT,instr.operands);
				if(SYMTAB.find(temp) != SYMTAB.end())
					BASE = SYMTAB[temp].value;
			}
			else if(instr.opcode == "LTORG" || instr.opcode == "EQU")
			{
				// write listing for the line_instruct
				fout2 << line << '\n';
			}
			else
			{
				string obcode = "", obcode1 = "";
				

				if(OPTAB(instr.opcode) != make_pair(-1, -1))
				{
					// generate op
					pair<int, int> format = OPTAB(instr.opcode);
					if(format.first==-1&&format.second==-1){break;}

					// extended operation line_instruct
					if(instr.e)
						format.first++;
						
					instr.length = getfirst(format);

					int op = getsecond(format);
					instr.format_opcode(op);
					int operandcode=0;
					// update program counter
					PC = LOCCTR + instr.length;
					
					// generate operandcode
					operandcode = 0;
					if(instr.with_operands())
					{     if(operandcode!=0){operandcode=0;}
						// format operands
						if(instr.with_mult_operands())
						{
							vector<string> m_operands = instr.get_operands();
							if(m_operands.size()<=0){return ;}
							// register-register line_instructs
							if(checkop(instr.length))
							{
								for(string op : m_operands)
								{       Symb temp(CSECT,op);
									if(SYMTAB.find(temp) != SYMTAB.end())
									{
										int x = SYMTAB[temp].value;
										operandcode = operandcode << 4;
										operandcode += x;
									}
									else
									{
										symbolerror("register not found");
									}
								}	
							}
							// line_instructs of type BUFFER,X
							else
							{
								int x = 0;
								Symb temp(CSECT,m_operands.front());
								if(SYMTAB.find(temp) != SYMTAB.end())
								{
									x = SYMTAB[temp].value;
									operandcode += x;
									if(instr.p)
									{
										// decide between PC relative and base relative
										if(checkcon1(operandcode,PC)){
											operandcode -= PC;}
										else if(checkcon2(operandcode,BASE))
										{
											operandcode -= BASE;
											instr.p = 0;
											instr.b = 1;
										}
										else
										{
											symbolerror("can't fit PC or BASE");
										}

									}

									if(instr.e)
									{
										stringstream modification;
										modification << "M" << numberformat(LOCCTR + 1, 6) << "05+" << stringformat(CSECT, 6); 
										modification_list[CSECT].push_back(modification.str());
										
										stringstream modification1;
										modification1 << "M^";
										modification1 << numberformat(LOCCTR + 1, 6)<<"^";
										modification1 << "05^+";
										modification1 << stringformat(CSECT, 6)<<"^"; 
										modification_list1[CSECT].push_back(modification1.str());
										if(!instr.e){break;}
									}

								}
								else
								{
									operandcode += x;
									if(m_operands.size()<=0){break;}
									if(EXTREF.find(m_operands.front()) != EXTREF.end())
									{
										stringstream modification;
										modification << "M" << numberformat(LOCCTR + 1, 6) << "05+" << stringformat(m_operands.front(), 6); 
										modification_list[CSECT].push_back(modification.str());
										
										stringstream modification1;
										modification1 << "M^";
										modification1 << numberformat(LOCCTR + 1, 6)<<"^";
										modification1 << "05^+";
										modification1 << stringformat(m_operands.front(), 6)<<"^"; 
										modification_list1[CSECT].push_back(modification1.str());
										
									}
								}
								instr.x = 1;
							}
						}
						else
						{
							// if line_instruct has literal value then get its address
							if(instr.literal)
							{       Symb temp(CSECT,instr.operands);
								if(LITTAB.find(temp) != LITTAB.end())
								{
									operandcode = LITTAB[temp].value;
									// decide between PC relative and BASE relative
									if(instr.p)
									{
										if(checkcon1(operandcode,PC)){
											operandcode -= PC;}
										else if(checkcon2(operandcode,BASE))
										{
											operandcode -= BASE;
											instr.p = 0;
											instr.b = 1;
										}
										else
										{
											symbolerror("can't fit PC or BASE");
											
										}

									}
								}
								else
								{
									// symbol not found in literal table -> that should not happen 
									symbolerror("my error");
								}
							}	
							// if line_instruct has operand symbol then get its address
							else if(SYMTAB.find(Symb(CSECT, instr.operands)) != SYMTAB.end())
							{       Symb temp(CSECT,instr.operands);
								operandcode = SYMTAB[temp].value;
								if(checkop(instr.length))
								{
									operandcode = operandcode << 4;
								}
								else
								{
									if(instr.p)
									{
										// decide between PC relative and BASE relative
										if(checkcon1(operandcode,PC)){
											operandcode -= PC;}
										else if(checkcon1(operandcode,BASE))
										{
											operandcode -= BASE;
											instr.p = 0;
											instr.b = 1;
										}
										else
										{
											symbolerror("can't fit PC or BASE");
											
										}
									}
									
									if(instr.e)
									{
										stringstream modification;
										modification << "M" << numberformat(LOCCTR + 1, 6) << "05+" << stringformat(CSECT, 6); 
										modification_list[CSECT].push_back(modification.str());
										
										stringstream modification1;
										modification1 << "M^";
										modification1 << numberformat(LOCCTR + 1, 6)<<"^";
										modification1 << "05^+";
										modification1 << stringformat(CSECT, 6)<<"^"; 
										modification_list1[CSECT].push_back(modification1.str());
									}
								}
								
							}
							// if operand is immediate or external reference
							else
							{       if(instr.operands.size()<=0){break;}
								if(EXTREF.find(instr.operands) != EXTREF.end())
								{
									stringstream modification;
									modification << "M" << numberformat(LOCCTR + 1, 6) << "05+" << stringformat(instr.operands, 6); 
									modification_list[CSECT].push_back(modification.str());
									
									stringstream modification1;
									modification1 << "M^";
									modification1 << numberformat(LOCCTR + 1, 6)<<"^";
									modification1 << "05^+";
									modification1 << stringformat(instr.operands, 6)<<"^"; 
									modification_list1[CSECT].push_back(modification1.str());
									
									
								}
								if(instr.i)
								{       if(is_number(instr.operands)==false){}
									if(is_number(instr.operands)){
										operandcode = stoi(instr.operands);
									}
									
								}

								
							}
						}

						instr.format_operand(operandcode);	
					}
					
					// writing listing for the line_instruct
					obcode = numberformat(op, 2);
					obcode += numberformat(operandcode, 2 * (instr.length - 1));
					obcode1 = obcode;
					obcode1.push_back('^');
					fout2 << line;
					fout2 << stringformat(obcode, 10) << '\n';	

				}
				else
				{
					// process constants
					if(instr.label == "*")
					{       if(instr.label!="*"){break;}
						string constant = instr.opcode.substr(2, instr.opcode.length() - 3);
						if(instr.opcode.front() == 'X')
						{
							int x = stoi(constant, nullptr, 16);
							obcode = numberformat(x, constant.length());
							obcode1 = obcode;
							obcode1.push_back('^');
						}
						else if(instr.opcode.front() == 'C')
						{
							for(char ch : constant)
							{
								int x = ch;
								obcode += numberformat(x, 2, false);
								obcode1 += numberformat(x, 2, false);
							}
						}
						else
						{
							int word = stoi(instr.operands);
							obcode = numberformat(word, 6, false);
							obcode1 = obcode;
							obcode1.push_back('^');
						}
					}
                                       // if opcode is "WORD"
					if(instr.opcode == "WORD")
					{
						if(is_number(instr.operands))
						{
							int word = stoi(instr.operands);
							obcode = numberformat(word, 6);
							obcode1 = obcode;
							obcode1.push_back('^');
						}
						else
						{
							ExpressionEval e(CSECT);
							int word = e.evaluate(instr.operands).second; 
							applybitsmask(word, 24);
							obcode = numberformat(word, 6);
							obcode1 = obcode;
							obcode1.push_back('^');

							vector<string> m_operands = e.tokenize(instr.operands);
							if(m_operands.size()<=0){break;}
							for(int i = 0; i < (int)(m_operands.size()); i++)
							{       if(m_operands[i].size()<0){continue;}
								if(EXTREF.find(m_operands[i]) != EXTREF.end())
								{
									stringstream modification;
									modification << "M" << numberformat(LOCCTR, 6) << "06";
									if(i > 0 && m_operands[i-1] == "-")
										modification << "-";
									else
										modification << "+";
									modification << stringformat(m_operands[i], 6); 
									modification_list[CSECT].push_back(modification.str());
									
									stringstream modification1;
									modification1 << "M^";
									modification1 << numberformat(LOCCTR, 6)<<"^";
									modification1 << "06^";
									if(i > 0 && m_operands[i-1] == "-")
										modification1 << "-";
									else
										modification1 << "+";
									modification1 << stringformat(m_operands[i], 6)<<"^"; 
									modification_list1[CSECT].push_back(modification1.str());
									
									
								}
							}
						}
					}
					if(line.size()<0){break;}
                                        // if opcode is "BYTE"
					if(instr.opcode == "BYTE")
					{
						string constant = instr.operands.substr(2, instr.operands.length() - 3);
						if(constant.size()<0){return ;}
						// checking constants and varibales
						if(checkX(instr.operands.front()))
						{
							int x = stoi(constant, nullptr, 16);
							obcode = numberformat(x, constant.length());
							obcode1 = obcode;
							obcode1.push_back('^');
						}

						
						if(instr.operands.front() == 'C')
						{
							for(char ch : constant)
							{
								int x = ch;
								obcode += numberformat(x, 2, false);
								obcode1 += numberformat(x, 2, false);
							}
						}
						if(constant.size()<0){constant="";}
					}

					// writing listing for the line_instruct
					fout2 << line << stringformat(obcode, 10) << '\n';	

				}

				// write the output machine code
				if(text.size() == 0)
					START = LOCCTR;

				if(checksum(text.size(),obcode.size())  && !checkstring(instr.opcode)){
					text += obcode;
					text1 += obcode1;
				}
				else
				{
					if(text.size()>0)
					{
						int LENGTH = (text.length()) / 2;
						stringstream text_record;
						text_record << "T" << numberformat(START, 6) << numberformat(LENGTH, 2) << text;

						text_list[CSECT].push_back(text_record.str());
						
						stringstream text_record1;
						text_record1 << "T^";
						text_record1 << numberformat(START, 6)<<"^";
						text_record1 << numberformat(LENGTH, 2)<<"^";
						text_record1 << text1<<"^";

						text_list1[CSECT].push_back(text_record1.str());
						text = obcode;
						text1 = obcode1;
						START = LOCCTR;
						if(text.size()<0){return ;}
					}
				}
			}
		}
		else
			fout2 << line << '\n';
	}

	if(text.length())
	{       if(text.size()<=0){return ;}
		int LENGTH = (text.length()) / 2;
		stringstream text_record;
		text_record << "T" << numberformat(START, 6) << numberformat(LENGTH, 2) << text;

		text_list[CSECT].push_back(text_record.str());
		
		stringstream text_record1;
		text_record1 << "T^";
		text_record1 << numberformat(START, 6)<<"^";
		text_record1 << numberformat(LENGTH, 2)<<"^";
		text_record1 << text1<<"^";

		text_list1[CSECT].push_back(text_record1.str());

		text = "";
		text1 = "";
		START = 0;
	}

        // final writing to output.txt and caret.txt 
	for(string CSECT: CSECTS)
	{
		fout << header_list[CSECT] << '\n';
		if(define_list[CSECT].size()>0){fout << define_list[CSECT] << '\n';}
		if(refer_list[CSECT].size()>0){fout << refer_list[CSECT] << '\n';}
		for(string text_record: text_list[CSECT]){fout << text_record << '\n';}
		for(string modification_record: modification_list[CSECT]){fout << modification_record << '\n';}
		fout << end_list[CSECT] << '\n';
		
		
		fout3 << removecaret(header_list1[CSECT]) << '\n';
		if(!define_list1[CSECT].empty())
			fout3 << removecaret(define_list1[CSECT]) << '\n';
		if(!refer_list1[CSECT].empty())
			fout3 << removecaret(refer_list1[CSECT]) << '\n';
		for(string text_record: text_list1[CSECT])
			fout3 << removecaret(text_record) << '\n';
		for(string modification_record: modification_list1[CSECT])
			fout3 << removecaret(modification_record) << '\n';
		fout3 << removecaret(end_list1[CSECT]) << '\n';
		
	}
	
}

// Main function
int main()
{
	// loading optab initially
	load_optab();
	// reading and writing specific files
	fin.open("input.txt",ios::in);
	
	
       
       fout.open("intermediate.txt", ios::out);
       
       // starting pass1
	pass1();
	
	fin.close();
	fout.close();
	
	// ending pass1 and closing all files
	// opening all files for reading and writing
	fin.open("intermediate.txt", ios::in);
	
	if(!fin){
          file_error1();
       }
	
	fout.open("output.txt", ios::out);
	fout2.open("listing.txt", ios::out);
	fout3.open("caret.txt",ios::out);
	
	pass2();

	// closing files and pass2 ends
	fin.close();
	fout.close();
	fout2.close();
	fout3.close();

	return 0;
}
