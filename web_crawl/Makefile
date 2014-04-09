# Name: Ani Ramchandran
# webcrawl makefile

CC = g++
CFLAGS = -g -Wall -Ilib/ -I.
LDFLAGS =

all : webpageSTL.o web_crawler.o webCrawlDriver.o
	@echo "Building webpageSTL.o web_crawler.o webCrawlDriver.o"
	@echo "Compiling and linking"
	$(CC) $(CFLAGS)  bin/webpageSTL.o bin/web_crawler.o bin/webCrawlDriver.o -o bin/search 
	
webpageSTL.o : webpageSTL.cpp
	$(CC) $(CFLAGS) -c webpageSTL.cpp -o bin/webpageSTL.o

web_crawler.o: web_crawler.cpp
	$(CC) $(CFLAGS) -c web_crawler.cpp -o bin/web_crawler.o

webCrawlDriver.o: webCrawlDriver.cpp
	$(CC) $(CFLAGS) -c webCrawlDriver.cpp -o bin/webCrawlDriver.o
	
clean:
	rm -f bin/search bin/*.o *~
