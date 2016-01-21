#ifndef SEARCHENG_H
#define SEARCHENG_H

#include <map>
#include <vector>
#include <string>
#include "webpage.h"
#include "myset.h"
#include "pageparser.h"

class SearchEng {
 public:
  SearchEng();
  ~SearchEng();
  void add_parse_from_index_file(std::string index_file, 
				 PageParser* parser);
  void add_parse_page(std::string filename, 
		      PageParser* parser);

  /**** Add other desired member functions here *****/
  bool isLetter(char letter);
  void parseRequest(std::string myline, bool& wrongFormat,
									  myset<WebPage*>& webpagesContainingFirstWord, myset<WebPage*> webpagesContainingSecondWord);

 private:
  /**** Add other desired data members here *****/
 	std::map<std::string, WebPage*> allwebpagesforsearch;
 	std::map<std::string, myset<WebPage*> > wordmap;
  std::string firstWord;
  std::string secondWord;
};

#endif
