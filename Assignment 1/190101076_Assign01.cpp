/*
Assignment - 1 CS348

Name - Ritish Bansal
Roll No - 190101076
Email id - rbansal@iitg.ac.in

Environment used for compilation - Linux g++

Command to complie File - g++ 190101076_Assign01.cpp
Command to run file - ./a.out

Assumed that input.txt is present inside same directory and acting as input to this program
After successfull run three files will be created intermediate.txt,listing.txt,output.txt.
Final Machine code is inside output.txt

*/



#include <bits/stdc++.h>

using namespace std;
#define ll long long

map<string,string> optab;
map<string,ll> symtab;

bool flagerror;
bool islabel;
bool iscomment;

string line,label,opcode,operand;

string pro_name;

string recordtext;

string objcode;

ll locctr;

ll start_addr;

ll length;
string stardrr;

fstream fin,fout,fout2;


string format6(string name){
     stringstream t;
     t << std::left << std::setfill(' ') << std::setw(6) << name;
	return t.str();

}

void load_optab(){
     optab["LDA"]="00";
     optab["LDX"]="04";
     optab["LDL"]="08";
     optab["STA"]="0C";
     optab["STX"]="10";
     optab["STL"]="14";
     optab["LDCH"]="50";
     optab["STCH"]="54";
     optab["ADD"]="18";
     optab["SUB"]="1C";
     optab["MUL"]="20";
     optab["DIV"]="24";
     optab["COMP"]="28";
     optab["J"]="3C";
     optab["JLT"]="38";
     optab["JEQ"]="30";
     optab["JGT"]="34";
     optab["JSUB"]="48";
     optab["RSUB"]="4C";
     optab["TIX"]="2C";
     optab["TD"]="E0";
     optab["RD"]="D8";
     optab["WD"]="DC";
}

char hexcode(ll n){
   if(n<=9){
      return char(n+'0');
   }
   n=n-10;
   return char(n+'A');
}


bool checklabel(string name){
     if(optab.find(name)!=optab.end()){return false;}
     if(name=="BYTE"){return false;}
     if(name=="WORD"){return false;}
     if(name=="RESW"){return false;}
     if(name=="RESB"){return false;}
     if(name=="START"){return false;}
     if(name=="END"){return false;}
     return true;
}

bool checkcomment(string name){
   if(name[0]=='.'){return true;}
   return false;
}

string convert_Hex(ll n){
    if(n==0){return "0";}
    string ans="";
    while(n){
      ans.push_back(hexcode(n%16));
      n=n/16;
    }
    reverse(ans.begin(),ans.end());
    return ans;
}


string nameformat(string name,int w){
   stringstream t;
     t << std::left << std::setfill(' ') << std::setw(w) << name;
	return t.str();

}

string formatnumber(string name, int w, int hex)
{
	int numb;
	if(hex==0)
		numb = stoi(name);
	else
		numb = stoi(name, NULL, 16);

	stringstream t;
	t <<  std::hex << std::uppercase << std::setfill('0') << std::setw(w) << numb;
	return t.str();
}

void textwritten(){
   if(recordtext.size()>0){
      int le=(recordtext.size())/2;
      string temp=to_string(le);
      string s1=formatnumber(stardrr,6,1);
      string s2=formatnumber(temp,2,0);
      fout<<"T"<<s1<<s2<<recordtext<<"\n";
   
   }


}


void seprate_line1(){
     vector<string> w;
     string word;
     stringstream ss(line);
     while(ss>>word){
       w.push_back(word);
     }
     //for(auto x: w){cout<<x<<" ";}
     //cout<<"\n";
     if(w.size()==0){return ;}
     iscomment=checkcomment(w[0]);
     if(iscomment==true){
        islabel=false;
        label="";
        opcode="";
        operand="";
        return ;
     }
     islabel=checklabel(w[0]);
     if(islabel==true){
       label=w[0];
       opcode=w[1];
       if(w.size()==3){operand=w[2];}
       else{operand="";}
     }
     else{
       label="";
       opcode=w[0];
       if(w.size()==2){operand=w[1];}
       else{operand="";}
     }
     
}

string format(string name){
       stringstream t;
	t << std::left << std::setfill(' ') << std::setw(5) << name;
	return t.str();
}


bool findlabel(string name){
   if(symtab.find(name)!=symtab.end()){return true;}
   return false;
}
bool findopcode(string name){
   if(optab.find(name)!=optab.end()){return true;}
   return false;

}
bool handlecase(string name){
   if(name.find(",")!=string::npos){
       return true;
   }
   return false;

}

void printlisting(ll type){
    if(type==1){
      fout2<<line<<"\n";
    }
    else{
       fout2<<line;
       string ex;
       ll temp=30-line.size();
       while(temp--){
         ex.push_back(' ');
       }
       fout2<<ex;
       fout2<<nameformat(objcode, 10)<<"\n";
    
    }


}

void handlecase1(){
    string s1=operand.substr(2,operand.size()-3);
    ll i=0;
    while(i<s1.size()){
       char x= s1[i];
       ll temp=x;
       objcode+= formatnumber(to_string(x),2,0);
       i++;
    }            


}


void pass1(){
    while(fin.good()){
        getline(fin,line);
        //cout<<line<<"\n";
        seprate_line1();
        
        if(iscomment==false){
            if(opcode=="START"){
                 start_addr=stoll(operand,nullptr,16);
                 locctr=start_addr;
                 fout<<format(convert_Hex(locctr))<<line<<"\n";
            }
            else if(opcode=="END"){
              length=locctr-start_addr;
              fout<<format(convert_Hex(locctr))<<line;
              return ;
            }
            else{
                 if(islabel){
                     if(findlabel(label)){
                        flagerror=true;
                        cout<<"Duplicate Symbol";
                     }
                     else{
                        symtab[label]=locctr;
                     
                     }
                 
                 }
                 fout<<format(convert_Hex(locctr))<<line<<"\n";
                 if(findopcode(opcode)){
                    locctr+=3;
                 }
                 else if(opcode=="WORD"){locctr+=3;}
                 else if(opcode=="RESB"){locctr+=stoll(operand);}
                 else if(opcode=="RESW"){locctr+=(3*stoll(operand));}
                 else if(opcode=="BYTE"){
                     if(operand[0]=='C'){
                        locctr+=(operand.size()-3);
                     }
                     else if(operand[0]=='X'){
                        locctr+=((operand.size()-3)/2);
                     }
                 }
                 else{
                   flagerror=true;
                   cout<<"Undefined Symbol\n";
                 }
            
            }
        
        }
        else{
           fout<<line<<"\n";
        }
    }
    
}

void seperate_line2(string &loctr){
    vector<string> w;
    string word;
    stringstream ss(line);
    while(ss>>word){
      w.push_back(word);
    }
    if(w.size()==0){return ;}
    //for(auto x: w){cout<<x<<" ";}
   // cout<<"\n";
    iscomment=checkcomment(w[0]);
    if(iscomment==true){
        islabel=false;
        label="";
        opcode="";
        operand="";
        loctr="";
        return ;
    }
    loctr=w[0];
    islabel=checklabel(w[1]);
    if(islabel){
       label=w[1];
       opcode=w[2];
       if(w.size()==4){operand=w[3];}
       else{operand="";}
    }
    else{
     label="";
     opcode=w[1];
     if(w.size()==3){operand=w[2];}
     else{operand="";}
    }

}



void pass2(){
     while(fin.good()){
           getline(fin,line);
           string loctr;
           seperate_line2(loctr);
           
           if(iscomment==false){
               if(opcode=="START"){
                   string s1=nameformat(label,6);
                   string s2=formatnumber(operand,6,1);
                   string s3=to_string(length);
                   s3=formatnumber(s3,6,0);
                   stringstream h;
                   h<<"H";
                   h<<s1;
                   h<<s2;
                   h<<s3;
                   fout<<h.str()<<"\n";
                   printlisting(1);
               } 
               
               else if(opcode=="END"){
                    if(recordtext.size()>0){
                        ll le=recordtext.size()/2;
                        string s1=formatnumber(stardrr,6,1);
                        string s2=to_string(le);
                        s2=formatnumber(s2,2,0);
                        fout<< "T" << s1<< s2<< recordtext<<"\n";
                    
                    }
                    stringstream e;
                    ll fir=symtab[operand];
                    e<<"E";
                    e<<formatnumber(to_string(fir),6,0);
                    fout<<e.str()<<"\n";
                    fout2<<line;
                    return ;
               } 
               else{
                  objcode="";
                  if(findopcode(opcode)){
                       objcode=optab[opcode];
                       if(operand!=""){
                          ll oper=0;
                          if(handlecase(operand)){
                                operand.resize(operand.find(","));
                                oper+=32768;
                          }
                          if(findlabel(operand)){
                                oper=oper+symtab[operand];
                                objcode+= formatnumber(to_string(oper),4,0);
                          }
                          else{
                              objcode+= formatnumber("0",4,0);
                              flagerror=true;
                              cout<<"undefined symbol";
                          }
                       
                       
                       }
                       else{
                          objcode+= formatnumber("0",4,0);
                       }
                       printlisting(2);
                  
                  
                  }
                  
                  else{
                       if(opcode=="BYTE"){
                             if(operand[0]=='C'){
                               handlecase1();  
                             }
                             
                             else{
                                string s1=operand.substr(2,operand.size()-3);
                                objcode+= formatnumber(s1,s1.length(),1);
                             }
                      
                       }
                       
                       else if(opcode=="WORD"){
                          objcode+= formatnumber(operand,6,0);
                       
                       }
                       printlisting(2);
                  
                  }
                  
                  if(recordtext.size()==0){
                     stardrr= loctr;
                  }
                  if(recordtext.size() + objcode.size()<=60 && opcode!="RESW"&& opcode!="RESB"){
                       recordtext+=objcode;
                  
                  }
                  else{
                      if(recordtext.size()>0){
                           ll le=recordtext.size()/2;
                           fout<<"T"<<formatnumber(stardrr,6,1)<<formatnumber(to_string(le),2,0)<<recordtext<<"\n";
                           recordtext=objcode;
                           stardrr=loctr;
                      
                      
                      }
                  }
               }
           
           }
           else{fout2<<line<<"\n";}
     }
}


void file_error1(){
  cout<<"Error :";
  cout<<"Input File Not Found\n";
  exit(0);

}

void file_error2(string name){
  cout<<"Error ;"; 
  cout<<"Creation Of"<<name<<" : Failed";
  exit(0);

}



int main(){
   fin.open("input.txt",ios::in);
   if(!fin){
       file_error1();
   }
   fout.open("intermediate.txt",ios::out);
   if(!fout){
     string name="intermediate.txt";
     file_error2(name);
   }

   locctr=0;
   start_addr=0;
   flagerror=false;
   load_optab();
   pass1();
   fin.close();
   fout.close();
   
   fin.open("intermediate.txt",ios::in);
   if(!fin){
     file_error1();
   }
   fout.open("output.txt",ios::out);
   if(!fout){
     string name="output.txt";
     file_error2(name);
   }
   fout2.open("listing.txt",ios::out);
   if(!fout2){
     string name="listing.txt";
     file_error2(name);
   }
   pass2();
   fin.close();
   fout.close();
   fout2.close();
   
   
   




}
