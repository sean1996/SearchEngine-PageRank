#ifndef MDPARSER_H
#define MDPARSER_H

#include "pageparser.h"
#include "myset.h"


class MDparser: public PageParser
{
public:
	MDparser();

	virtual void parse(std::string filename,
		     myset<std::string>& allWords,
		     myset<std::string>& allLinks);


	~MDparser();
private:
	bool isLetter(char letter);

	void angleBracket(std::string& tobeStored, bool& readyToStore, std::ifstream& myinput,myset<std::string>& allWords, myset<std::string>& allLinks);
};



#endif
