#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <malloc.h>
#include <time.h>

#include "utility.h"
#include  "Model.h"	

 //general utils
size_t countAvailableTodos(char * path){
	
	errno=0;
    FILE *stream=fopen(path,"r");
    if(stream==NULL || errno){
      fprintf(stderr,"ERROR: %s\n",strerror(errno));
      return 0;
    }
	 
	 char * buffer = NULL;
	size_t n=0;
	int nblines=0;
	
	while(getline(&buffer,&n,stream)!=EOF)
	nblines++;
	fclose(stream);
	return nblines;
}
int notAnumber(char * buffer,int len){
	len=min(len,strlen(buffer));
	for(size_t i=0;i<len;i++)
	{
		if(buffer[i]!='0'&&buffer[i]!='1'&&buffer[i]!='2'&&buffer[i]!='3'&&buffer[i]!='4'&&buffer[i]!='5'&&buffer[i]!='6'&&buffer[i]!='7'&&buffer[i]!='8'&&buffer[i]!='9')
			return 1;
	}
	return  0;
}	
int isAnumber(char * buffer,int len){
	len=min(len,strlen(buffer));
	for(size_t i=0;i<len;i++)
	{
		if(buffer[i]!='0'&&buffer[i]!='1'&&buffer[i]!='2'&&buffer[i]!='3'&&buffer[i]!='4'&&buffer[i]!='5'&&buffer[i]!='6'&&buffer[i]!='7'&&buffer[i]!='8'&&buffer[i]!='9')
			return -1;
	}
	return  atoi(buffer);
}
int min(int a,int b){
	return (a>b)?b:a;
}	
void oneByOne(char* str, unsigned  int  delay ){
	int length=strlen(str);
	for(size_t i=0; i< length ; i++)
	{
		printf("%c",str[i]);
		msleep(delay);
		fflush(stdout);
	}
}
void msleep(unsigned  int  delay ){
	struct timespec tc;
	tc.tv_sec=0;
	if(delay>999)
		delay=999999999;
		else
		delay=delay*1E6;	
	tc.tv_nsec=delay;
	nanosleep(&tc,NULL);
}	
	//add utilities
void fillModel(){

	
	size_t n=0;
	char * buffer=NULL;
	size_t  buffer_length;
	
	for(size_t  i=0;i < MODELLEN-3 ; i++)
	{
			do{
			printf("%s:",todoModel[i][0]);
			//scanf("%[^\n]",todoModel[i][1]);
			buffer_length=getline(&buffer,&n,stdin);
			}while(buffer_length>255);	
			strncpy(todoModel[i][1],buffer,255);
			todoModel[i][1][buffer_length-1]=0;
	}
	strcpy(todoModel[MODELLEN-3][1],"created");
}
void serializeAndWriteToDb(FILE* DB){
	
	for(size_t i=0;i < MODELLEN-2;  i++)
	{
		addPeriod(todoModel[i][1]); 
		fprintf(DB,"%s",todoModel[i][1]);	
	}
	time_t modified_at= time(NULL);
	fprintf(DB,"%d\004",modified_at);
	time_t created_at= time(NULL);
	fprintf(DB,"%d\004\n",created_at);
}
void addPeriod(char * string ){
  //title       \fdesc        \ftime\n
  int extended_len=255;
  int trimlen = strlen(string);
  for (size_t  i = trimlen; i < extended_len-2; i++) {
      string[i]=3;
  }
  string[extended_len-2]=4;
  string[extended_len-1]=0;
}
	//showALL  utilities
void parseModelAndPrint(const int id,char* buffer){
	printf("todo°%d)\n",id);
	
	int  buffer_ptr=0;
	int field_ptr;
	char field[255];
	
  for(size_t i=0;i <MODELLEN; i++)
	{
		
		field_ptr=0;
		while(buffer[buffer_ptr]!=4)
			{
				if(buffer[buffer_ptr]!=3 )
				{
					field[field_ptr]=buffer[buffer_ptr];
					field_ptr++;
				}
				buffer_ptr++;
			}
			buffer_ptr++;
			field[field_ptr]=0;
			if(i==MODELLEN-1 || i==MODELLEN-2)
			{
				printf("	%s: %s\n",todoModel[i][0],convertTime(field));
			}
			else
			printf("	%s: %s\n",todoModel[i][0],field);
	}
}
int parseModelAndPrintNonDeleted(const int id,char* buffer){
		
	int skip=0; 
	int  buffer_ptr=0;
	int field_ptr;
	char field[255];
	
  for(size_t i=0;i < MODELLEN; i++)
	{
		field_ptr=0;
		while(buffer[buffer_ptr]!=4)
			{
				if(buffer[buffer_ptr]!=3 )
				{
					field[field_ptr]=buffer[buffer_ptr];
					field_ptr++;
				}
				buffer_ptr++;
				
			}
			buffer_ptr++;
			field[field_ptr]=0;
			if(strcmp(todoModel[i][0],"state")==0 && strcmp(field,"deleted")==0)
					skip=1;
					
			if(i==MODELLEN-1 || i==MODELLEN-2)
			{
				strcpy(todoModel[i][1],convertTime(field));
			}
			else
			strcpy(todoModel[i][1],field);
	}
	if(skip==0)
	{
		printf("todo°%d)\n",id);
		for(size_t i=0 ;i < MODELLEN;i++)
		{
				printf("	%s: %s\n",todoModel[i][0],todoModel[i][1]);
		}	
	}
	return skip;
}
char * convertTime(char * time){
  errno=0;
  time_t simp_time=atoi(time);
  if(errno || simp_time==0)
	  return  "unrecognized time format";
  struct tm* broken_time=localtime(&simp_time);
  char *readableTime=asctime(broken_time);
  readableTime[strlen(readableTime)-1]=0;
  return readableTime;  
}
	//export utils
int containsSpaces(const  char * buffer){
	for(size_t i=0;i<strlen(buffer);i++)
	{
		if(buffer[i]==32 || buffer[i]=='\t' )
			return  1;
	}
	return 0;
}
void parseModelAndExport(const  int id,char * buffer, FILE* output){
	fprintf(output,"\ntodo°%d)\n",id);
	
	int  buffer_ptr=0;
	int field_ptr;
	char field[255];
	
  for(size_t i=1;i <MODELLEN; i++)
	{
		
		field_ptr=0;
		while(buffer[buffer_ptr]!=4)
			{
				if(buffer[buffer_ptr]!=3 )
				{
					field[field_ptr]=buffer[buffer_ptr];
					field_ptr++;
				}
				buffer_ptr++;
			}
			buffer_ptr++;
			field[field_ptr]=0;
			if(i==MODELLEN-1 || i==MODELLEN-2)
			{
				fprintf(output,"	%s: %s\n",todoModel[i][0],convertTime(field));
			}
			else
			fprintf(output,"	%s: %s\n",todoModel[i][0],field);
	}
}
	//delete utils
void deleteTodoAt(int id,FILE* stream){
	
	size_t currentID=0;
	size_t n=0;
	size_t nbchars;
	size_t  lineLength;
	char* buffer=NULL;
	
	do
	{
		lineLength=getline(&buffer,&n,stream);
		currentID++;
	}while(id>currentID );
	buffer[lineLength-1]=0;
	fseek(stream,-lineLength,SEEK_CUR);
	
	int  buffer_ptr=0;
	int field_ptr;
	char field[255];
  for(size_t i=0;i < MODELLEN; i++)
	{
		field_ptr=0;
		while(buffer[buffer_ptr]!=4)
			{
				if(buffer[buffer_ptr]!=3 )
				{
					field[field_ptr]=buffer[buffer_ptr];
					field_ptr++;
				}
				buffer_ptr++;
				
			}
			buffer_ptr++;
			field[field_ptr]=0;
			if(strcmp(todoModel[i][0],"state")==0) 
				strcpy(todoModel[i][1],"deleted");
			else	
			strcpy(todoModel[i][1],field);
	}
	
	do
	{
		printf("deleting  todo at  id=%d  whose title=%s  [Y/n]",currentID,todoModel[0][1]);
		n=0;
		buffer=NULL;
		nbchars=getline(&buffer,&n,stdin);
		buffer[nbchars-1]=0;
	}while(strcmp(buffer,"Y")!=0 && strcmp(buffer,"n")!=0);

	if(strcmp(buffer,"Y")==0)
	{
		sprintf(todoModel[MODELLEN-2][1],"%d",time(NULL));
		serializeAndUpdateDb(stream);
		puts("delete operation completed");
	}
	else
	{
		puts("delete operation aborted");
	}
		
}
	//update utils;
void updateTodoAt(int id,FILE* stream){
	
	size_t currentID=0;
	size_t n=0;
	size_t nbchars;
	size_t  lineLength;
	char* buffer=NULL;
	
	do
	{
		lineLength=getline(&buffer,&n,stream);
		currentID++;
	}while(id>currentID );
	fseek(stream,-lineLength,SEEK_CUR);
	buffer[lineLength-1]=0;
	
	int skip=0; 
	int  buffer_ptr=0;
	int field_ptr;
	char field[255];
	
  for(size_t i=0;i < MODELLEN; i++)
	{
		field_ptr=0;
		while(buffer[buffer_ptr]!=4)
			{
				if(buffer[buffer_ptr]!=3 )
				{
					field[field_ptr]=buffer[buffer_ptr];
					field_ptr++;
				}
				buffer_ptr++;
				
			}
			buffer_ptr++;
			field[field_ptr]=0;
			
			if(i==MODELLEN-1 || i==MODELLEN-2)
			{
				strcpy(todoModel[i][1],field);
			}
			else
			strcpy(todoModel[i][1],field);
	}
	printf("\ntodo°%d)\n",id);
	for(size_t i=0;i <MODELLEN;	i++)
	{
		printf("	%s: %s\n",todoModel[i][0],todoModel[i][1]);
	}	
	puts("update fields");
	n=0;
	nbchars;
	lineLength;
	buffer=NULL;
	int option;
	for(size_t i=0;i <MODELLEN-2; i++){
	
	do{
		if(strcmp(todoModel[i][0],"state")==0)
		{
			do
			{
				puts("choose an option from the list below: Default 0");
				for(size_t  i=0; i<STATELEN;i++)
				{
					printf("%d=%s ",i,state[i]) ;
				}
				puts("");
				oneByOne(todoModel[i][0],30);
				printf(": ");
				nbchars=getline(&buffer,&n,stdin);
				buffer[nbchars-1]=0;
				option=isAnumber(buffer,nbchars-1);
			}while( option < 0 || option >=STATELEN );
		}
		else
		{
				oneByOne(todoModel[i][0],30);
				printf(": ");
				nbchars=getline(&buffer,&n,stdin);
		}
		}while(nbchars>255);
		buffer[nbchars-1]=0;
		if(strcmp(todoModel[i][0],"state")==0)
		strcpy(todoModel[i][1],state[option]);
		else
		strcpy(todoModel[i][1],buffer);
	}
	n=0;
	nbchars;
	lineLength;
	buffer=NULL;
	do{
	puts("confirm to complete the update procedure [Y/n]");
	nbchars=getline(&buffer,&n,stdin);
	buffer[nbchars-1]=0;
	}while(strcmp(buffer,"Y") && strcmp(buffer,"n")  );
	if(strcmp(buffer,"Y")==0)
	{
			sprintf(todoModel[MODELLEN-2][1],"%d",time(NULL));
			serializeAndUpdateDb(stream);
			puts("updated operation completed");
	}
	else
	{
		puts("update operation aborted");
	}
	
	
}
void serializeAndUpdateDb(FILE* DB){
	
	for(size_t i=0;i < MODELLEN-2;  i++)
	{
		addPeriod(todoModel[i][1]); 
		fprintf(DB,"%s",todoModel[i][1]);	
	}
	fprintf(DB,"%s\004",todoModel[MODELLEN-2][1]);
	fprintf(DB,"%s\004\n",todoModel[MODELLEN-1][1]);
}

