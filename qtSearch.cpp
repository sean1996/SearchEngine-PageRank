#include "qtSearch_window.h"
#include <QApplication>
#include <string>
#include <fstream>
#include <cstdlib>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	std::string eachLine;
	std::string word;
	std::string src;
	std::string index_file;
	double Restart_Probablity = 0;
	int PageRankSteps = 0;
	if(argc == 1){
		src = "config.txt";
	}
	else{
		src = argv[1];
	}
	ifstream my_input(src.c_str());

	while(!my_input.fail()){
		getline(my_input, eachLine);
		std::istringstream my_sstream(eachLine);
		while(!my_sstream.fail()){
			my_sstream>>word;
			if(word == "#"){
				break;
			}
			else if(word == "INDEX_FILE"){
				my_sstream >> word;
				my_sstream >> word;
				index_file = word;
				break;
			}
			else if(word == "RESTART_PROBABILITY"){
				my_sstream >> word;
				my_sstream >> word;
				Restart_Probablity = atof(word.c_str());				
			}
			else if(word == "STEP_NUMBER"){
				my_sstream >> word;
				my_sstream >> word;
				PageRankSteps = atoi(word.c_str());				
			}			
		}

	}

	my_input.close();
	
	qtSearchWindow QTSearchWindow(index_file, Restart_Probablity, PageRankSteps);

	QTSearchWindow.show();

	return app.exec();
}