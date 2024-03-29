#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helpfunctions.h"
#include <pthread.h>
#include <unistd.h>
#include <time.h>

// general defines // change
#define NUM_THREADS 4096
#define NUM_COUNTERS 100
#define MAX_LINE 1024
#define MAX_COMMANDS 128 //remove?
#define MAX_COMMAND_LENGTHZ 18
#define NUM_COMMANDS 6

// COMMAND DEFINITIONS
#define MSLEEP 0
#define INCREMENT 1
#define DECREMENT 2
#define DISPATCHER_WAIT 3
#define DISPATCHER_MSLEEP 4
#define REPEAT 5

// DEBUGGING COMMENT
/*Main debugging happens in: parseJob and threadFunc with: the execution command the thread's workingNumber, etc.*/


//structures
typedef struct Job{
	int commands[MAX_COMMANDS];//command indices
	int args[MAX_COMMANDS];//command arguments
	int index;//job line index
	int length;//length of commands in job
	clock_t startTime;//starTime of job for turnaround calculation
}Job;

typedef struct Thread{ // this thread structure is used to keep track of the progress of the thread, and identify it easily.
	pthread_t threadId;
	int bWorking;
	int workerNumber;
}Thread;

// structure for entering necessary arguments into the threads' function.
typedef struct funcArgument{ // Used to pass onto the thread function to end the thread and process the job throughout.
	Thread* thread;
	Job* job;
}funcArgument;

// functions

//function to parse and execute the commands in the command line
void parseJob(char line[], Job* job); // Parses the line by separating the commands and arguments and indexing them and storing them in a Job.
void loadLines();// loads each line to a buffer and stores it in 'lines'
void loadJobs();// parses every job using parseJob.
void executeJobs();// Executes jobs using threading(threadFunc). It also handles dispatcher calls on the main thread.
int getNumLines(); // gets the number of lines the command file has.
// function helpers
void printStats(); // creates stats.txt file for inserting calculations made inside it regarding the runtime of the program.
// intializing and accessing log and counter files
void initCounterFiles(); // creates the counter files.
void incrementToFile(int increment, int counterFileIndex); // increments a counter file
void startThreadLog(FILE* fp,const int workerNumber, const int jobIndex); // the thread creates a log file for itself, and writes a start log line into it.
void endThreadLog(FILE* fp, const int workerNumber, const int jobIndex); // the thread creates a log file for itself, and writes an end log line into it.

// thread related functions
void* threadFunc(void* ptrVoidArgs); // used to execute commands on general threads.
int getWorker(); // returns available worker thread identification number
void startThread(int workerNumber,Job* job); // sets up the argument to be inserted into the thread function for this specific job.
void waitForThreads(); // dispatcher waits until all threads are done.

// global variables

// constant 
char commandList[NUM_COMMANDS][MAX_COMMAND_LENGTHZ]={"msleep", "increment", "decrement","repeat","dispatcher_msleep", "dispatcher_wait"}; // used to index all the commands in parseJob

// typical no-pointer variables, and strings(exception to no-pointer rule).
clock_t currentTime; // used to give time spanned since running hw2
char* filePath; // stores the path of the command file 
int numLines = 0; // keeps track of the number of lines in the command file.
int numThreads, numCounters, bLog; // num_threads, numb_counter log_enabled

// arrays of string, structures.
// jobs and threads are made global to easily access them throughout all functions. lines is used for logging and debugging.
Thread* threads;
Job* jobs;
char** lines; 

// functions' expansions
int getWorker() // returns available worker thread
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


void startThread(int workerNumber,Job* job)// sets up the argument to be inserted into the thread function for this specific job.
{
	funcArgument* ptrArgs = (funcArgument*) malloc(sizeof(funcArgument));
	
	ptrArgs->job = job;
	ptrArgs->thread = &threads[workerNumber];

	threads[workerNumber].workerNumber = workerNumber;
	threads[workerNumber].bWorking =1;
	pthread_create(&threads[workerNumber].threadId,NULL,threadFunc,(void*)ptrArgs);
}

void waitForThreads()// dispatcher waits until all threads are done.
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


void startThreadLog(FILE* fp,const int workerNumber, const int jobIndex) // the thread creates a log file for itself, and writes a start log line into it.
{
	if(!bLog)
		return;
	// constructing the file name
	char fileName[15] = "thread";
	char workerNumStr[5];
	int bFileExists =0;
	sprintf(workerNumStr, "%d", workerNumber);
	strcat(fileName,workerNumStr);
	strcat(fileName,".txt");

	// check if file exists, and add a new line in case it does. 
	fp = fopen(fileName, "r");
	if(fp!=NULL)
	{
		fclose(fp);
		bFileExists=1;
	}
	fp = fopen(fileName,"a");
	if(fp==NULL)
	{
		printf("[ERROR] opening file failed!\n");
		exit(-1);
	}
	if (bFileExists)
		fprintf(fp,"\n"); // add new line in case the file is already made (and supposedly has been written to).
	if(fgetc(fp)!='\0')
	//printf("[Time Log START]- fileName:%s - time:%ldms\n",fileName, currentTime); //debug
	fprintf(fp,"TIME %lld: START job %s\n",currentTime,lines[jobIndex]);
	fclose(fp);

}

void endThreadLog(FILE* fp, const int workerNumber, const int jobIndex)// the thread creates a log file for itself, and writes an end log line into it.
{
	if(!bLog)
		return;
	// constructing the file name
	char fileName[15] = "thread";
	char workerNumStr[5];
	
	sprintf(workerNumStr, "%d", workerNumber);
	strcat(fileName,workerNumStr);
	strcat(fileName,".txt");
	fp = fopen(fileName,"a");
	if(fp==NULL)
	{
		printf("[ERROR] opening file failed!\n");
		exit(-1);
	}

	//printf("[Time Log END  ] - fileName:%s - time:%ldms\n",fileName, currentTime); //debug
	fprintf(fp,"TIME %lld: END job %s",currentTime,lines[jobIndex]);
	fclose(fp);
}

void* threadFunc(void* ptrVoidArgs)// used to execute commands on general threads.
{
	currentTime = clock();
	FILE* fp; 

	// loading arguments
	funcArgument* fArgs = (funcArgument*) ptrVoidArgs;
	Thread* currentThread = fArgs->thread;
	Job* job = fArgs->job;

	int* commands = job->commands;
	int* args = job->args;

	if(currentThread!=NULL)
	{
		job->startTime = currentTime;
		startThreadLog(fp,currentThread->workerNumber, job->index);
	}

	//repeat arguments in case we have a repeat command
	funcArgument* repeatArgs = NULL;

	int length = job->length;
	for(int c=0;c<length;c++)
	{
		// execute commands
		//
		//debug
		//if(currentThread!=NULL)
		//{
		//	printf("[WORKER:%d]Executing Command:%s - Argument:%d - Line:%d\n",currentThread->workerNumber,commandList[commands[c]],args[c],job->index); // debug
		//}
		//else
		//{
		//	printf("[WORKER:REPEAT]Executing Command:%s - Argument:%d - Line:%d\n",commandList[commands[c]],args[c],job->index); // debug
		//}
		

		if(commands[c] == 0) // msleep
		{
			//printf("sleep!\n"); // debug
			unsigned int microseconds = args[c]*1000; // milli to micro seconds
			usleep(microseconds);
			//printf("done sleeping!\n");
		}

		if(commands[c]==1) // increment
			incrementToFile(1, args[c]);
		if(commands[c]==2) // decrement
			incrementToFile(-1, args[c]);

		if(commands[c]==3) // repeat
		{
			repeatArgs = (funcArgument*)malloc(sizeof(funcArgument)); //possibly unnecessary malloc
			repeatArgs->thread = NULL; // no thread given to repeat commands
			repeatArgs->job = (Job*)malloc(sizeof(Job));
			Job* tempJob = repeatArgs->job; // setting next job
			
			//copying the next commands' command strings and arguments in the job
			int i=c;
			for(;i<length-1;i++)
			{
				tempJob->commands[i-c] = commands[i+1];
				tempJob->args[i-c] = args[i+1];
			}
			tempJob->length = i-c;
			tempJob->index = job->index;
			                                                                                        
			for (i=0;i<args[c];i++)
				threadFunc((void*)repeatArgs);
			break;
		}

	}

	// end thread
	if(currentThread!=NULL)
	{
		currentTime = clock();
		free(ptrVoidArgs);
		if(repeatArgs!=NULL)
		{
			free(repeatArgs->job);
			free(repeatArgs);
		}
		endThreadLog(fp,currentThread->workerNumber, job->index);
		currentThread->bWorking =0;
	}
	return NULL;
}

void loadLines()// loads each line to a buffer and stores it in 'lines'
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
	int jobCount = 0;
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
			strcpy(lines[jobCount],buffer);
			jobCount++;
		}
		//printf("%s-length: %d, count: %d\n",buffer, bufferCount,chrCount); // debug
		chrCount += bufferCount;

		strcpy(buffer,"");
	}
	fclose(fp);
}

void loadJobs()// parses every job using parseJob.
{
	for(int i=0;i<numLines&&strcmp(lines[i],"");i++)
	{
		jobs[i].index = i;
		parseJob(lines[i], &jobs[i]);
	}
}


void initCounterFiles()// creates the counter files.
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

void incrementToFile(int increment, int counterFileIndex)// increments a counter file
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

void parseJob(char line[], Job* job) // Parses the line by separating the commands and arguments and indexing them and storing them in a Job.
{
	char tempLine[MAX_LINE];
	char* temp; // pointer for tempLine
	char* commandStr; // pointer to command string
	char* argStr; // pointer to argument string
	
	int arg;

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
			job->commands[c] = 5;
        		//strcpy(job->commands[c], commandStr);
        		job->args[c] = -1;
        	                                         
			//printf("token:%s(index:%d)-arg:%d\n",commandStr,job->commands[c], -1); // debug
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
		
		//check for errors
		if(arg < 0)
		{
			printf("Error! Invalid argument!\n");

		}
		int commandIndex=-1;
		for(int i =0;i<NUM_COMMANDS;i++)
		{
			if(!strcmp(commandStr,commandList[i]))
			{
				
				commandIndex = i;
				break;
			}
		}
		if(commandIndex==-1)
		{
			printf("Error! %s is not a supported command!\n", commandStr);
			exit(-1);
		}

		// index command and argument
		job->commands[c] = commandIndex;
		job->args[c] = arg;
		//strcpy(job->commands[c], commandStr);

		//printf("token:%s(index:%d)-arg:%d\n",commandStr,job->commands[c], arg); // debug
		c++;
		tokenCmd = strtok(NULL, ";");
	}
	//strcpy(job->commands[c], "");
	job->length = c;
	
	//printf("done parsing line!\n"); // debug
}

int getNumLines()// gets the number of lines the command file has.
{
	int c = 0;
	FILE* fp = fopen(filePath, "r");
	if(fp==NULL)
	{
		printf("[ERROR] opening file failed!\n");
		exit(-1);
	}
	int fileLen = getFileLen(fp);
	int bufferCount =0;
	while (!feof(fp))
	{
		char ch = fgetc(fp);
		if (bufferCount == fileLen) 
		{
			c++;
			break;
		}
		if(ch=='\n')
		{
			c++;
		}
	                                                                                                 
		if (fileLen >= MAX_LINE)
		{
			printf("[ERROR] line is too large!\n");
			exit(-1);
		}
	                                                                                                  
		bufferCount++;
	}
	fclose(fp);
	return c;
}


void executeJobs()// Executes jobs using threading(threadFunc). It also handles dispatcher calls on the main thread.
{
	//printf("executing jobs!\n");// debug
	int workerNumber;
	//char commands[MAX_COMMANDS][MAX_COMMAND_LENGTHZ];
	int* commands;
       	int* args;
	for(int i=0;i<numLines;i++)
	{

		commands = jobs[i].commands;
		args = jobs[i].args;
		if(jobs[i].length==0)
			break;
		if(commands[0]==4) // dispatcher_msleep
                {
                	//printf("[WORKER:%d]Executing Command:dispatcher_msleep - Argument:%d - Line:%d\n",-1,args[0],i); // debug
                	unsigned int microseconds = args[i]*1000; // milli to micro seconds
                	usleep(microseconds);
                	continue;
                }

		if(commands[0]==5) // dispatcher_wait
		{
			//printf("[WORKER:%d]Executing Command:dispatcher_wait - Argument:%d - Line:%d\n",-1,args[0],i); // debug
			waitForThreads();
			//printf("command after thread waiting:%s",commands[i]); // debug
			continue;
		}

		workerNumber = getWorker(); // find worker
		startThread(workerNumber,&jobs[i]);
	}
}


void printStats()// creates stats.txt file for inserting calculations made inside it regarding the runtime of the program.
{
	clock_t min, max, sum;
	double avg;
	min = jobs[0].startTime;
	max = jobs[0].startTime;
	sum=0;
	int i=0;
	for(;i<numLines&&jobs[i].length!=0;i++)
	{
		if(jobs[i].commands[0] >=4)//in case of dispatcher
			continue;

		clock_t current = jobs[i].startTime;
		//printf("job: %s\n job time:%lld\n",lines[i],current); // debug
		//printf("current stat: %lld\n",current); // debug
		sum+=current;
		if(current < min)
			min = current;
		if(current>max)
			max=current;
	}
	avg = (double)sum/(double)i;
	FILE* fp = fopen("stats.txt","w");
	if(fp==NULL)
	{
		printf("[ERROR] opening file failed!\n");
		exit(-1);
	}
	fprintf(fp, "total running time: %lld milliseconds\n",currentTime);
	fprintf(fp, "sum of jobs turnaround time: %lld milliseconds\n",sum);
	fprintf(fp, "min job turnaround time: %lld milliseconds\n",min);
	fprintf(fp, "average job turnaround time: %f milliseconds\n",avg);
	fprintf(fp, "max job turnaround time: %lld milliseconds\n",max);
	fclose(fp);
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
		threads[i].threadId = -1;
		threads[i].bWorking = 0;
	}
	
	initCounterFiles();
	numLines = getNumLines();
	
	// load and parse commands
	jobs = (Job*)malloc(sizeof(Job)*numLines);
	lines = (char**) malloc(numLines*sizeof(char*));

	for(int i=0;i<numLines;i++) // initializing global arrays
	{
		jobs[i].length = 0; // job
		lines[i] = (char*)malloc(MAX_LINE); // lines
		strcpy(lines[i],"");
	}

	loadLines();
	loadJobs();

	//debug
	//for(int i=0;i<numLines;i++)
	//{
	//	//debug
	//	int countJobCommand=0; 
	//	printf("DEBUG:");
	//	for(;countJobCommand<jobs[i].length;countJobCommand++)
	//	{
	//		printf("%d: %s - ",countJobCommand,jobs[i].commands[countJobCommand]);
	//		countJobCommand++;
	//	}
	//	printf("\n");
	//}

	executeJobs();
	waitForThreads(); // remove? should be included in command file?
	
	currentTime = clock(); // total time

	// calculate turnaround times:
	printStats();

	// freeing memory
	for(int i=0;i<numLines;i++)
	{
		free(lines[i]);
	}
	free(lines);
	free(jobs);
	free(threads);
}
