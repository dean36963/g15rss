#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

struct rssfeed
{
	char title[80];
	char feeds[480];
};

int getRssCurl(char *filename, char *url);
int probeRssFeed(struct rssfeed *rss);
