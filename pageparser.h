#ifndef ELEM_PARSER_H
#define ELEM_PARSER_H

#include <string>
#include "myset.h"

/**
 * Base class for parsers (for some specific
 *  format such as MD or HTML)
 */

class PageParser {
 public:
  virtual ~PageParser() { }

  /** 
   * Parses a file and returns all unique words according
   *  to some parser rules and all hyperlinks based 
   *  on the format the parser is designed to implement
   */
  virtual void parse(std::string filename,
		     myset<std::string>& allWords,
		     myset<std::string>& allLinks) = 0;
};

#endif
