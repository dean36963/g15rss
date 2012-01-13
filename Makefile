all: rss
	gcc g15rss.c rss.o -lg15daemon_client -lg15render -lpthread -lm -lrt -o g15rss -Wunused -lcurl

install: all
	cp g15rss /usr/bin/
#	touch /etc/g15rssrc

uninstall:
	rm g15rss /usr/bin/g15rss

rss:
	gcc -c rss.c -lcurl


clean:
	rm g15rss rss.o
