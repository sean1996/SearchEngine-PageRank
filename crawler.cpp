#include "fstream"
#include <string>
#include <sstream>
#include <set>

using namespace std;


void crawler_helper(string& line, ofstream& outputfile, set<string>& visited){

	if(visited.count(line)==0){
		string buffer;
		char letter;
		ifstream parserStream(line.c_str());
		if(parserStream.fail()){
			//check non-existant files
			return;
		}
		//
		outputfile<<line<<endl;
		visited.insert(line);
		//start parsing this page
		while(true){
			if(parserStream.fail()){
				return;	//prase to the end of this page
			}
			letter = (char)parserStream.get();
			if(letter == '['){
				while(letter!=']'&& !parserStream.fail()){
					letter = (char)parserStream.get();
				}
				if(parserStream.fail()){
					return;		//here not sure
				}
				letter = (char)parserStream.get();
				if(letter=='('){
					buffer="";
					letter = (char)parserStream.get();
					while(letter!=')' && !parserStream.fail()){
						buffer+=letter;
						letter = (char)parserStream.get();
					}
					if(parserStream.fail()){
						return;
					}
					else{
						crawler_helper(buffer, outputfile, visited);
					}

				}
			}
		}
		//parserStream.close();
	}
}

int main(int argc, char* argv[])
{
	set<string> visited;
	string configfilepath;
	string eachLine;
	string word;
	string index_file;
	string ouput_file;

	//parse config for Output path and index path to start with
	if(argc == 1){
		configfilepath = "config.txt";
	}
	else{
		configfilepath = argv[0];
	}
	ifstream my_input(configfilepath.c_str());

	while(!my_input.fail()){
		getline(my_input, eachLine);
		std::istringstream my_sstream(eachLine);
		while(!my_sstream.fail()){
			my_sstream>>word;
			if(word == "#"){
				break;
			}
			else if(word == "INDEX_FILE"){
				my_sstream >> word;	//word is "="
				my_sstream >> word;
				index_file = word;
			}
			else if(word == "OUTPUT_FILE"){
				my_sstream >> word;	//word is "="
				my_sstream >> word;
				ouput_file = word;
			}
		}

	}
	my_input.close();

	// till here we have the index and output file path
	ifstream indexfile(index_file.c_str());
	ofstream outputfile(ouput_file.c_str());

	while(true){
		if(indexfile.fail()){
			break;
		}
		indexfile>>eachLine;
		crawler_helper(eachLine, outputfile, visited);
	}
	indexfile.close();
	return 0;
}