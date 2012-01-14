#include "rss.h"
 
int getRssCurl(char *filename, char *url)
{
	CURL *curl;
	CURLcode res;
	FILE *savehere;

	//init file
	savehere = fopen(filename,"w");
	if(savehere==NULL)
	{
		printf("Couldn't write file: %s\n",filename);
		return -1;
	}
 
	curl = curl_easy_init();
	if(curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA,savehere);
		res = curl_easy_perform(curl);
		//cleanup
		curl_easy_cleanup(curl);
	}
	fclose(savehere);
	if(res != 0)
	{
		printf("There was an error downloading!\n");
		printf("Error = %d\n",res);
		return -1;
	}
	return 0;
}

//I dont use this anymore, but scared to delete it!
/*
int probeRssFeed(struct rssfeed *rss)
{
	int loop=1,i;
	FILE *file;
	char *buffer;
	char *start,*end,*storehere;


	buffer = (char*)malloc(BUFSIZ*sizeof(char));

	file = fopen("/tmp/g15rssfeed","r");
	if(file == NULL)
	{
		printf("Couldn't open /tmp/g15rssfeed\n");
		return -1;
	}

	while(loop)
	{
		fgets(buffer,BUFSIZ,file);
		if(buffer==NULL)
		{
			printf("buffer is null wtF!\n");
			loop=0;
		}
		if(buffer=="EOF")
		{
			printf("Buffer is EOF now!\n");
			return -2;
		}
		start = strstr(buffer,"<title>");
		if(start!=NULL)
		{
			end = strstr(buffer,"</title>");
			strncpy(rss->title,start+7,end-start-7);
			rss->title[end-start-7]='\0';
			start[2]='X';
			end[2]='X';
			loop=0;
		}
	}
	for(loop=1;loop==1;)
	{
		if(buffer==NULL)
		{
			printf("buffer is null wtF!\n");
			loop=0;
		}
		if(buffer=="EOF")
		{
			printf("Buffer is empty aborting!\n");
			return -2;
		}
		start = strstr(buffer,"<item>");
		if(start!=NULL)
		{
			loop=0;
		}
		else
		{
			fgets(buffer,BUFSIZ,file);
		}
	}
	for(i=0, loop=1;i<5 && loop==1;)
	{
		if(buffer==NULL)
		{
			printf("buffer is null wtF!\n");
			loop=0;
		}
		if(buffer=="EOF")
		{
			printf("BUFF = EOF\n");
			return -2;
		}
		start = strstr(buffer,"<title>");
		if(start!=NULL)
		{
			storehere=rss->feeds+80*i;
			end = strstr(buffer,"</title>");
			strncpy(storehere,start+7,end-start-7);
			storehere[end-start-7]='\0';
			start[2]='X';
			end[2]='X';
			i++;
		}
		else fgets(buffer,BUFSIZ,file);
	}
	fclose(file);
	free(buffer);
	return 0;
}
*/

