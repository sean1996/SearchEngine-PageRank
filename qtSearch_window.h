#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QRadioButton>
#include <QMessageBox>
#include <QTextEdit>

#include <string>
#include <vector>
#include "myset.h"
#include "webpage.h"
#include "searcheng.h"
#include "MDparser.h"
#include "msort.h"
#include <sstream>
#include <map>

struct node{
	int numberOfValidIncomingLinks;
	int numberOfValidOutgoingLinks;
	myset<WebPage*> ValidIncomingLinks;
	myset<WebPage*> ValidOutgoingLinks;
	double currentPageRankValue;
};

class qtSearchWindow : public QWidget
{
	Q_OBJECT
public:
	qtSearchWindow(std::string index_file, double Restart_Probability, int PageRankSteps);
	~qtSearchWindow();


private slots:
	void displayByPageRank();
	void query();
	void showWebpageContents();
	void displayByFilename();
	void displayByIncomingLinks();
	void displayByOutgoingLinks();
	void detailedDisplayByFileNames();
	void detailedDisplayByIncomingLinks();
	void detailedDisplayByOutgoingLinks();
	void showdetail1();
	void showdetail2();
	void quitMain();
	void quitSub();

private:

	// Layouts
	QHBoxLayout* overallLayout;


	//Query functionality
	QVBoxLayout* queryLayout;
	QLabel* queryLabel;
	QLineEdit* queryInput;
	QRadioButton* singleOption;
	QRadioButton* andOption;
	QRadioButton* orOption;
	QPushButton* queryButtton;
	QPushButton* quitButton;

	// List to display searching results
	QVBoxLayout* displayResultLayout;
	QListWidget* resultNameList;
	QPushButton* showDetail;
	QPushButton* displayByOutgoingLinksButton;
	QPushButton* displayByIncomingLinksButton;
	QPushButton* displayByFilenameslistButton;
	QPushButton* displayByPageRankButton;

	// Displayed webPage Detailed Contents
	QWidget* detailedWindow;
	QVBoxLayout* webpageDetailedDisplayLayout;
	QVBoxLayout* contentDisplayLayout;
	QLabel* webpageDetailedNameLabel;
	QLabel*	contentName;
	QTextEdit* content;


	QLabel* webpageDetailedIncomingLabel;
	QListWidget* webpageDetailedIncomingList;

	QLabel* webpageDetailedOutgoingLabel;
	QListWidget* webpageDetailedOutgoingList;


	QPushButton* showdeatilDetail1;
	QPushButton* showdeatilDetail2;
	QPushButton* quit;
	QPushButton* detailedDisplayByFilenamesButtom;
	QPushButton* detailedDisplayByIncomingLinksButtom;
	QPushButton* detailedDisplayByOutgoingLinkButtom;	
	QHBoxLayout* webPageDetailedContentsDisplayLayout;




	//essential data stuctures that power the search engine 
	PageParser* parser;
	SearchEng searchengine;
	std::string userQuery;
	bool wrongFormat;
  	myset<WebPage*> webpagesContainingFirstWord;
  	myset<WebPage*>  webpagesContainingSecondWord;
  	std::vector<WebPage*> searchResult;
  	std::vector<WebPage*> detailedIncomingList;
  	std::vector<WebPage*> detailedOutgoingList;


  	//essential data that powers the pagerank
  	double Restart_Probability;
  	int PageRankSteps;
  	std::map<WebPage*, node> PageRankGraph;
};

struct webpageNameComp{
  bool operator()(const WebPage* first, const WebPage* second){
    return(first->filename()<=second->filename());
  }
};

struct webpageIncomingLinksComp{
  bool operator()(const WebPage* first, const WebPage* second){
    return(first->incoming_links().size()<= second->incoming_links().size());
  }
};

struct webpageOutgoingLinksComp{
  bool operator()(const WebPage* first, const WebPage* second){
    return(first->outgoing_links().size()<= second->outgoing_links().size());
  }
};
struct webpagePageRankComp{
  bool operator() ( WebPage* first, WebPage* second){
  	return (first->getPageRank() > second->getPageRank());
  }
};

