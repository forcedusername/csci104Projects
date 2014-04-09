#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include "setSTL.h"
#include <map>
#include <algorithm>
#include <cstdlib>
#include <string>
#include "comparatorsSTL.h"
#include "webpageSTL.h"

//is this necssary?
//#include "web_crawler.h"

bool  WebPtr_compare::operator()(WebPage* const page1, WebPage* const page2)
{
      bool ret = false;
      if (page1->filename() > page2->filename())
        ret = true;
			return ret;
}

WebPage *WebPage::getWebPageFromMap(const string &page_filename, map<string, WebPage*> &pageMap)
{
	map<string, WebPage*>::iterator it= pageMap.begin();
	while (it!=pageMap.end()){
		if (it->second->filename() == page_filename)
    {
      return it->second;
    }
    it++;
	}
  return NULL;
}


WebPage::WebPage (string filename){
	page_filename=filename;
	//void parse (page_filename, &pageMap);
}

void WebPage::parse (const string &filename, map<string, WebPage*> &pageMap){
	ifstream input (filename.c_str());
	string placer;

	while (getline (input, placer)){
		lineVector.push_back (placer);
	}
	input.close ( );
    unsigned int i = 0;
    while (i<lineVector.size()) {
		string explorer = lineVector[i];
		char *replacer = new char[explorer.size()+1];
	
		unsigned int j=0;
		int z=0;
		while (j<explorer.length ()){
			if (explorer[j]<='9'&&explorer[j]>='0'){
				replacer [z++]=explorer[j];
                ssWebPage << explorer[j];
			}
			else if (explorer[j]<='Z'&&explorer[j]>='A'){
				replacer [z++]=tolower(explorer[j]);
                ssWebPage << explorer[j];
            }
			else if (explorer[j]<='z' && explorer[j]>='a'){
				replacer [z++]=explorer [j];
                ssWebPage << explorer[j];
            }
			else if (explorer [j] == '[') {
				unsigned int explore_iter=j+1;
				int hyp_fill=0;
				char* hyperlink=new char [explorer.size()+1];
				//string hyperlink instead of char* doesnt work...
				
                while (explorer [explore_iter]!=']' ){
                    hyperlink [hyp_fill]=explorer [explore_iter];
                    hyp_fill++;
                    explore_iter++;
                    if (explore_iter >= explorer.length ())
                    {
                        // not a proper hyper link, let us capture remaining words for indexing
                        delete [ ] hyperlink;
                        ssWebPage << explorer[j];
                        j++;
                        break; // break this while loop and continue to 'j' loop
                    }
                }

                hyperlink [hyp_fill]='\0';
        
                // Let us skip ']'
                explore_iter++;
				
                if (explorer [explore_iter] !='('){
                    // not a proper hyper link, let us capture remaining words for indexing
                    delete [ ] hyperlink;
                    ssWebPage << explorer[j];
                    j++;
                    continue; // continue to 'j' loop
                }
                // Let us skip '('
                explore_iter++;
        
                char* webpage= new char [explorer.size()+1];
				//string webpage doesnt work either...
				
                int wpage_fill=0;
                while (explorer [explore_iter]!=')'){
                    webpage [wpage_fill]=explorer [explore_iter];
                    wpage_fill++;
                    explore_iter++;
                    if (explore_iter >= explorer.length ())
                    {
                        // not a proper hyper link, let us capture remaining words for indexing
                        delete [ ] hyperlink;
                        delete [ ] webpage;
                        ssWebPage << explorer[j];
                        j++;
                        break; // break this while loop and continue to 'j' loop
                    }
                }
                webpage [wpage_fill]='\0';
                // skip ')'
                explore_iter++;

                // Now insert each word in the hyper link into allWords.
                stringstream ss(hyperlink);
                string word;
                for (int i = 0; ss>>word; i++)
                    topSecret_stuff.insert (word);
        
                try {
                    string webfilename(webpage);
                    WebPage *pPoints2WebPage = pageMap.at (webfilename);
                    points_to.insert (pPoints2WebPage);
                    pPoints2WebPage->connected_to.insert (this);
                }
                catch (...){
                    ;
                }
                 ssWebPage << hyperlink;
                delete [ ] webpage;
                delete [ ] hyperlink;
                // now that we got both webpage and hyper link, move j past explore_iter;

                j = explore_iter;
                continue;
            } // else '['
            else {
                ssWebPage << explorer[j];
                replacer [z]='\0';
                z=0;
                string compare_str(replacer);
                if (topSecret_stuff.contains (compare_str)==false){
                    pair<Set<string, string_compare>::iterator,bool> ret;
                    ret = topSecret_stuff.insert (replacer);
                    //cout << "insert ret is : " << ret.second << endl;
                }
            }
            j++;
        } // while j
        delete [ ]replacer;
        i++;
  } // while i
  //cout<<"Parse done.. num words = " << topSecret_stuff.size() << endl;
} // parse

ostream& operator<< (ostream &os, const WebPage &page){
    /*
	int sizer= page.lineVector.size();
	//do not print out any of the filename, just the anchor text when you find a link
	
	int i=0;
	string rover;
	while (i<sizer){
		rover= page.lineVector[i];
		os<<rover;
		os<<endl;
		i++;
	}
    */
    os<<page.ssWebPage.str();

  return os;
}

string WebPage:: filename () const {
	return page_filename;
}

Set<string, string_compare> WebPage:: allWords () const{
	return topSecret_stuff;
}

Set <WebPage*, WebPtr_compare> WebPage::allOutgoingLinks ()const {
	return points_to;
}

Set <WebPage*, WebPtr_compare> WebPage::allIncomingLinks ()const {
	return connected_to;
}

