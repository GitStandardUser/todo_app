#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <malloc.h>
#include "todo.h"

//main method implementation //
void main(){
  char c[255];  
  size_t len;
  while(1)
  {
    puts("a: add a new todo");
    puts("t: show todos");
	puts("s: show  all todos");
    puts("u: update a todo");
    puts("d: delete a todo");
    puts("e: export as text");
    puts("q: exit");
	
do{
		c[1]=0;
		scanf("%[^\n]",c);
		getchar();
		len=strlen(c);
		if(len>1)
		puts("undefined option! ");
  }while(len>1);
  
    switch (c[0]) {
      case 'a':{
        add();
        break;
      }
      case 's':{
        showAll();
        break;
      }
      case 't':{
        show();
        break;
      }
      case 'u':{
        update();
        break;
      }
      case 'd':{
        delete();
        break;
      }
      case 'e':{
        export();
        break;
      }
      case 'q':
      {
        return;
      }
      default:{
		  puts("undefined option! ");
		  break;
		  }
    }
  }
}
//long runtime stack smashing error//