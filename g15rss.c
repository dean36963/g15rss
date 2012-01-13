//Simple RSS feed viewer for G15 keyboard!
//Dean Birch

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <libg15.h>
#include <ctype.h>
#include <g15daemon_client.h>
#include <libg15render.h>
#include <sched.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <math.h>
#include <pthread.h>
#include <time.h>
#include "rss.h"

pthread_cond_t wake_now = PTHREAD_COND_INITIALIZER;
const int MAX_STR_LENGTH=256;
const int DEFAULT_WAIT_TIME=20;


void g15_wait(int seconds);

int main(int argc, char *argv[])
{
	g15canvas *canvas;
	int i,j,length=0;
	int g15screen_fd;
	int fail=0;
	int wait_time=DEFAULT_WAIT_TIME;
	char *filename,*buffer;
	time_t rawtime;
	struct tm *realtime;
	struct rssfeed rss;
	char timestr[80];
	FILE *file;
	char **list;
	int numberoffeeds=0;

	//initialise filename string
	filename = (char*) malloc(MAX_STR_LENGTH*sizeof(char));
	sprintf(filename,"%s.g15rssrc",getenv("HOME"));

	//init buffer
	buffer = (char*) malloc(MAX_STR_LENGTH*sizeof(char));

	//copes with any amount of arguements and processes them in a logical way!
	for(i=0;i<argc;i++)
	{
		if(0==strncmp(argv[i],"-h",2)||0==strncmp(argv[i],"--help",6))
		{	//prints the help page
			printf("G15 RSS reader\n");
			printf("-h,--help\t\tShows this help page.\n");
			printf("-v,--version\t\tPrints version number.\n");
			printf("-c,--conf [path/to/file]\tUse this configuration file.\n");
			printf("Configuration files are a list of RSS urls.\n");
			printf("Send bug reports dean.birch0@gmail.com\n");
			return 0;
		}
		else if(0==strncmp(argv[i],"-v",2)||0==strncmp(argv[i],"--version",9))
		{	//prints version number
			printf("0.2\n");
			return 0;
		}
		else if(0==strncmp(argv[i],"-c",2)||0==strncmp(argv[i],"--conf",6))
		{	//allowing to specify filename
			if( i+1 <= argc )
			{
				filename = argv[i+1];
			}
			else
			{
				printf("-c/--conf should be followed by a filename.\n");
				return 0;
			}
		}
		else if(0==strncmp(argv[i],"-w",2)||0==strncmp(argv[i],"--wait",6))
		{	//sets waiting time between refreshing
			if( i+1 <= argc) wait_time=DEFAULT_WAIT_TIME;
			else
			{
				printf("-w/--wait should be followed by the wait time.\n");
				return 0;
			}
		}
	}

	//opening conf file
	file = fopen(filename,"r");
	if(file == NULL)
	{
		printf("Could not read %s.\nMake sure it exists and contains RSS urls.\n",filename);
		return -2;
	}
	while( fgets(buffer,BUFSIZ,file) )
	{
		if(buffer[0]!='#')
			numberoffeeds++;
		length++;
	}
printf("number of feeds should be %d\n",numberoffeeds);
	rewind(file);
	list = (char**) malloc( (size_t)numberoffeeds*sizeof(char*) );
	if(list == NULL) return -3;
	for(i=0,j=0;i<length;i++)
	{
		fgets(buffer,BUFSIZ,file);
		if(buffer[0]!='#')
		{
			list[j] = (char*) malloc( (size_t)MAX_STR_LENGTH*sizeof(char) );
			if(list[j]==NULL) return -4;
			//fgets(list[i],MAX_STR_LENGTH,file);
			strncpy(list[j],buffer,MAX_STR_LENGTH);
			printf("Feed number %d = %s\n",j,list[j]);
			j++;
		}
	}
	


	//connecting to g15daemon
	if((g15screen_fd=new_g15_screen(G15_G15RBUF))<0)
	{
		printf("Sorry cannot connect to the G15daemon\n");
		return -1;
	}

	//getting canvas
	canvas = (g15canvas *) malloc (sizeof (g15canvas));
	if(canvas!=NULL) g15r_initCanvas(canvas);
	else return -1;




	//switches through list of rss feeds, downloads and displays them
	for(i=0;1;i++)
	{
		//reset i if it gets too big!
		if(i>=numberoffeeds) i=0;
		printf("Displaying feed number %d\n",i);
		
		//get time
		time(&rawtime);
		realtime = gmtime(&rawtime);
		sprintf(timestr,"%2d:%02d",  (realtime->tm_hour)%24,realtime->tm_min);

		//get news feed and get info from it!
		fail = getRssCurl("/tmp/g15rssfeed",list[i]);
		if(fail == -1)
		{
			printf("Continuing to next loop iter!\n");
			g15r_clearScreen(canvas,0x00);
			g15r_renderString(canvas,(unsigned char*)timestr,0,G15_TEXT_MED,132,0);
			g15r_renderString(canvas,(unsigned char*)"Error Downloading",0,G15_TEXT_HUGE,0,0);
			//send to g15daemon
			g15_send(g15screen_fd,(char*)canvas->buffer,G15_BUFFER_LEN);
			//wait
			g15_wait(wait_time);
			continue;
		}
		probeRssFeed(&rss);
		
		//clear screen
		g15r_clearScreen(canvas,0x00);
		//write title, 5 most recent posts and time
		g15r_renderString(canvas,(unsigned char*)rss.title,		0, G15_TEXT_MED,	0,0);
		g15r_renderString(canvas,(unsigned char*)rss.feeds,		0, G15_TEXT_SMALL,	0,9);
		g15r_renderString(canvas,(unsigned char*)rss.feeds+80,	0, G15_TEXT_SMALL,	0,15);
		g15r_renderString(canvas,(unsigned char*)rss.feeds+160, 0, G15_TEXT_SMALL,	0,21);
		g15r_renderString(canvas,(unsigned char*)rss.feeds+240, 0, G15_TEXT_SMALL,	0,27);
		g15r_renderString(canvas,(unsigned char*)rss.feeds+320, 0, G15_TEXT_SMALL,	0,33);
		g15r_renderString(canvas,(unsigned char*)timestr,		0, G15_TEXT_MED,	132,0);
		//send to g15daemon
		g15_send(g15screen_fd,(char*)canvas->buffer,G15_BUFFER_LEN);
		//wait
		g15_wait(wait_time);
	}

	return 0;
	
}

//shamelessly stolen from g15stats program!
void g15_wait(int seconds) {
    pthread_mutex_t dummy_mutex;
    pthread_mutexattr_t   mta;
    struct timespec timeout;
      /* Create a dummy mutex which doesn't unlock for sure while waiting. */
    pthread_mutexattr_init(&mta);

    pthread_mutex_init(&dummy_mutex, &mta);
    pthread_mutex_lock(&dummy_mutex);

    if (clock_gettime(CLOCK_REALTIME, &timeout) != 0) {
        perror("clock_gettime");
    }

    timeout.tv_sec += seconds;

    pthread_cond_timedwait(&wake_now, &dummy_mutex, &timeout);
    pthread_mutex_unlock(&dummy_mutex);
    pthread_mutex_destroy(&dummy_mutex);
    pthread_mutexattr_destroy(&mta);
}

