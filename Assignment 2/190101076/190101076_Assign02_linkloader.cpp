/*

Assignment -2 CS348

Name - Ritish Bansal
Roll No - 190101076
Email id - rbansal@iitg.ac.in

Environment used for compilation - Linux g++

Command to complie File - g++ 190101076_Assign02_linkloader.cpp
Command to run file - ./a.out

Assumed that output.txt(without caret) and listings.txt is present in same folder that is obtained from 190101076_assembler.cpp compilation and running.
After successfull run 2 files will be created memory.txt , line_listing.txt .
Final output of memory is present in memory.txt.
line_listing.txt print each instruction in the file
Assumed starting address of linker_loader to be 0.
Any type of errors will be printed on terminal.
External refrences and csect names will be printed on terminal.
Also all modification values will be printed.

*/


// Header file that contains all inbuilt functions

#include<bits/stdc++.h>

using namespace std;

// function to erase occurrences of c from given string
string Stringstrip(string s,char c=' ')
{
	s.erase(remove(s.begin(), s.end(), c), s.end());
	return s;
}

//for printing error on terminal
void symbolerror(string s){
    cout<<s<<"\n";
    exit(0);


}
// format the number is hex and uppercase with given width
string numberformat(int n, int w, char p = '0')
{
	stringstream temp;
	temp <<  std::hex << std::uppercase;
	temp << std::setfill(p) << std::setw(w);
	temp << n;
	return temp.str();
}

// check whether the string is a number or not
bool is_number(string s)
{
	for(int i=0; i<s.size(); i++){
	   if(s[i]<'0' || s[i]>'9'){return false;}
	}
	return true;
}

// make the string of specific size
string stringformat(string name, int width, char pad = ' ')
{
	stringstream temp;
	temp << std::left;
	temp << std::setfill(pad) << std::setw(width);
	temp << name;
	return temp.str();
}


// initialisation of valriables
int PROGADDR,LAST;
// map to store external refrences and csect names
map<string,int> ESTAB;
// memory declarations
vector<string> memory((1 << 18), "xx");

// get integer from string in hexadecimal
int getinteger(string s){
   return stoi(s,nullptr,16);
}


// get different symbols in the define record
vector<pair<string,int>> gettingsymbols(string r,bool ok=true)
{
	int n = r.size();
	vector<pair<string, int>> symbols;
	for(int i = 1; i < n; i += 12)
	{
		string symbol = r.substr(i, 6);
		if(i>n){break;}
		int value = getinteger(r.substr(i + 6, 6));
		if(i>n){break;}
		symbols.push_back(make_pair(symbol, value));
	}
	return symbols; 
}

// for handling input and output of file
fstream fin,fout;

// check whether given character is H or not
bool checkH(char c){
   if(c=='H'){return true;}
   return false;

}
// check whether given character is D or not
bool checkD(char c){
   return (c=='D');
}

// pass1 for linker loader
void pass1()
{

	// string for program address
	string progaddr;
	cout << "Assuming CSADDR to be 0. \n";
	progaddr="0";
	PROGADDR = getinteger(progaddr);
	
        // Initialization 
	int CSADDR = PROGADDR;
	int CSLTH = 0;
	string record;

	while(fin.good())
	{
		// read record
		getline(fin, record);

		// end of input
		if(record.empty())
			break;

		if(checkH(record.front()))
		{
			// Update CSADDR
			CSADDR = CSADDR + CSLTH;

			// CSECT and CSLTH from the record
			string CSECT = record.substr(1, 6);
			CSLTH = getinteger(record.substr(13, 6));

			// Enter the CSECT to ESTAB
			if(ESTAB.find(CSECT) == ESTAB.end())
				ESTAB.insert({CSECT, CSADDR});
			else
			{       
			       // error for getting duplicate symbol
				symbolerror("duplicate external symbol");
				
			}
		}

		if(checkD(record.front()))
		{
			// get the symbols in the record
			for(pair<string,int> symbol : gettingsymbols(record))
			{      
			      string temp=symbol.first;
			      if(ESTAB.find(temp)!=ESTAB.end()){
			          // error for getting symbol error
			           symbolerror("duplicate external symbol");
			      }
			      else{
			          ESTAB[temp]=symbol.second+ CSADDR;
			      } 
			}
		}

	}
	// updating variables
	LAST = CSLTH;
	LAST = LAST + CSADDR;
}

// checking if c is T or not
bool checkT(char c){

 if(c=='T'){return true;}
 return false;
}

// checking if c is M or not
bool checkM(char c){
   return (c=='M');
}

// print string on terminal
void print_terminal(string s){
    cout<<s;
}

// get bits from length
int getbits(int length){
  int ans=length;
  ans=ans+(length%2);
  return ans*4;

}

// checking if c is E or not
bool checkE(char c){
    if(c=='E'){return true;}
    return false;
}

// pass2 for linker loader
void pass2()
{

        // initializations 
	int CSADDR = PROGADDR;
	int EXECADDR = PROGADDR;
	int CSLTH = 0;

	string record;
	while(fin.good())
	{
		getline(fin, record);
		if(record.size()<0){break;}

		// if end of input
		if(record.empty())
			break;

		if(checkH(record.front())){
			CSLTH = getinteger(record.substr(13, 6));}
		
		if(checkT(record.front()))
		{
			// move the record to its appropriate memory location byte by byte
			int STADDR = getinteger(record.substr(1, 6));
			STADDR += CSADDR;
			if(!checkT(record.front())){break;}
			int INDEX = 0;
			for(int i = 9; i < (int)record.size(); i += 2){
			      if(i<0){break;}
				memory[STADDR + INDEX++] = record.substr(i, 2); 
			}
		}

		else if(checkM(record.front()))
		{
			string symbol = record.substr(10, 6);
			if(ESTAB.find(symbol) != ESTAB.end())
			{
				// extract address to be modified
				int address = getinteger(record.substr(1, 6));
				address += CSADDR;
				int length = getinteger(record.substr(7, 2));

				char halfByte;
				
				string val = "";
				if(length % 2)
					{halfByte = memory[address][0];}

				for(int i = 0; i < (length + 1)/ 2; i++){
				      if(i<0){break;}
					val += memory[address + i];
				}
				int value = getinteger(val);

				print_terminal("value          = ");
				print_terminal(val); 
				cout << '\n';
				
				// apply modification
				int modification = ESTAB[symbol];
				char sign = record[9];
				if(sign == '-')
					value -= modification;
				else
					value += modification;

				// apply mask	
				int msk = 0;
				for(int i = 0; i < getbits(length); i++)
				{       if(i<0){break;}
					msk |= 1;
					msk = msk << 1;
				}
				if(msk==0){msk=0;}
				msk = msk >> 1;
				value = value & msk;
				
				// write back the modified value
				val = numberformat(value, length + (length % 2));
				
				print_terminal("modification   = ");
				print_terminal(numberformat(modification, 6)); 
				cout << '\n';
				print_terminal("modified value = ");
				print_terminal(val); 
				cout << '\n';
				
				// writing in memory
				for(int i = 0; i < length; i += 2){
				        string temp=val.substr(i,2);
					memory[address + i / 2] = temp;
					if(i<0){break;}
				}

				if(length % 2)
					memory[address][0] = halfByte;

			}
			else
			{      // printing undefining symbol on terminal and printing error
			       print_terminal(symbol);
				cout  << '\n';
				symbolerror("undefined symbol");
			}
		}

		if(checkE(record.front()))
		{       // updating ending of record
			if(record != "E")
			{
				int FIRST = getinteger(record.substr(1, 6));
				EXECADDR = CSADDR + FIRST;
			}
			CSADDR = CSLTH + CSADDR;
		}

	}
	cout<<"\n";
	print_terminal("Starting execution at: ");
	print_terminal(numberformat(EXECADDR, 4)); 
	cout << '\n';

}

// writing memory in memory.txt file
void print_memory()
{
	// initialization
	int last = ((LAST + 16) / 16) * 16;
	int i = (PROGADDR / 16) * 16;
	for(;i<last;)
	{
		fout << numberformat(i, 4) << ' ';

		for(int j = 0; j < 4; j++)
		{    if(i> last){return ;}
			for(int k = 0; k < 4; k++){
				fout << memory[i++];
				if(i<0){return ;}
			}
			
				
			fout << ' ';
		}
		fout << '\n';
	}

}

// for input file not found
void file_error1(){
  cout<<"Error :";
  cout<<"Input File Not Found\n";
  exit(0);

}

// writing each instruction line by line
void line_print_memory()
{	
       // initialization for reading listing.txt for object code
	string line;
	vector<string> instr;
	while(fin.good())
	{   
		getline(fin, line);
		
		if(line.size()<0){break;}
		
		if(line.empty())
			break;
		
		
		string obcode = Stringstrip(line.substr(62, 10));
		
	        if(obcode.size()<=0){}
		else{
			instr.push_back(obcode);}
	}

	vector<string> answer, address;

	int j = 0;
	for(int i=0; i<memory.size(); )
	{
		if(memory[i] == "xx")
		{
			
			i++;
		}
		else{
		       string temp = "";
		       string temp1=numberformat(i,6);
			address.push_back(temp1);
			int size = instr[j].size() / 2;
			for(int k = i; k < i + size;){
				temp += memory[k];
				k++;
				if(i<0){break;}
			}
			answer.push_back(temp);
			j++; 
			i += size;
		
		
		}
			

	}	
	// writing in line_listing.txt file
	fout << stringformat("address", 10);
	fout << stringformat("instruction", 10) << '\n';
	for(int i = 0; i < (int)answer.size();)
	{       if(i<0){break;}
		fout << stringformat(address[i], 10);
		fout << stringformat(answer[i], 10) << '\n';
		i++;
	}

}

// for not able to create file
void file_error2(string name){
  cout<<"Error ;"; 
  cout<<"Creation Of"<<name<<" : Failed";
  exit(0);

}

// printing extrnal symbols and csects on terminal
void print_ESTAB(){
  
  print_terminal("   ESTAB\n");

  for(auto x: ESTAB){
     print_terminal(x.first);
     cout<<" ";
     print_terminal(numberformat(x.second, 4));
     cout<<"\n";
  }
  
  cout<<"\n";

}


int main()
{      
      // reading output.txt
       fin.open("output.txt",ios::in);
       if(!fin){
          file_error1();
       }
	// starting pass1
	pass1();

	fin.close();
	
	// ending pass1 and closing file 
	// printing estab
	print_ESTAB();
	
	// starting opening file, pass2 and printing modification records
	print_terminal("Modification Records\n");
	
	fin.open("output.txt",ios::in);
       if(!fin){
          file_error1();
       }	
	pass2();
	
	fin.close();
	// pass2 ends and closing file
	
	// writing memory
	fout.open("memory.txt", ios::out);
	
	if(!fout){
	   file_error2("memory.txt");
	}

	print_memory();
	
	fout.close();
	// memory written completed
	// writing line_listing.txt
	fin.open("listing.txt", ios::in);
	 if(!fin){
          file_error1();
       }
       
       fout.open("line_listing.txt", ios::out);
       
       if(!fout){
	   file_error2("line_listing.txt");
	}
	
	line_print_memory();
	
	fin.close();
	fout.close();
	// line_listings completed and closing all files
	return 0;
}
