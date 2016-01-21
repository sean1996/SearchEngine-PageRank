#include "webpage.h"
#include <fstream>
#include <cstring>

  WebPage::WebPage(){}

  WebPage::WebPage(std::string filename){
  	URL=filename;
  }

  WebPage::~WebPage(){}

  /**
   * Sets the filename/URL of this webpage
   */
  void WebPage::filename(std::string fname){
  	URL=fname;
  }

  /**
   * Returns the filename/URL of this webpage
   */
  std::string WebPage::filename() const{
  	return URL;
  }

  /**
   * Updates the set containing all unique words in the text
   *  with the contents of the input parameter words
   */
  void WebPage::all_words(const myset<std::string>& words){
  	allwords=words;
  }

  /**
   * Returns all the unique, tokenized words in this webpage 
   */
  myset<std::string> WebPage::all_words() const{
  	return allwords;
  }

  /**
   * Adds a webpage that links to this page
   */
  void WebPage::add_incoming_link(WebPage* wp){
  	incominglinks.insert(wp);
  }

  /**
   * Returns all webpages that link to this page
   */
  myset<WebPage*> WebPage::incoming_links() const{
  	return incominglinks;
  }

  /**
   * Adds a webpage that this page links to
   */
  void WebPage::add_outgoing_link(WebPage* wp){
  	outgoinglinks.insert(wp);
  }

  /**
   * Returns all webpages this page links to
   */
  myset<WebPage*> WebPage::outgoing_links() const{
  	return outgoinglinks;
  }

  void WebPage::setPageRank(double val){
    pageRankValue = val;
  }

  double WebPage::getPageRank(){
    return pageRankValue;
  }

  /**
   * Displays the webpage text to the screen 
   */
 std::ostream & operator<< (std::ostream & os, const WebPage & page){
  	char nextCharacter;
  	std::string buffer("");
  	std::ifstream myinput(page.URL.c_str());
  	while(true){
	  	nextCharacter=(char)myinput.get();
		if(myinput.fail()){
			myinput.close();
			return os;
		}
		if(nextCharacter=='['){
			while(nextCharacter!=']' && !myinput.fail()){
					os<<nextCharacter;
					nextCharacter=(char)myinput.get();
			}
			if(!myinput.fail()){
				os<<nextCharacter; //print out ']'
				nextCharacter=(char)myinput.get();//get the next character after ']'
				if(nextCharacter=='('){
					while(nextCharacter!=')'&& !myinput.fail()){
						buffer+=nextCharacter;	//if there's no ')' matching, then print our the string after '(' as usual.
						nextCharacter=(char)myinput.get();
					}
					if(myinput.fail()){
						for(int i=0;i<(int)buffer.size();i++){
							os<<buffer[i];
						}
					}
				}
		    }

	    }
	    else {
	    	os<<nextCharacter;
	    }
  	}
 }