#include <stdio.h>
#include <string.h>
#include <stdlib.h>



int login_infomation(char *user, char * password)
{
	int accnt_cnt = 0;
	int i;
	int j;
	
	int login_flag = 0;
	int char_cnt;
	char name[50]={'\0',};
	char code[50]={'\0',};
	char cmp[100]={'\0',};
	char p[50]={'\0',};
	int cmp_int;

	FILE *f;

	f=fopen("member.txt","r");
	strcpy(name,user);
	strcpy(code, password);

	strcat(cmp,name);
	strcat(cmp, "_");
	strcat(cmp,code);

	fgets(p, sizeof(p), f);
	accnt_cnt = atoi(p);
	
	for(i=0;i < accnt_cnt;i++){
		char_cnt = 0;
		fgets(p, sizeof(p), f);

		for( j = 0; j < 50; j++)
		{
			if( p[j] != '\0' )
				char_cnt++;
			else
				break;
		}

		p[ char_cnt - 1 ] = '\0';

		cmp_int = strcmp( p, cmp );

		if( cmp_int == 0)
		{
			
			login_flag = 1;
			return 1;
			break;
		}
		else{
			
		}
	}

	if( login_flag == 0 )
		return 0;
		

	login_flag = 0;

	

	fclose(f);
}
