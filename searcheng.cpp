#include "searcheng.h"
#include "fstream"
#include <sstream>

SearchEng::SearchEng(){}
SearchEng::~SearchEng(){
	std::map<std::string, WebPage*>::iterator it;
	for(it = allwebpagesforsearch.begin(); it != allwebpagesforsearch.end(); ++it){
		delete it->second;
	}
}
void SearchEng::add_parse_from_index_file(std::string index_file, PageParser* parser){
	std::ifstream allfilenames(index_file.c_str());
	std::string filename;
	std::map<std::string, WebPage*>::iterator itmap;
	while(true){
		getline(allfilenames,filename);
		if(allfilenames.fail()){
			allfilenames.close();
			break;
		}
		if(filename==""){ //if there's a blank line, ignore and jump into next line
			continue;
		}
		itmap = allwebpagesforsearch.find(filename);
		if(itmap==allwebpagesforsearch.end()){
			WebPage* webpagePointer = new WebPage(filename);  //be sure to deallocate the memory
			allwebpagesforsearch[filename]=webpagePointer;
		}
	}
	//here we're done with creating all the webpages whose names are specified in index_file
	//then we want to iterate through all the webpages we have, and configure them one by one
	for(itmap = allwebpagesforsearch.begin(); itmap != allwebpagesforsearch.end(); ++itmap){
		add_parse_page(itmap->first, parser);
	}
}

void SearchEng::add_parse_page(std::string filename, PageParser* parser){//please pass in the actual MDParser since PageParser is pure virtual
	WebPage* webpagePointer = allwebpagesforsearch[filename];
	myset<std::string>  allwords;
	myset<std::string>  alllinks;
	std::string currentWord;
	parser->parse(filename, allwords, alllinks);
	webpagePointer->all_words(allwords);
  if(allwords.empty()){ //this is a non-existent webpage
    allwebpagesforsearch.erase(filename);
    return;
  }
	myset<std::string> ::iterator itset;
	for(itset = alllinks.begin(); itset != alllinks.end(); ++itset){
		webpagePointer->add_outgoing_link(allwebpagesforsearch[*itset]);//add the outgoing link
		allwebpagesforsearch[*itset]->add_incoming_link(webpagePointer);//add the current link as a incoming link to the pointed link
	}
	//till here configuring allwords, alloutgoinglinks and all incoming links has been done
	//below we are going to map the word to webpages that contain this word.
    std::map<std::string, myset<WebPage*> >::iterator itmap;
    for(itset = allwords.begin(); itset != allwords.end(); ++itset){
		currentWord=*itset;
		for(int i=0;i<(int)currentWord.size();i++){
	        //Decaptilize each letter in firstWord
	       	if (currentWord[i]>='A' && currentWord[i]<='Z'){
	            currentWord[i]=currentWord[i]-('A'-'a');
	        } 
        }
		itmap = wordmap.find(currentWord);
		if(itmap == wordmap.end()){ //if the word is not in the map, i.e the word hasn't been created yet
			myset<WebPage*> webpagesContainingThisWord;
			webpagesContainingThisWord.insert(webpagePointer);
			wordmap[currentWord] = webpagesContainingThisWord;
		}
		else{
			wordmap[currentWord].insert(webpagePointer);
		}
	}
}

bool SearchEng::isLetter(char letter){
   if((letter>='a' && letter<='z')||(letter>='A' && letter<='Z')){
      return true;
   }
   else{
      return false;
   }
}

void SearchEng::parseRequest(std::string myline, bool& wrongFormat, 
									  myset<WebPage*>& webpagesContainingFirstWord, myset<WebPage*> webpagesContainingSecondWord){
	
  std::map<std::string, myset<WebPage*> >::iterator itmap;
      std::istringstream queryline(myline);
        queryline>>firstWord;
        for(int i=0;i<(int)firstWord.size();i++){
        //Decaptilize each letter in firstWord
            if(!isLetter(firstWord[i])){
              wrongFormat=true;
              return;
            }
            if (firstWord[i]>='A' && firstWord[i]<='Z'){
                firstWord[i]=firstWord[i]-('A'-'a');
            }          
        }
        if(queryline.fail()){
          wrongFormat=true;
          return;
        }
        else if(firstWord == "and"){
          queryline>>firstWord;
          if(queryline.fail()){//this is the case where users search for websites containing word AND
          	itmap = wordmap.find("and");
          	if(itmap==wordmap.end()){
          		//do nothing, since word"and" is not in the wordmap
          	}
          	else{webpagesContainingFirstWord = itmap->second;}
          	return; 
          }
          for(int i=0;i<(int)firstWord.size();i++){
            if(!isLetter(firstWord[i])){
              wrongFormat=true;
              return;
            }
            //Decaptilize each letter in firstWord
            if (firstWord[i]>='A' && firstWord[i]<='Z'){
                 firstWord[i]=firstWord[i]-('A'-'a');
            }          
          }
          itmap = wordmap.find(firstWord);
          if(itmap==wordmap.end()){
            //do nothing
          }
          else{
            webpagesContainingFirstWord = itmap->second;
          }
          while(true){
          	queryline>>secondWord;
            if(queryline.fail()){     //this is the case where we reach the end of each query
              break; 
            }
            for(int i=0;i<(int)secondWord.size();i++){
              //Decaptilize each letter in firstWord
              if(!isLetter(secondWord[i])){
                wrongFormat=true;
                break;
               }
              if (secondWord[i]>='A' && secondWord[i]<='Z'){
              	   secondWord[i]=secondWord[i]-('A'-'a');
              } 
            }
            itmap = wordmap.find(secondWord);
            if(itmap==wordmap.end()){
            	//do nothing, since the word is not in the dictionary;
            }
            else{
            	webpagesContainingSecondWord = itmap->second;
            }
            webpagesContainingFirstWord = webpagesContainingFirstWord.set_intersection(webpagesContainingSecondWord);
          }
        }
        else if(firstWord == "or"){
          queryline>>firstWord;
          if(queryline.fail()){//this is the case where users search for websites containing word OR
          	itmap = wordmap.find("or");
          	if(itmap==wordmap.end()){
          		//do nothing, since word"and" is not in the wordmap
          	}
          	else{webpagesContainingFirstWord = itmap->second;}
          	return; 
          }
          for(int i=0;i<(int)firstWord.size();i++){
            //Decaptilize each letter in firstWord
            if(!isLetter(firstWord[i])){
              wrongFormat=true;
              return;
            }
            if (firstWord[i]>='A' && firstWord[i]<='Z'){
                 firstWord[i]=firstWord[i]-('A'-'a');
            }          
          }
          itmap = wordmap.find(firstWord);
          if(itmap==wordmap.end()){
            //do nothing
          }
          else{
            webpagesContainingFirstWord = itmap->second;
          }
          while(true){
          	queryline>>secondWord;
            if(queryline.fail()){     //this is the case where we reach the end of each query
              break; 
            }

            for(int i=0;i<(int)secondWord.size();i++){
              //Decaptilize each letter in firstWord
              if(!isLetter(secondWord[i])){
                wrongFormat=true;
                break;
              }              
              if (secondWord[i]>='A' && secondWord[i]<='Z'){
              	   secondWord[i]=secondWord[i]-('A'-'a');
              } 
            }
            itmap = wordmap.find(secondWord);
            if(itmap==wordmap.end()){
            	//do nothing, since the word is not in the dictionary;
            }
            else{
            	webpagesContainingSecondWord = itmap->second;
            }
            webpagesContainingFirstWord = webpagesContainingFirstWord.set_union(webpagesContainingSecondWord);
          }
        }
        else {
        	queryline>>secondWord;
        	if(!queryline.fail()){		// Wrong Format!
        		wrongFormat=true;
        		return;
        	}
        	else{						//this is right format, a single word query
	            itmap = wordmap.find(firstWord);
	            if(itmap==wordmap.end()){
	              //do nothing
	            }
	            else{
	              webpagesContainingFirstWord = itmap->second;
	            }
        	}
        }
}
