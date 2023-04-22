#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <malloc.h>
#include <time.h>

#include "todo.h"
#include "utility.h"


typedef struct timespec Time;

FILE * file;

/*main features*/
void add(){

	fillModel();
		
	errno=0;
    file=fopen(FILENAME,"a");
    if(file==NULL || errno){
      fprintf(stderr,"ERROR: %s\n",strerror(errno));
      return;
    }
	
	serializeAndWriteToDb(file);
	fclose(file);
}//add a new daily task to the list
void showAll(){
  errno=0;
  file=fopen(FILENAME,"r");
  if(file==NULL || errno){
    fprintf(stderr,"ERROR: %s\n",strerror(errno));
    return;
  }
  int id=1;
  size_t n=0;
  char *buffer=NULL;
  system("clear");
  while(getline(&buffer,&n,file)!=EOF)
  {
	  //display each todo one at a time by keypress
	parseModelAndPrint(id,buffer);
	puts("");
    id++;
  }
  puts("");
  fclose(file);
}
void show(){
errno=0;
  size_t size=0;
  file=fopen(FILENAME,"r");
  if(file==NULL || errno){
    fprintf(stderr,"ERROR: %s\n",strerror(errno));
    return;
  }
  int id=1;
  size_t n=0;
  char *buffer=NULL;
  system("clear");
  int deleted_todos=0;
  while(getline(&buffer,&n,file)!=EOF)
  {
	  //display each todo one at a time by keypress
    deleted_todos+=parseModelAndPrintNonDeleted(id,buffer);
    id++;
	puts("");
  }
  printf("you have %d deleted todos\n\n",deleted_todos);
  fclose(file);
}//show tasks corresponding to a specific day / id / title /;//show tasks sorted by datetime//
void update(){
    const size_t nbtodos=countAvailableTodos(FILENAME);
if(nbtodos==0){	
	  if(!errno)
	  puts("todos inventory currently empty");
	  return ;
  }
  int updatedID;
 char * buffer = NULL;
 size_t n=0;
 size_t nbchars;

  do{
	errno=0;
	printf("choose an id  in the range of [1-%d]:  ",nbtodos);
	nbchars=getline(&buffer,&n,stdin);
	
	buffer[nbchars-1]=0;
	updatedID=atoi(buffer);
	if(updatedID>nbtodos || updatedID<=0 || notAnumber(buffer,nbchars-1))
	puts("ID out of range");	
	if(errno)
	puts(strerror(errno));	
  }
  while(updatedID>nbtodos || updatedID<=0 || errno ||  notAnumber(buffer,nbchars-1));
  
	errno=0;
    FILE *stream=fopen(FILENAME,"r+");
    if(stream==NULL || errno){
      fprintf(stderr,"ERROR: %s\n",strerror(errno));
      return ;
    }
	updateTodoAt(updatedID,stream);
	fclose(stream);
}
void delete(){

  const size_t nbtodos=countAvailableTodos(FILENAME);
if(nbtodos==0){	
	  if(!errno)
	  puts("todos inventory currently empty");
	  return ;
  }
  int deletedID;
 char * buffer = NULL;
 size_t n=0;
 size_t nbchars;

  do{
	errno=0;
	printf("choose an id  in the range of [1-%d]:  ",nbtodos);
	nbchars=getline(&buffer,&n,stdin);
	
	buffer[nbchars-1]=0;
	deletedID=atoi(buffer);
	if(deletedID>nbtodos || deletedID<=0 || notAnumber(buffer,nbchars-1))
	puts("ID out of range");	
	if(errno)
	puts(strerror(errno));	
  }
  while(deletedID>nbtodos || deletedID<=0 || errno ||  notAnumber(buffer,nbchars-1));
  
	errno=0;
    FILE *stream=fopen(FILENAME,"r+");
    if(stream==NULL || errno){
      fprintf(stderr,"ERROR: %s\n",strerror(errno));
      return ;
    }
	deleteTodoAt(deletedID,stream);
	fclose(stream);
}
void export(){
	char output[32];
{
char * buffer;
size_t  n;
int nbchars;
int emptyspaces;
do
{
	buffer=NULL;
	n=0;
	emptyspaces=0;
	printf("FILENAME:");
	
	nbchars=getline(&buffer,&n,stdin);
	char ignored[nbchars];
	
	if(nbchars>32)
		puts("filename must be less than 32 characters long!");
	if(containsSpaces(buffer))	
			{
				emptyspaces=1;
				puts("filename must not contain empty space characters!");
			}
}while( emptyspaces || nbchars>32);
strcpy(output,buffer);
output[nbchars-1]=0;
}
  errno=0;
  file=fopen(FILENAME,"r");
  if(file==NULL || errno){
    fprintf(stderr,"ERROR: %s\n",strerror(errno));
    return;
  }
  errno=0;
  FILE * stream=fopen(output,"a");
  if(file==NULL || errno){
    fprintf(stderr,"ERROR: %s\n",strerror(errno));
    return;
  }
  int id=1;
  size_t n=0;
  char *buffer=NULL;
  system("clear");
  while(getline(&buffer,&n,file)!=EOF)
  {
	  //display each todo one at a time by keypress
    parseModelAndExport(id,buffer,stream);
    id++;
  }
  fclose(stream);
  fclose(file);
}


