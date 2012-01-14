#include <stdio.h>
#include <libxml/xmlreader.h>

/*
void processNode(xmlTextReaderPtr reader)
{
    const xmlChar *name, *value;

    name = xmlTextReaderConstName(reader);
    if (name == NULL)
	name = BAD_CAST "--";

    value = xmlTextReaderConstValue(reader);

    printf("%d %d %s %d %d", 
	    xmlTextReaderDepth(reader),
	    xmlTextReaderNodeType(reader),
	    name,
	    xmlTextReaderIsEmptyElement(reader),
	    xmlTextReaderHasValue(reader));
    if (value == NULL)
	printf("\n");
    else {
        if (xmlStrlen(value) > 40)
            printf(" %.40s...\n", value);
        else
	    printf(" %s\n", value);
    }
}
*/


int myProcessNode(xmlTextReaderPtr reader,int *lastdepth, int *readnext ,int *mode,char** savehere,int *i)
{
    const xmlChar *name, *value;

    name = xmlTextReaderConstName(reader);
    if (name == NULL)
	name = BAD_CAST "--";

    value = xmlTextReaderConstValue(reader);

	if(*readnext == 1 && *lastdepth == xmlTextReaderDepth(reader)-1)
	{
		if(*i <6)
		{
			sprintf(savehere[*i],"%s",value);
			*i=*i + 1;
		}
		else
			return 1;
	}
	else if(0==strncmp(name,"title",5) && *mode>0)
	{
		*readnext = 1;
		*lastdepth = xmlTextReaderDepth(reader);
		if(*mode==1)
			*mode=0;//now look for items
	}
	else if(0==strncmp(name,"item",4)||0==strncmp(name,"entry",5))
	{	//find some news feeds
		*mode=2;
	}
	else
		*readnext = 0;
	return 0;
}

int streamFile(const char *filename,char** savehere)
{
    xmlTextReaderPtr reader;
    int ret,dep=0,nxt=0,mode=1,i=0,fin=0;

    reader = xmlReaderForFile(filename, NULL, 0);
    if (reader != NULL)
	{
        ret = xmlTextReaderRead(reader);
        while (ret == 1)
		{
            fin = myProcessNode(reader,&dep,&nxt,&mode,savehere,&i);
			if(fin==1)
				return 0;
            ret = xmlTextReaderRead(reader);
        }
        xmlFreeTextReader(reader);
        if (ret != 0)
		{
            fprintf(stderr, "%s : failed to parse\n", filename);
        }
    }
	else
	{
        fprintf(stderr, "Unable to open %s\n", filename);
    }
}
/*
int main(int argc, char **argv)
{
	int i;
	char **rssfeeds;//will define an array of ptrs , defining strings of feeds
	//initialised to size of one so far
	rssfeeds = (char**) malloc( 6*sizeof(char*) );
	for(i=0;i<6;i++) rssfeeds[i] = (char*) malloc( 128 * sizeof(char) );

    if (argc != 2)
        return(1);

    LIBXML_TEST_VERSION

    streamFile(argv[1],rssfeeds);

    xmlCleanupParser();
    xmlMemoryDump();

	for(i=0;i<6;i++) printf("Rss = %s\n",rssfeeds[i]);


    return(0);
}*/

