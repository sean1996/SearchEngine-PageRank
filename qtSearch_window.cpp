#include "qtSearch_window.h"
#include <iostream>

qtSearchWindow::qtSearchWindow(std::string index_file, double restart_Probability, int pageRankSteps){
	Restart_Probability = restart_Probability;
	PageRankSteps = pageRankSteps;

	PageParser* parser = new MDparser();
	searchengine.add_parse_from_index_file(index_file, parser);
	wrongFormat = false;

	//title
	setWindowTitle("qtSearch");

	// Overall layout
	overallLayout = new QHBoxLayout();

	// //form to show the seach result
	//Layout
	displayResultLayout = new QVBoxLayout();
	overallLayout->addLayout(displayResultLayout);

	//form
	resultNameList = new QListWidget();
	displayResultLayout->addWidget(resultNameList);


	//display options
	showDetail = new QPushButton("Show Detailed Content!");
	connect(showDetail, SIGNAL(clicked()), this, SLOT(showWebpageContents()));
	displayByFilenameslistButton = new QPushButton("Sort by Filenames");
	connect(displayByFilenameslistButton, SIGNAL(clicked()), this, SLOT(displayByFilename()));
	displayByOutgoingLinksButton = new QPushButton("Sort by Outgoing Links");
	connect(displayByOutgoingLinksButton, SIGNAL(clicked()), this, SLOT(displayByOutgoingLinks()));
	displayByIncomingLinksButton = new QPushButton("Sort by Incoming Links");
	connect(displayByIncomingLinksButton, SIGNAL(clicked()), this, SLOT(displayByIncomingLinks()));
	displayByPageRankButton = new QPushButton("Sort by PageRank");
	connect(displayByPageRankButton, SIGNAL(clicked()), this, SLOT(displayByPageRank()));
	displayResultLayout->addWidget(showDetail);
	displayResultLayout->addWidget(displayByFilenameslistButton);
	displayResultLayout->addWidget(displayByOutgoingLinksButton);
	displayResultLayout->addWidget(displayByIncomingLinksButton);
	displayResultLayout->addWidget(displayByPageRankButton);

	// //Search functionality section
	//Layout
	queryLayout = new QVBoxLayout();
	overallLayout->addLayout(queryLayout);

	//Query Label
	queryLabel = new QLabel("What do you want to search?");
	queryLayout->addWidget(queryLabel);

	//Query words input
	queryInput = new QLineEdit();
	queryLayout->addWidget(queryInput);
	connect(queryInput, SIGNAL(returnPressed()), this, SLOT(query()));

	//search options
	singleOption = new QRadioButton("Let's do single word search!", this);
	andOption = new QRadioButton("Let's do <AND> search!", this);
	orOption = new QRadioButton("Let's do <OR> search!", this);
	singleOption->setChecked(true);
	queryLayout->addWidget(singleOption);
	queryLayout->addWidget(andOption);
	queryLayout->addWidget(orOption);

	//search button
	queryButtton = new QPushButton("Magic Search!");
	connect(queryButtton, SIGNAL(clicked()), this, SLOT(query()));
	quitButton = new QPushButton("Quit Application");
	queryLayout->addWidget(queryButtton);
	queryLayout->addWidget(quitButton);
	connect(quitButton, SIGNAL(clicked()), this, SLOT(quitMain()));

	//this is the section for subwindow

	detailedWindow = new QWidget();
	webpageDetailedDisplayLayout = new QVBoxLayout();
	contentDisplayLayout = new QVBoxLayout();
	webpageDetailedDisplayLayout->addLayout(contentDisplayLayout);
	webpageDetailedNameLabel = new QLabel();

	contentName = new QLabel("Content:");
	content = new QTextEdit();
	content->setReadOnly(true);
	contentDisplayLayout->addWidget(webpageDetailedNameLabel);
	contentDisplayLayout->addWidget(contentName);
	contentDisplayLayout->addWidget(content);
	webpageDetailedIncomingLabel = new QLabel("IncomingLinks of this page:");
	webpageDetailedIncomingList = new QListWidget();
	showdeatilDetail1 = new QPushButton("Show detail of this link!");
	connect(showdeatilDetail1, SIGNAL(clicked()), this, SLOT(showdetail1()));


	webpageDetailedOutgoingLabel= new QLabel("OutgoingLinks of this page:");
	webpageDetailedOutgoingList = new QListWidget();
	showdeatilDetail2 = new QPushButton("Show detail of this link!");
	connect(showdeatilDetail2, SIGNAL(clicked()), this, SLOT(showdetail2()));


	webpageDetailedDisplayLayout->addWidget(webpageDetailedIncomingLabel);
	webpageDetailedDisplayLayout->addWidget(webpageDetailedIncomingList);
	webpageDetailedDisplayLayout->addWidget(showdeatilDetail1);
	webpageDetailedDisplayLayout->addWidget(webpageDetailedOutgoingLabel);
	webpageDetailedDisplayLayout->addWidget(webpageDetailedOutgoingList);
	webpageDetailedDisplayLayout->addWidget(showdeatilDetail2);
	detailedDisplayByFilenamesButtom = new QPushButton("Sort by Filenames");

	detailedDisplayByIncomingLinksButtom = new QPushButton("Sort by Incominglinks");
	detailedDisplayByOutgoingLinkButtom = new QPushButton("Sort by Outgoinginglinks");
	quit = new QPushButton("Close Window");

	webpageDetailedDisplayLayout->addWidget(detailedDisplayByFilenamesButtom);
	webpageDetailedDisplayLayout->addWidget(detailedDisplayByIncomingLinksButtom);
	webpageDetailedDisplayLayout->addWidget(detailedDisplayByOutgoingLinkButtom);
	webpageDetailedDisplayLayout->addWidget(quit);
	connect(quit, SIGNAL(clicked()), this, SLOT(quitSub()));
	connect(detailedDisplayByFilenamesButtom, SIGNAL(clicked()), this, SLOT(detailedDisplayByFileNames()));
	connect(detailedDisplayByIncomingLinksButtom, SIGNAL(clicked()), this, SLOT(detailedDisplayByIncomingLinks()));
	connect(detailedDisplayByOutgoingLinkButtom, SIGNAL(clicked()), this, SLOT(detailedDisplayByOutgoingLinks()));
	

	detailedWindow->setLayout(webpageDetailedDisplayLayout);

	//Set overall layout
	setLayout(overallLayout);	

}

qtSearchWindow::~qtSearchWindow(){

}

void qtSearchWindow::showdetail1(){
	if(webpageDetailedIncomingList->count()==0){
		return;
	}
	int currentRow = webpageDetailedIncomingList->currentRow();
	webpageDetailedIncomingList->clear();
	webpageDetailedOutgoingList->clear();
	detailedIncomingList.clear();
	detailedOutgoingList.clear();
	QString filename= QString::fromStdString(detailedIncomingList[currentRow]->filename());
	webpageDetailedNameLabel->setText(filename);
 	ostringstream mystring;
	mystring << *detailedIncomingList[currentRow];
	QString qstr = QString::fromStdString(mystring.str());
	content->setPlainText(qstr);
	//configuring the incoming listWidget and outgoing listWidget
	myset<WebPage*>  allincomingLinks = detailedIncomingList[currentRow]->incoming_links();
	myset<WebPage*>  alloutygoingLinks = detailedIncomingList[currentRow]->outgoing_links();
	webpageNameComp nameComp;
	for(set<WebPage*>::iterator it = allincomingLinks.begin();
  		it != allincomingLinks.end();
  		++it){
  		detailedIncomingList.push_back(*it);
	}
	for(set<WebPage*>::iterator it = alloutygoingLinks.begin();
  		it != alloutygoingLinks.end();
  		++it){
  		detailedOutgoingList.push_back(*it);
	}
	mergeSort(detailedIncomingList,nameComp);
	mergeSort(detailedOutgoingList,nameComp);

	for(int i=0; i<(int)detailedIncomingList.size();i++){
		qstr = QString::fromStdString(detailedIncomingList[i]->filename());
		qstr = qstr + "	IncomingLinks:" + QString::number(detailedIncomingList[i]->incoming_links().size());
		qstr = qstr + "	OutgoingLinks:" + QString::number(detailedIncomingList[i]->outgoing_links().size());
	webpageDetailedIncomingList->addItem(qstr);
	}
	for(int i=0; i<(int)detailedOutgoingList.size();i++){
		qstr = QString::fromStdString(detailedOutgoingList[i]->filename());
		qstr = qstr + "	IncomingLinks:" + QString::number(detailedOutgoingList[i]->incoming_links().size());
		qstr = qstr + "	OutgoingLinks:" + QString::number(detailedOutgoingList[i]->outgoing_links().size());
	webpageDetailedOutgoingList->addItem(qstr);
	}

	webpageDetailedIncomingList->setCurrentRow(0); //prevent segfault
	webpageDetailedOutgoingList->setCurrentRow(0);	//prevent segfault
}


void qtSearchWindow::showdetail2(){
	if(webpageDetailedOutgoingList->count()==0){
		return;
	}
	int currentRow = webpageDetailedOutgoingList->currentRow();

	webpageDetailedIncomingList->clear();
	webpageDetailedOutgoingList->clear();
	detailedIncomingList.clear();
	detailedOutgoingList.clear();
	QString filename= QString::fromStdString(detailedOutgoingList[currentRow]->filename());
	webpageDetailedNameLabel->setText(filename);
 	ostringstream mystring;
	mystring << *detailedOutgoingList[currentRow];
	QString qstr = QString::fromStdString(mystring.str());
	content->setPlainText(qstr);
	//configuring the incoming listWidget and outgoing listWidget
	myset<WebPage*>  allincomingLinks = detailedOutgoingList[currentRow]->incoming_links();
	myset<WebPage*>  alloutygoingLinks = detailedOutgoingList[currentRow]->outgoing_links();
	webpageNameComp nameComp;
	for(set<WebPage*>::iterator it = allincomingLinks.begin();
  		it != allincomingLinks.end();
  		++it){
  		detailedIncomingList.push_back(*it);
	}
	for(set<WebPage*>::iterator it = alloutygoingLinks.begin();
  		it != alloutygoingLinks.end();
  		++it){
  		detailedOutgoingList.push_back(*it);
	}
	mergeSort(detailedIncomingList,nameComp);
	mergeSort(detailedOutgoingList,nameComp);

	for(int i=0; i<(int)detailedIncomingList.size();i++){
		qstr = QString::fromStdString(detailedIncomingList[i]->filename());
		qstr = qstr + "	IncomingLinks:" + QString::number(detailedIncomingList[i]->incoming_links().size());
		qstr = qstr + "	OutgoingLinks:" + QString::number(detailedIncomingList[i]->outgoing_links().size());
	webpageDetailedIncomingList->addItem(qstr);
	}
	for(int i=0; i<(int)detailedOutgoingList.size();i++){
		qstr = QString::fromStdString(detailedOutgoingList[i]->filename());
		qstr = qstr + "	IncomingLinks:" + QString::number(detailedOutgoingList[i]->incoming_links().size());
		qstr = qstr + "	OutgoingLinks:" + QString::number(detailedOutgoingList[i]->outgoing_links().size());
	webpageDetailedOutgoingList->addItem(qstr);
	}
	webpageDetailedIncomingList->setCurrentRow(0); //prevent segfault
	webpageDetailedOutgoingList->setCurrentRow(0);	//prevent segfault
}

void qtSearchWindow::showWebpageContents(){
	if(resultNameList->count()==0){
		return;
	}
	int currentRow = resultNameList->currentRow();
	webpageDetailedIncomingList->clear();
	webpageDetailedOutgoingList->clear();
	detailedIncomingList.clear();
	detailedOutgoingList.clear();
	QString filename= QString::fromStdString(searchResult[currentRow]->filename());
	webpageDetailedNameLabel->setText(filename);
 	ostringstream mystring;
	mystring << *searchResult[currentRow];
	QString qstr = QString::fromStdString(mystring.str());
	content->setPlainText(qstr);
	//configuring the incoming listWidget and outgoing listWidget
	myset<WebPage*>  allincomingLinks = searchResult[currentRow]->incoming_links();
	myset<WebPage*>  alloutygoingLinks = searchResult[currentRow]->outgoing_links();
	webpageNameComp nameComp;
	for(set<WebPage*>::iterator it = allincomingLinks.begin();
  		it != allincomingLinks.end();
  		++it){
  		detailedIncomingList.push_back(*it);
	}
	for(set<WebPage*>::iterator it = alloutygoingLinks.begin();
  		it != alloutygoingLinks.end();
  		++it){
  		detailedOutgoingList.push_back(*it);
	}
	mergeSort(detailedIncomingList,nameComp);
	mergeSort(detailedOutgoingList,nameComp);

	for(int i=0; i<(int)detailedIncomingList.size();i++){
		qstr = QString::fromStdString(detailedIncomingList[i]->filename());
		qstr = qstr + "	IncomingLinks:" + QString::number(detailedIncomingList[i]->incoming_links().size());
		qstr = qstr + "	OutgoingLinks:" + QString::number(detailedIncomingList[i]->outgoing_links().size());
	webpageDetailedIncomingList->addItem(qstr);
	}
	for(int i=0; i<(int)detailedOutgoingList.size();i++){
		qstr = QString::fromStdString(detailedOutgoingList[i]->filename());
		qstr = qstr + "	IncomingLinks:" + QString::number(detailedOutgoingList[i]->incoming_links().size());
		qstr = qstr + "	OutgoingLinks:" + QString::number(detailedOutgoingList[i]->outgoing_links().size());
	webpageDetailedOutgoingList->addItem(qstr);
	}

	webpageDetailedIncomingList->setCurrentRow(0); //prevent segfault
	webpageDetailedOutgoingList->setCurrentRow(0);	//prevent segfault
	detailedWindow->show();
}



void qtSearchWindow::query(){
	//clean up the data from last search
	resultNameList->clear();
	searchResult.clear();
	PageRankGraph.clear();
	//new search
	QString qstr; 
	webpagePageRankComp pageRankComp;
	if(queryInput->text().isEmpty()){
		return;
	}
	userQuery = queryInput->text().toStdString();
	if(singleOption->isChecked()){
		searchengine.parseRequest(userQuery, wrongFormat, webpagesContainingFirstWord, webpagesContainingSecondWord);
	}
	else if(andOption->isChecked()){
		userQuery = "AND "+userQuery;
		searchengine.parseRequest(userQuery, wrongFormat, webpagesContainingFirstWord, webpagesContainingSecondWord);
	}
	else if(orOption->isChecked()){
		userQuery = "OR "+userQuery;
		searchengine.parseRequest(userQuery, wrongFormat, webpagesContainingFirstWord, webpagesContainingSecondWord);
	}
	////the search process finished here
	//check if the result is wrong format
	if(wrongFormat){
		QMessageBox msgBox;
		msgBox.setWindowTitle("Oops!");
		msgBox.setText("\"Oops! Seems that your query format is invalid!\"");
		msgBox.setStandardButtons(QMessageBox::Yes);
		msgBox.setDefaultButton(QMessageBox::Yes);
		msgBox.exec();
	}
	else{
		myset<WebPage*> extended;
		for(set<WebPage*>::iterator it = webpagesContainingFirstWord.begin();
      		it != webpagesContainingFirstWord.end();
      		++it){

	  		myset<WebPage*> allin = (**it).incoming_links();
	  		myset<WebPage*> allOut = (**it).outgoing_links();
	  		for(set<WebPage*>::iterator itIn = allin.begin();
      		itIn != allin.end();
      		++itIn){
      			if(extended.count(*itIn)==0){ //maybe here check if the link exists
      				extended.insert(*itIn);
      			}
      		}
      		for(set<WebPage*>::iterator itOut = allOut.begin();
      		itOut != allOut.end();
      		++itOut){
	  			if(extended.count(*itOut)==0){ //maybe here check if the link exists
      				extended.insert(*itOut);
      			}
      		}
      	}
      	webpagesContainingFirstWord = webpagesContainingFirstWord.set_union(extended);
      	extended.clear();
		for(set<WebPage*>::iterator it = webpagesContainingFirstWord.begin();
      		it != webpagesContainingFirstWord.end();
      		++it){
			searchResult.push_back(*it);
		}
		//display the result by default, sorted by filenames 
		if(searchResult.size()==0){
			QMessageBox msgBox;
			msgBox.setWindowTitle("Oops!");
			msgBox.setText("\"0 Hits! None of the website contains your words\"");
			msgBox.setStandardButtons(QMessageBox::Yes);
			msgBox.setDefaultButton(QMessageBox::Yes);
			msgBox.exec();
		}
		else{

			//for each webpage in the candidate set, count the valid in/out links and map them
			for(int i=0; i<(int)searchResult.size();i++){
				//initialize this node, map it to the webpage
				int initialization_link = 0;	
				int initialization_link2 = 0;
				double initialization_pagevalue = 1/(double)searchResult.size();
				node newNode;
				newNode.numberOfValidIncomingLinks = initialization_link;
				newNode.numberOfValidOutgoingLinks = initialization_link2;
				newNode.currentPageRankValue = initialization_pagevalue;
				PageRankGraph[searchResult[i]] = newNode;

				//identify the incoming and outgoing degree of this node
				//indentify the set of in/out links within the candidate set(valid)
	  			myset<WebPage*> allin = searchResult[i]->incoming_links();
	  			myset<WebPage*> allOut = searchResult[i]->outgoing_links();
	  			for(set<WebPage*>::iterator itIn = allin.begin();
      			itIn != allin.end();
      			++itIn){
      				if(webpagesContainingFirstWord.count(*itIn)==1){	//maybe here check if the link exists
      					PageRankGraph[searchResult[i]].numberOfValidIncomingLinks += 1;
      					PageRankGraph[searchResult[i]].ValidIncomingLinks.insert(*itIn); 
      				}
      			}
      			for(set<WebPage*>::iterator itOut = allOut.begin();
      			itOut != allOut.end();
      			++itOut){
      				if(webpagesContainingFirstWord.count(*itOut)==1){	//maybe here check if the link exists
      					PageRankGraph[searchResult[i]].numberOfValidOutgoingLinks += 1;
      					PageRankGraph[searchResult[i]].ValidOutgoingLinks.insert(*itOut); 
      				}
      			}

      			//adding self-links
      			if(PageRankGraph[searchResult[i]].ValidIncomingLinks.count(searchResult[i])==0){
      				PageRankGraph[searchResult[i]].numberOfValidIncomingLinks += 1;
      				PageRankGraph[searchResult[i]].ValidIncomingLinks.insert(searchResult[i]);
      			}
      			if(PageRankGraph[searchResult[i]].ValidOutgoingLinks.count(searchResult[i])==0){
      				PageRankGraph[searchResult[i]].numberOfValidOutgoingLinks += 1;
      				PageRankGraph[searchResult[i]].ValidOutgoingLinks.insert(searchResult[i]);
      			}
			}
			//after the initializataion above, start iteration to calculate pagerank for each page
			for(int i=0; i<PageRankSteps; i++){
				for(std::map<WebPage*, node>::iterator itmap = PageRankGraph.begin();
					itmap != PageRankGraph.end();
					++itmap){

					double newPageRankValue = 0;
					//With the remaining probability 1-ε, he follows an outgoing link (chosen uniformly at random) from his current node v. That is, for each directed edge (v,u) (including the self loop), 
					//he next goes to u with probability 1/d+(v).
					for(set<WebPage*>::iterator itIn = itmap->second.ValidIncomingLinks.begin();
	      			itIn != itmap->second.ValidIncomingLinks.end();
	      			++itIn){
						newPageRankValue += (1-Restart_Probability)*(PageRankGraph[*itIn].currentPageRankValue)/(double)(PageRankGraph[*itIn].numberOfValidOutgoingLinks);
					}
					newPageRankValue += Restart_Probability/searchResult.size();
					//dynamically compute new pagevalue of this node, store it in PageRANK of a webpage
					itmap->first->setPageRank(newPageRankValue);
				}
				//now we compute all the new pagerank value after one iteration, update it
				for(std::map<WebPage*, node>::iterator itmap = PageRankGraph.begin();
					itmap != PageRankGraph.end();
					++itmap){
				itmap->second.currentPageRankValue = itmap->first->getPageRank();
				}
			}

			//here we finish calculating page rank for each page after given number of iterations
			//now we sort them by PageRank (default)
			mergeSort(searchResult, pageRankComp);

			for(int i=0; i<(int)searchResult.size();i++){
			qstr = QString::fromStdString(searchResult[i]->filename());
			qstr = qstr + "	IncomingLinks:" + QString::number(searchResult[i]->incoming_links().size());
			qstr = qstr + "	OutgoingLinks:" + QString::number(searchResult[i]->outgoing_links().size());
			qstr = qstr + "	PageRank:" + QString::number(PageRankGraph[searchResult[i]].currentPageRankValue);
			resultNameList->addItem(qstr);
			}	
		}	
	}
	resultNameList->setCurrentRow(0);
	//after fetching, clean up the data to prepare for next search
	queryInput->setText("");
	wrongFormat=false;
    webpagesContainingFirstWord.clear();
    webpagesContainingSecondWord.clear();
}

	
void qtSearchWindow::displayByPageRank(){
	resultNameList->clear();
	webpagePageRankComp pageRankComp;
	QString qstr;
	mergeSort(searchResult, pageRankComp);

	for(int i=0; i<(int)searchResult.size();i++){
	qstr = QString::fromStdString(searchResult[i]->filename());
	qstr = qstr + "	IncomingLinks:" + QString::number(searchResult[i]->incoming_links().size());
	qstr = qstr + "	OutgoingLinks:" + QString::number(searchResult[i]->outgoing_links().size());
	qstr = qstr + "	PageRank:" + QString::number(PageRankGraph[searchResult[i]].currentPageRankValue);
	resultNameList->addItem(qstr);
	}
	resultNameList->setCurrentRow(0);
}

void qtSearchWindow::displayByFilename(){
	resultNameList->clear();
	webpageNameComp nameComp;
	QString qstr;
	mergeSort(searchResult, nameComp);

	for(int i=0; i<(int)searchResult.size();i++){
	qstr = QString::fromStdString(searchResult[i]->filename());
	qstr = qstr + "	IncomingLinks:" + QString::number(searchResult[i]->incoming_links().size());
	qstr = qstr + "	OutgoingLinks:" + QString::number(searchResult[i]->outgoing_links().size());
	qstr = qstr + "	PageRank:" + QString::number(PageRankGraph[searchResult[i]].currentPageRankValue);
	resultNameList->addItem(qstr);
	}
	resultNameList->setCurrentRow(0);
}

void qtSearchWindow::detailedDisplayByFileNames(){
	webpageDetailedOutgoingList->clear();
	webpageDetailedIncomingList->clear();
	webpageNameComp nameComp;
	QString qstr;
	mergeSort(detailedOutgoingList, nameComp);
	mergeSort(detailedIncomingList, nameComp);
	for(int i=0; i<(int)detailedOutgoingList.size();i++){
	qstr = QString::fromStdString(detailedOutgoingList[i]->filename());
	qstr = qstr + "	IncomingLinks:" + QString::number(detailedOutgoingList[i]->incoming_links().size());
	qstr = qstr + "	OutgoingLinks:" + QString::number(detailedOutgoingList[i]->outgoing_links().size());
	webpageDetailedOutgoingList->addItem(qstr);
	}
	for(int i=0; i<(int)detailedIncomingList.size();i++){
	qstr = QString::fromStdString(detailedIncomingList[i]->filename());
	qstr = qstr + "	IncomingLinks:" + QString::number(detailedIncomingList[i]->incoming_links().size());
	qstr = qstr + "	OutgoingLinks:" + QString::number(detailedIncomingList[i]->outgoing_links().size());
	webpageDetailedIncomingList->addItem(qstr);
	}
	webpageDetailedIncomingList->setCurrentRow(0); //prevent segfault
	webpageDetailedOutgoingList->setCurrentRow(0);	//prevent segfault
}


void qtSearchWindow::displayByIncomingLinks(){
	resultNameList->clear();
	webpageIncomingLinksComp incomingLinksComp;
	QString qstr;
	mergeSort(searchResult, incomingLinksComp);

	for(int i=0; i<(int)searchResult.size();i++){
	qstr = QString::fromStdString(searchResult[i]->filename());
	qstr = qstr + "	IncomingLinks:" + QString::number(searchResult[i]->incoming_links().size());
	qstr = qstr + "	OutgoingLinks:" + QString::number(searchResult[i]->outgoing_links().size());
	qstr = qstr + "	PageRank:" + QString::number(PageRankGraph[searchResult[i]].currentPageRankValue);
	resultNameList->addItem(qstr);
	}
	resultNameList->setCurrentRow(0);
}

void qtSearchWindow::detailedDisplayByIncomingLinks(){
	webpageDetailedOutgoingList->clear();
	webpageDetailedIncomingList->clear();
	webpageIncomingLinksComp incomingLinksComp;
	QString qstr;
	mergeSort(detailedOutgoingList, incomingLinksComp);
	mergeSort(detailedIncomingList, incomingLinksComp);
	for(int i=0; i<(int)detailedOutgoingList.size();i++){
	qstr = QString::fromStdString(detailedOutgoingList[i]->filename());
	qstr = qstr + "	IncomingLinks:" + QString::number(detailedOutgoingList[i]->incoming_links().size());
	qstr = qstr + "	OutgoingLinks:" + QString::number(detailedOutgoingList[i]->outgoing_links().size());
	webpageDetailedOutgoingList->addItem(qstr);
	}
	for(int i=0; i<(int)detailedIncomingList.size();i++){
	qstr = QString::fromStdString(detailedIncomingList[i]->filename());
	qstr = qstr + "	IncomingLinks:" + QString::number(detailedIncomingList[i]->incoming_links().size());
	qstr = qstr + "	OutgoingLinks:" + QString::number(detailedIncomingList[i]->outgoing_links().size());
	webpageDetailedIncomingList->addItem(qstr);
	}
	webpageDetailedIncomingList->setCurrentRow(0); //prevent segfault
	webpageDetailedOutgoingList->setCurrentRow(0);	//prevent segfault
}

void qtSearchWindow::displayByOutgoingLinks(){
	resultNameList->clear();
	webpageOutgoingLinksComp outcomingLinksComp;
	QString qstr;
	mergeSort(searchResult, outcomingLinksComp);

	for(int i=0; i<(int)searchResult.size();i++){
	qstr = QString::fromStdString(searchResult[i]->filename());
	qstr = qstr + "	IncomingLinks:" + QString::number(searchResult[i]->incoming_links().size());
	qstr = qstr + "	OutgoingLinks:" + QString::number(searchResult[i]->outgoing_links().size());
	qstr = qstr + "	PageRank:" + QString::number(PageRankGraph[searchResult[i]].currentPageRankValue);
	resultNameList->addItem(qstr);
	}
	resultNameList->setCurrentRow(0);
}

void qtSearchWindow::detailedDisplayByOutgoingLinks(){
	webpageDetailedOutgoingList->clear();
	webpageDetailedIncomingList->clear();
	webpageOutgoingLinksComp outcomingLinksComp;
	QString qstr;
	mergeSort(detailedOutgoingList, outcomingLinksComp);
	mergeSort(detailedIncomingList, outcomingLinksComp);
	for(int i=0; i<(int)detailedOutgoingList.size();i++){
	qstr = QString::fromStdString(detailedOutgoingList[i]->filename());
	qstr = qstr + "	IncomingLinks:" + QString::number(detailedOutgoingList[i]->incoming_links().size());
	qstr = qstr + "	OutgoingLinks:" + QString::number(detailedOutgoingList[i]->outgoing_links().size());
	webpageDetailedOutgoingList->addItem(qstr);
	}
	for(int i=0; i<(int)detailedIncomingList.size();i++){
	qstr = QString::fromStdString(detailedIncomingList[i]->filename());
	qstr = qstr + "	IncomingLinks:" + QString::number(detailedIncomingList[i]->incoming_links().size());
	qstr = qstr + "	OutgoingLinks:" + QString::number(detailedIncomingList[i]->outgoing_links().size());
	webpageDetailedIncomingList->addItem(qstr);
	}
	webpageDetailedIncomingList->setCurrentRow(0); //prevent segfault
	webpageDetailedOutgoingList->setCurrentRow(0);	//prevent segfault
}

void qtSearchWindow::quitMain(){
	detailedWindow->close();
 	close();

}

void qtSearchWindow::quitSub(){
	detailedWindow->hide();
}
