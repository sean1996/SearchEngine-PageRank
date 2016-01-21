# SearchEngine-PageRank

##Introduction

This is a final project that I've done for data structure & object-oriented deisgn class.

This search engine is written in C++ and primary designed for text-searching, featuring:

**-A Qt-GUI interface**

**- A Depth-First-Search cralwer that accepts a few seed webpages to start with, crawl through their outgoing links and discover previously-unknown webpages relating to seed webpages. The cralwer will generate a configuration file containing names of newly-discovered webpages for main search engine to use.**

**- Pagerank algorithm, the key algorithm used by Google Search to  measure the importance of webpages and rank according.**

**- Sorting criteria including pagerank that sort the result using mergeSort.**

##File Description

Below are brief description of each file in this project

**msort.h** - A mergeSort template class that would work for any data type as long as given comparison criteria.

**myset.h** - A template subclass of std::set class that  set_union and set intersection functions

**pageparser.h** - An abstract interface that defines basic functionalities for page parsers of any page format.

**MDParser.h and MDPaser.cpp** - A subclass inheriting pageparser interface. This MDParser is specially designed for parsing txt files into individual words and incoming/outgoing links.

 **webpage.h and webpage.cpp** - Defines a webpage class. Webpage objects are the primary target for searching and displaying and they can be initialized with content from txt files.

**searcheng.h and searcheng.cpp**-  The key class that powers my search engine by supporting essential search functionalities and query operations. Upon launching the engine, this class will create necessary webpage objects, parse their content and relationships, and prepare the Hashmap ready for future search query.

**qtSearch_window.h and qtSearch_window.cpp** - A class that adds graphical interface *(Qt-GUI)* to my search engine. The main application features a master-detail window.

**qtSearch.cpp** - The main application that links every components of this project and get them working appropriately.
