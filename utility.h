//general 
int getModelLength();	
size_t countAvailableTodos(char * path);
int notAnumber(char * buffer,int len);
int isAnumber(char * buffer,int len);
int min(int a,int b);
void msleep(unsigned  int  delay );
void oneByOne(char* str, unsigned  int  delay );
	
	//add  utilities
	void fillModel();
	void serializeAndWriteToDb(FILE* db);
	void addPeriod(char * string );
	//show utilities	
	void parseModelAndPrint(const int id,char* buffer);
	int parseModelAndPrintNonDeleted(const int id,char* buffer);
	char * convertTime(char * time);
	//export utils
	int containsSpaces(const  char *);
	void parseModelAndExport(const int id,char * buffer, FILE* output);
	//delete utils
	void deleteTodoAt(int id,FILE*  stream);
	//update utils 
	void updateTodoAt(int id,FILE* stream);
	void serializeAndUpdateDb(FILE* DB);
	
	//STR LITERALS
	#define FILENAME "fstest.db"