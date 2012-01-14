all: rss parse
	gcc parse.o g15rss.c  rss.o -lg15daemon_client -lg15render -lpthread -lm -lrt -o g15rss -Wunused -lcurl `xml2-config --cflags` `xml2-config --libs`

install: 
	cp g15rss /usr/bin/

uninstall:
	rm g15rss /usr/bin/g15rss

rss:
	gcc -c rss.c -lcurl


clean:
	rm g15rss rss.o parse.o

parse:
	gcc -c `xml2-config --cflags` `xml2-config --libs` parse.c

