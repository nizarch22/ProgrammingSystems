#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helpfunctions.h"
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 4096
#define NUM_COUNTERS 100
#define MAX_LINE 1024
#define MAX_COMMANDS 128 //remove?
#define MAX_COMMAND_LENGTHZ 18
#define NUM_COMMANDS 6

/*COMMENTS*/

/* 
 * considerations to let char line[], char commands[MAX_COMMANDS][MAX_COMMAND_LENGTHZ], int args[MAX_COMMANDS]
 * be global variables fail as the command 'repeat' requires recursive function executeCommandLine
 *  */

/*
 * another consideration to make is to index command by integer numbers, rather than to use the slow char commands[] and its functions
 * 
 * */



// functions

//function to parse and execute the commands in the command line
void loadAndExecuteCommandLine();
void parseCommandLine(char line[], char commands[MAX_COMMANDS][MAX_COMMAND_LENGTHZ], int args[MAX_COMMANDS]);
void executeCommandLine(char line[]);

// intializing and accessing log and counter files
void initCounterFiles();
void initLogFiles();
void incrementToFile(int increment, int counterFileIndex);

// thread related functions
void* threadFunc(void* ptrVoidArgs);
int getWorker(); // 'returns' available worker thread
void startThread(int workerNumber,char* command, int arg);
void waitForThreads();


//structures

// Thread function to indicate when a thread is working.
typedef struct Thread{
	pthread_t threadId;
	int bWorking;
}Thread;

// structure for entering necessary arguments into the threads' function.
typedef struct funcArgument{
	Thread* thread;
	char command[MAX_COMMAND_LENGTHZ];
	int arg;
}funcArgument;

// global variables
Thread* threads;
char* filePath;
char commandList[NUM_COMMANDS][MAX_COMMAND_LENGTHZ]={"msleep", "increment", "decrement","dispatcher_wait","dispatcher_msleep","repeat"};
int numThreads, numCounters, bLog; // num_threads, numb_counter log_enabled
char currentLine[MAX_LINE];

// functions' expansions

int getWorker() // 'returns' available worker thread
{
	int bAvailable = 0;
	int index = -1;
	while(!bAvailable)
	{

		for(int i=0;i<numThreads;i++)
		{
			if (!threads[i].bWorking)
			{
				bAvailable = 1;
				index = i;
				break;
			}
		}
	}
	//printf("got a worker: %d\n",index); // debug
	return index;
}

void initLogFiles()
{
}


void startThread(int workerNumber,char* command, int arg )
{
	funcArgument* ptrArgs = (funcArgument*) malloc(sizeof(funcArgument));
	strcpy(ptrArgs->command,command);
	ptrArgs->arg = arg;
	ptrArgs->thread = &threads[workerNumber];

	threads[workerNumber].bWorking =1;
	pthread_create(&threads[workerNumber].threadId,NULL,threadFunc,(void*)ptrArgs);
}

void waitForThreads()
{
	//printf("Waiting for threads!\n"); // debug
	for(int i=0;i<numThreads;i++)
	{
		if(threads[i].bWorking)
		{
			pthread_join(threads[i].threadId, NULL);
			//printf("bWorking: %d", threads[i].bWorking); // debug
		}
	}
	//printf("Done waiting for threads!\n"); // debug
}


void* threadFunc(void* ptrVoidArgs)
{
	// loading arguments
	funcArgument* args = (funcArgument*) ptrVoidArgs;
	Thread* currentThread = args->thread;
	char* command = args->command;
	int arg =  args->arg;
	// execute commands

	//sleep
	if(!strcmp(command,"msleep")) 
	{
		printf("sleep!\n"); // debug
		unsigned int microseconds = arg*1000; // milli to micro seconds
		usleep(microseconds);
		printf("done sleeping!\n");
	}
	                                                                                                          
	//increment & decrement
	int amount = 1;
	if(!strcmp(command,"decrement"))
		amount = -1;
	                                                                                                          
	if(!strcmp(command,"decrement")||!strcmp(command,"increment"))
	{
		printf("increment!\n"); //open file, read number, atoi, increase write to file, close // debug
		incrementToFile(amount, arg);
	}
	
	// end thread
	currentThread->bWorking =0;
	free(args);

	return NULL;
}

void loadAndExecuteCommandLine() // loads each line to a buffer and passes that to executeCommandLine(line[]) function
{
	// parse command file - line by line
	char buffer[MAX_LINE]; // line string
	long long chrCount = 0;
	FILE* fp = fopen(filePath, "r");
	
	if(fp==NULL)
	{
		printf("[ERROR] opening file failed!\n");
		exit(-1);
	}
	long long fileLen = getFileLen(fp);
	long lineCount = 0;
	while(chrCount!=fileLen)
	{
		long long bufferCount =0;
		while (!feof(fp))
		{
			char ch = fgetc(fp);
			buffer[bufferCount] = ch;
			if (bufferCount == fileLen) //useless - this is useless, but will be kept for continuity.
				break;
			if(ch=='\n')
			{
				chrCount++;
				break;
			}
	                                                                                                         
			if (fileLen >= MAX_LINE)
			{
				printf("[ERROR] line is too large!\n");
				exit(-1);
			}
	                                                                                                          
			bufferCount++;
		}
	                                                                                                          
		buffer[bufferCount] = '\0';
	         
		//execute current line commands		
		if(strcmp(buffer,""))
		{
			strcpy(currentLine,buffer);
			executeCommandLine(buffer);

		}
		//printf("%s-length: %lld, count: %lld\n",buffer, bufferCount,chrCount); debug
		chrCount += bufferCount;

		strcpy(buffer,"");
	}
	fclose(fp);
}


void initCounterFiles()
{
	char strFileName[12];
	char strPre[] = "count";                                                                                            	
	char strSuffix[] = ".txt";
	char strNum[3];
	
	FILE* fp;
	                                                                                                                     
	//initCounterFiles(numCounters);
	for(int i=0;i<numCounters;i++) {
	                                                                                                                     
		strcpy(strFileName, ""); //reset filename (also means the starting point of the concatenation is at index 0)
		// constructing filename (strFileName)
		sprintf(strNum, "%02d", i);
		strcat(strFileName,strPre);
		strcat(strFileName,strNum);
		strcat(strFileName,strSuffix);
	                                                                                                                     
		//inserting 0 into each file
		fp = fopen(strFileName, "w");
		if(fp==NULL)
		{
			printf("[ERROR] opening file failed!\n");
			exit(-1);
		}
		fprintf(fp,"%d",0);
		fclose(fp);
		
	}
}

void incrementToFile(int increment, int counterFileIndex)
{
	char strFileNum[20];
	                                             
	char strFileName[12];
	char strShortNum[3];
	sprintf(strShortNum, "%02d", counterFileIndex);
	
	// concatenating the file name 
	strFileName[0] = '\0'; // starting point of concatenation			
	strcat(strFileName, "count");
	strcat(strFileName, strShortNum);
	strcat(strFileName, ".txt");
	                                             
	//printf("file:%s\n",strFileName); // debug
	readFile(strFileName,strFileNum);
	                                                                                               
	long long num = atoll(strFileNum);

	// incrementing to the number
	num+=increment;  
	sprintf(strFileNum, "%lld", num);

	// writing to file
	FILE* fp = fopen(strFileName,"w");
	if(fp==NULL) // error checking
	{
		printf("Error while opening write file!");
		exit(-1);
	}
	fprintf(fp,"%s", strFileNum);
	fclose(fp);
}

void parseCommandLine(char line[], char commands[MAX_COMMANDS][MAX_COMMAND_LENGTHZ], int args[MAX_COMMANDS]) 
{

	char tempLine[MAX_LINE];
	char* temp; // pointer for tempLine
	char* commandStr; // pointer to command string
	char* argStr; // pointer to argument string
	
	long long arg;

	strcpy(tempLine, line);
	char* tokenCmd = strtok(tempLine, ";"); // string token for the command
	
	// skip worker
	temp = strstr(tokenCmd, "worker");
	// error checking
	if(temp!=NULL)
	{
		// worker call 
		tokenCmd = strstr(temp, " ");
		temp = strstr(line, "dispatcher");
		if(temp!=NULL)
		{
			printf("Error! dispatcher call can't be in worker job queue\n");
			exit(-1);
		}
	}
	else
	{
		//dispatcher call
		temp = strstr(line, "dispatcher");
		if(temp==NULL)
		{
			printf("Error! need to call worker to initiate worker job queue\n");
			exit(-1);
		}
	}

	// preparing token to process dispatcher call
	temp = strstr(line, "dispatcher");
	if(temp!=NULL) 
	{
		strcpy(tokenCmd, tokenCmd);
	}

	int c =0;
	while(tokenCmd!=NULL)
	{
		strcpy(tempLine,tokenCmd);
		temp = tempLine;

		while (*temp == ' ') // skip the initial spaces
		{
			temp++;	
		}
		commandStr = temp;

		temp = strchr(tempLine, '\0');
		temp--;
		while (*temp == ' ') // remove ending spaces
                {
			*temp='\0';
                	temp--;
                }
		
		temp = strchr(commandStr, ' '); // isolate command string (for commandStr)
		if(temp!=NULL)
		{
			*temp ='\0';
			temp++;
		}

		if(!strcmp(commandStr,"")) // eliminate spaces falsely detected as commands
		{
			tokenCmd = strtok(NULL, ";");
			continue;
		}
		if(!strcmp(commandStr,"dispatcher_wait")) // exception - dipatcher_wait has no argument - argument is set to -1 manually
        	{
        		strcpy(commands[c], commandStr);
        		args[c] = -1;
        	                                         
			//printf("token:%s-arg:%d\n",commandStr, -1); // debug
        	        c++;
        	        tokenCmd = strtok(NULL, ";");
        		continue;
        	}

		if(temp==NULL&&strcmp(commandStr,"dispatcher_wait")) // if not a dispatcher_wait with no argument, then it's an error
		{
			printf("Error! no argument specified in command:%s!\n",commandStr);
			exit(-1);
		}

		while (*temp == ' ') // skip the middle spaces
		{
			temp++;
		}

		argStr = temp; // reached the argument
		arg = atoi(argStr);
		//printf("token:%s-arg:%lld\n",commandStr, arg); // debug
		
		//check for errors
		if(arg < 0)
		{
			printf("Error! Invalid argument!\n");

		}
		int bCommandInvalid=1;
		for(int i =0;i<NUM_COMMANDS;i++)
		{
			if(!strcmp(commandStr,commandList[i]))
			{
				bCommandInvalid = 0;
				break;
			}
		}
		if(bCommandInvalid)
		{
			printf("Error! %s is not a supported command!\n", commandStr);
			exit(-1);
		}

		// index command and argument
		strcpy(commands[c], commandStr);
		args[c] = arg;

		c++;
		tokenCmd = strtok(NULL, ";");
	}
	strcpy(commands[c], "");
	//printf("done parsing line!\n"); // debug
}


void executeCommandLine(char line[])//, int numCommands) if file is large consider saving memory
{
	if(line == NULL)
		return;
	char repeatLine[MAX_LINE]; // to accomodate the repeat command's need to run recursively

	char commands[MAX_COMMANDS][MAX_COMMAND_LENGTHZ];
	int args [MAX_COMMANDS];
	int workerNumber;
	parseCommandLine(line, commands, args);
	
	printf("----------\nLine's parsed commands now running:\n----------\n"); // debug



	// go through the commands in the current line 
	int c =0;
	while(strcmp(commands[c],""))
	{
		printf("Executing Command:%s - Argument:%d\n",commands[c],args[c]); // debug
		if(!strcmp(commands[c],"dispatcher_msleep")) // dispatcher_msleep
		{
			//printf("dispatcher_msleep!\n"); // debug
			unsigned int microseconds = args[c]*1000; // milli to micro seconds
			usleep(microseconds);
			c++;
			continue;
		}
		if(!strcmp(commands[c],"dispatcher_wait")) // dispatcher_wait
		{
			//printf("dispatcher_wait!\n"); // debug
			waitForThreads();
			c++;
			//printf("command after thread waiting:%s",commands[c]); // debug
			continue;
		}

		if(!strcmp(commands[c],"repeat")) // repeat
		{
			//setting up in case of recursion (multiple repeats)
			//printf("repeat!\n"); // debug
			char* ptrLine = strstr(line,"repeat");

			if(ptrLine!=NULL)
			{
				ptrLine = strstr(ptrLine,";"); // go to next command
				if(ptrLine!=NULL)
				{
					ptrLine++;
					strcpy(repeatLine, "worker ");
								      
					strcat(repeatLine, ptrLine);
					//printf("repeat line:%s\n", repeatLine); // debug
				}
		                                                                                                            
				for(int k=0;k<args[c];k++) // executes the next commands x times (as specified by repeat x)
					executeCommandLine(repeatLine);
			}
			break;
		}

		// worker command from here on
		workerNumber = getWorker(); // find worker
		startThread(workerNumber,commands[c], args[c]);

		c++;
	}
	//printf("done executing line!\n"); // debug

}

int main(int argc, char* argv[])
{
	
	// initializing values
	filePath = argv[1]; 
	numThreads = atoi(argv[2]); 
	numCounters = atoi(argv[3]);
	bLog = atoi(argv[4]);
	
	//Error checking
	if (argc!=5)
	{
		printf("Incorrect number of arguments!\n");
		exit(-1);
	}
	                                                                                      
	if(numThreads > NUM_THREADS || numCounters > NUM_COUNTERS || (bLog != 1 && bLog !=0))
	{
		printf("Incorrect inputs!\n");
		exit(-1);
	}

	// create threads
	threads = (Thread*) malloc(sizeof(Thread)*numThreads);
	for(int i=0;i<numThreads;i++)
	{
		threads[i].bWorking = 0;
	}
	
	initCounterFiles();
	initLogFiles();

	loadAndExecuteCommandLine();

	waitForThreads();
	free(threads);
}
