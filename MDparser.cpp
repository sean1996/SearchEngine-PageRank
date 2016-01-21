#include "MDparser.h"
#include <fstream>
#include <string>
#include <fstream>


using namespace std;

MDparser::MDparser(){}
//a private helper function that determies if a char charcater is a letter
bool MDparser::isLetter(char letter){
   if((letter>='a' && letter<='z')||(letter>='A' && letter<='Z')){
      return true;
   }
   else{
      return false;
   }
}
//the case if encounter a angle bracket "["
void MDparser::angleBracket(std::string& tobeStored, bool& readyToStore, std::ifstream& myinput, myset<std::string>& allWords, myset<std::string>& allLinks){
	string buffer("");

    char letter=(char)myinput.get();
    while(letter!=']'&& !myinput.fail()){
      buffer=buffer+letter;
      letter=(char)myinput.get();
   }
   buffer+="]";
   //parsing,change bool varibale to false
   for (int i=0;i<(int)buffer.size();i++){
   		letter=buffer[i];
   		if(isLetter(letter)){
   			readyToStore=true;
   			tobeStored+=letter;
   		}
   		else if(!isLetter(letter)&&readyToStore){
   			allWords.insert(tobeStored);
   			tobeStored="";
   			readyToStore=false;
   		}
   		else if(!isLetter(letter)&&!readyToStore){
   			//donothing
   		}
   }

   if(!myinput.fail()){
   	   letter=(char)myinput.get(); //get the next letter after']'
		if(letter=='('){
			buffer="";
			letter=(char)myinput.get();
			while(letter!=')'){
				buffer+=letter;
				letter=(char)myinput.get();
			}
			ifstream my_input(buffer.c_str());
			if(!my_input.fail()){
				allLinks.insert(buffer);				
			}
			my_input.close();
			buffer="";
			readyToStore=false;
		}
		else{
			if(isLetter(letter)){
				readyToStore=true;
				tobeStored+=letter;
			}
			else{
				readyToStore=false;
			}
		}
   }
}

void MDparser::parse(std::string filename, myset<std::string>& allWords, myset<std::string>& allLinks){
	ifstream myinput(filename.c_str());
	string tobeStored("");
	char nextCharacter;
	bool readyToStore=false;
	while(true){
		nextCharacter=(char)myinput.get();
		if(myinput.fail()){
			if(readyToStore){
				allWords.insert(tobeStored);
			}
			myinput.close();
			return;
		}
		//case when the next character is open parathesis
		if(nextCharacter=='['){
			if(readyToStore){
				allWords.insert(tobeStored);
				readyToStore=false;
				tobeStored="";
			}
			angleBracket(tobeStored,readyToStore, myinput, allWords, allLinks);

		}
		else if(isLetter(nextCharacter)){
			readyToStore=true;
			tobeStored+=nextCharacter;

		}
		else if(readyToStore&&!isLetter(nextCharacter)){
			allWords.insert(tobeStored);
			readyToStore=false;
			tobeStored="";
		}

	}
}

MDparser::~MDparser(){}
