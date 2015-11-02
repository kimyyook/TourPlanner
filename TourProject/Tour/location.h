#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int locating(char * location){
	int i, result=0;
	char station[50];
	char *cmp[100] = {'\0'};
	char p[20]={'\0',};
    FILE *f;
	
	
	strcpy(station,location);
	/*
	printf("station : ");
	gets(station);
	*/
	strcat(station, "\n");
	
	f=fopen("getsstation.txt","r");
	
	for(i=0; i<96; i++)
	{
		fgets(p, sizeof(p), f);
		cmp[i] = (char*)malloc(strlen(p)+1);
		strcpy(cmp[i],p);
	}

	for(i=0; i<96; i++)
		if(strcmp(station, cmp[i])==0)
			result = 1;

	fclose(f);
	
	if(result==1)
		return 1;
	else
		return 0;
}