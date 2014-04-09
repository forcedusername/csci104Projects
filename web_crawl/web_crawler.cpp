//#include <fstream>
//#include <string>
#include "web_crawler.h"

using namespace std;

//so what's going to happen? 
//you need to parse the webpages, so you'd have to probably include webpage.h or inherit from it
//do you need hyperlinks> yes, that's how you figure out where everything goes
//your seed file has some files, you follow those files' outgoing links 
//write all seed file filenames to output file first
//check outgoing links with ones in output file, if not in seed file, write to output file
//you take in seed file, have output
//walk through seed file, parse, (is this necessary, you only need to get the links)
//for DFS, you pursue all the outgoing links from a certain page and then go to the next page
//first finish getting all the links from the seed file and finish writing.
//stack or recursion? 

//functions are: write file, parse, depth first search
//call depth first search first, then call parse

/*
void WebCrawler::open_file (string input_file, map<string, WebPage*> &pageMap){
	ifstream input (input_file.c_str());
	//ifstream input;
    string webfilename;

    //int position = 0;
    while (getline (input, webfilename)){
        WebPage* pWebPage = new WebPage(webfilename);
        webPagePtrList.push_back(pWebPage);
        pageMap.insert (pair <string, WebPage*> (webfilename, pWebPage));
    }

	
}
*/

void WebCrawler :: write_file (char* output_file, map<string, WebPage*> &pageMap){
	//use parsing before, now this code is for writing to text file and recursing for the DFS
	//situation is: you have already found the outgoing links in a certain webpage...
	//you then call the DFS function which will search for it all
	//then you call write_file
	//ifstream input (input_file.c_str());
	ofstream writer (output_file);
	map<string, WebPage*>::iterator printing=pageMap.begin();
	while (printing!=pageMap.end()){
		writer<<printing->first<<endl;
		printing++;
	}
	writer.close();
	pageMap.clear();
	
}

void WebCrawler :: parse (const char* filename, map<string, WebPage*> &pageMap){
	string web_name (filename);
	WebPage* insert;
	try {
		insert=pageMap.at (web_name);
	}
	catch (...){
		insert = new WebPage (web_name);
		pageMap.insert (pair <string, WebPage*> (web_name, insert));
	}
	//you're parsing a file into a webpage, you should put all this into a new webpage 
	
	ifstream input (web_name.c_str());
	string placer;
	//so what happens in these four lines......
	//never entered for first two files...
	
	while (getline (input, placer)){
		insert->lineVector.push_back (placer);
		//cout<<"Inserting for: "<<web_name<<endl;
	}
	input.close ( );
	
	/*
	unsigned int temp=0;
	while (temp<insert->lineVector.size()){
		cout<<"line vector size is: "<<insert->lineVector.size()<<endl;
		cout<<"Filename is: "<<insert->filename()<<endl;
		cout<<"Line vector is: "<<insert->lineVector[temp]<<endl;
		temp++;
	}
	*/
	
    unsigned int i = 0;
    while (i<insert->lineVector.size()) {
		string explorer = insert->lineVector[i];
		char *replacer = new char[explorer.size()+1];
	
		unsigned int j=0;
		int z=0;
		while (j<explorer.length ()){
			if (explorer[j]<='9'&&explorer[j]>='0'){
				replacer [z++]=explorer[j];
                insert->ssWebPage << explorer[j];
			}
			else if (explorer[j]<='Z'&&explorer[j]>='A'){
				replacer [z++]=tolower(explorer[j]);
                insert->ssWebPage << explorer[j];
            }
			else if (explorer[j]<='z' && explorer[j]>='a'){
				replacer [z++]=explorer [j];
                insert->ssWebPage << explorer[j];
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
                        insert->ssWebPage << explorer[j];
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
                    insert->ssWebPage << explorer[j];
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
                        insert->ssWebPage << explorer[j];
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
                    insert->topSecret_stuff.insert (word);
        
                try {
                    string webfilename(webpage);
                    WebPage *pPoints2WebPage = pageMap.at (webfilename);
                    insert->points_to.insert (pPoints2WebPage);
                    pPoints2WebPage->connected_to.insert (insert);
                }
                catch (...){
				//you go and do your depth search if 
					//what do you do if the webpage isnt in the map??
			
					string webfile (webpage);
					cout<<"Entering catch because: "<<webfile<<" isn't in pagemap."<<endl;
					WebPage *pPoints2WebPage= new WebPage(webfile);
					insert->points_to.insert(pPoints2WebPage);
					pPoints2WebPage->connected_to.insert (insert);
					//pageMap.insert 
					
					parse (pPoints2WebPage->filename().c_str(), pageMap);
					//depth_search (insert, pageMap);
					//but this doesn't recognize anything as an outgoing link.....
					
                    //depth_search (pPoints2WebPage, pageMap);
					//should you call parse again? 
					//parse (pPoints2WebPage->filename(), pageMap);
                }
                insert->ssWebPage << hyperlink;
                delete [ ] webpage;
                delete [ ] hyperlink;
                // now that we got both webpage and hyper link, move j past explore_iter;

                j = explore_iter;
                continue;
            } // else '['
            else {
                insert->ssWebPage << explorer[j];
                replacer [z]='\0';
                z=0;
                string compare_str(replacer);
                if (insert->topSecret_stuff.contains (compare_str)==false){
                    pair<Set<string, string_compare>::iterator,bool> ret;
                    ret = insert->topSecret_stuff.insert (replacer);
                    //cout << "insert ret is : " << ret.second << endl;
                }
            }
            j++;
        } // while j
        delete [ ]replacer;
        i++;
  } // while i
  //cout<<"Parse done.. num words = " << insert->topSecret_stuff.size() << endl;
  
} // parse

/*
void WebCrawler::depth_search (WebPage* search_links, map <string, WebPage*> &allPages){
	
	Set <WebPage*, WebPtr_compare> outgoing = search_links->allOutgoingLinks(); 
	
	//make a copy and then keep removing things from outCopy?
	//you want to make a copy of the outgoing, but is this possible 
	//Set <WebPage*, WebPtr_compare> outCopy=outgoing;
	Set <WebPage*, WebPtr_compare>::iterator next_pageOut= outgoing.begin();
	
	//is this check needed? yes, because you'll shrink outCopy 
	if (next_pageOut!=outgoing.end()){
		//you want to get to the end of the thread before you start checking
		
		Set<WebPage*, WebPtr_compare>::iterator next_pagePtr= next_pageOut++;
		
		//can you remove an element via iterator
		//outCopy.erase (next_pageOut);
		//you remove so that you can shrink outCopy because you keep doing begin
		//do you want to do that here though? 
		
		depth_search (*next_pagePtr, allPages);
	}
	
	//end of what though? because you're just checking if you've hit the end of the
	//outgoing links of your particular pointer 
	//you'll have to keep a running count of every web page you've found w filenames 
	//this just means that you've reached a file w no hyperlinks?
	
	else {
		//results.insert (*next_pageOut);
		Set<WebPage*, WebPtr_compare>::iterator next_pagePtr=outgoing.begin();
		while (next_pagePtr!=outgoing.end()){
			//try {
			//	WebPage* dummy= allPages.at (*next_pagePtr);
			//	next_pagePtr++;
			//}
			//catch (...){
				WebPage* next= *next_pagePtr; 
				WebPage* add_page= new WebPage (next->filename()); 
				//parse it
				allPages.insert (pair<string, WebPage*> (next->filename(), add_page));
				//insert into map of all webpages so you can keep track
				//you should parse after you update the map?
				
				//parse (next->filename(), allPages);
				//add_page->parse (next->filename(), allPages);
				
				next_pagePtr++;
				//here, you get the webpage pointer from next page ptr 
				//does the webpage that next page ptr points to have a filename? 
			//}
		}
		//outCopy.erase (next_pagePtr);
		
		return;
	}
	
}
*/