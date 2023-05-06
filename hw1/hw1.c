#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>


//definitions
#define  _CRT_SECURE_NO_WARNINGS
#define MAX_CHARS 1024
#define MAX_ARGS 64
#define MAX_COMMAND_LENGTHZ 5

//process functions
typedef struct Process
{
	int pid;
	int bBackground;
	char command[MAX_CHARS];
	struct Process* next;
} Process;

// Declaration
Process* proc; 

// Process functions
void createProc(int pid, char line[],int bBackground)
{

	proc = (Process*)malloc(sizeof(Process));
	proc->bBackground = bBackground; 
	proc->pid = pid;
	strcpy(proc->command, line);
	proc->next = NULL;

}

void addProcess(int pid, char line[], int bBackground)
{
	if(proc==NULL)
	{
		createProc(pid,line, bBackground);
		return;
	}

	Process* temp = (Process*)malloc(sizeof(Process));
	temp->bBackground = bBackground;
	strcpy(temp->command, line);
	temp->pid = pid;
	temp->next = NULL;

	Process* curr = proc;
	while (curr->next != NULL)
		curr = curr->next;

	curr->next = temp;

}

void deleteProcess(int pid)
{
	Process* temp = proc;
	while (proc->pid != pid && temp != NULL)
	{
		temp = temp->next;
	}

	if (temp != NULL)
	{
		temp = proc;
		proc = proc->next;
		free(temp);
	}
}

int getProcLength()
{
	Process* temp = proc;
	int c = 0;
	while (temp != NULL)
	{
		c++;
		temp = temp->next;
	}
	return c;
}

void terminateZombies()
{
	int pid;
	int* status;
	while ((pid = waitpid(-1, status, WNOHANG)) > 0)
	{
		printf("hw1shell: pid %d finished\n", pid);
		deleteProcess(pid);
	}

}

void printJobs()
{
	Process* temp = proc;
	while (temp != NULL)
	{
		printf("%d\t%s\n", temp->pid, temp->command);
		temp = temp->next;
	}

}


// functions in main
void executeCat(char argv[64][1024]); /*unneeded*/
void executeCommand(int index, char argv[64][1024]); /*unneeded*/

void process(char line[]);
long getFileLen(FILE* file);
void readFile(FILE* file, char* buffer);


long getFileLen(FILE* file)
{
	fseek(file, 0L, SEEK_END);
	long fileLen = ftell(file);
	rewind(file);

	return fileLen;
}


void readFile(FILE* file, char* buffer)
{
	long fileLen = getFileLen(file);
	int bufferCount = 0;
	while (!feof(file))
	{
		char ch = fgetc(file);
		buffer[bufferCount] = ch;
		if (bufferCount == fileLen - 1)
			break;
		bufferCount++;
	}
	fclose(file);
	buffer[fileLen] = '\0';
}

void executeCommand(int index, char argv[MAX_ARGS][MAX_CHARS])
{
	switch (index)
	{
	case 0://exit	
		while (getProcLength() != 0)
			terminateZombies();
		break;
	case 1://cd
		if (!strcmp(argv[1], ""))
		{
			if(chdir(argv[0])==-1)
				printf("hw1shell: invalid command\n");
		}
		else
		{
			//ERROR - Invalid cd command
			printf("hw1shell: invalid command\n");
		}
		break;
	case 2://jobs
		printJobs();
		break;

	}


}

void executeCat(char argv[64][1024])
{
	char* filePath = argv[0];
	char* filePath2 = NULL;  // file to be written to 

	FILE* file = fopen(filePath, "r");
	//check error of open
	if (file == NULL)
	{
		// ERROR - opening file 
	}
	long fileLen = getFileLen(file);
	char* buffer = (char*)malloc(fileLen + 1); // buffer to load file data into

	FILE* file2 = NULL;

	readFile(file, buffer);
	//determine whether to copy or read (and print) a file
	int bCopy = 0;

	if (!strcmp(argv[1], ">"))
		bCopy = 1;

	if (bCopy)
	{
		//copy
		filePath2 = argv[2];
		file2 = fopen(filePath2, "w");
		if (file2 == NULL)
		{
			// ERROR - opening file 
		}

		// writing buffer into file2
		fprintf(file2, buffer); 
		fclose(file2);
	}
	else
	{
		//print
		printf("%s\n", buffer);
	}
	free(buffer);
}


void process(char line[])
{
	char commands[3][5] = { "exit","cd","jobs"};

	char argv[MAX_ARGS][MAX_CHARS] = { NULL }; // initializing empty arguments
	char command[5];

	int len = strlen(line);
	line[len - 1] = '\0'; // removing \n from the command line

	char tokenLine[MAX_CHARS];
	strcpy(tokenLine, line);

	char* token = strtok(tokenLine, " ");
	if (token != NULL)
	{
		strcpy(command, token);
		strcpy(argv[0],token);
		token = strtok(NULL, " ");
	}
	else
	{
		// No command to analyze
		return; // break out of function (and request another input in the main function loop)
	}

	// copying all the parameters to argv
	int paramCount = 1;

	while (token != NULL)
	{
		if (paramCount >= MAX_ARGS)
		{
			//ERROR - more than 64 parameters!
			break;
		}

		strcpy(argv[paramCount], token);

		token = strtok(NULL, " ");
		paramCount++;
	}

	// command processing for shell specific commands
	//check if command is non-existent
	int commandIndex = -1;
	for (int i = 0; i < 5; i++)
	{
		if (!strcmp(commands[i], command))
		{
			commandIndex = i;
			executeCommand(commandIndex, argv);
			return;
		}
	}
	// other commands - exec
	char* backgroundPtr = strstr(line, "&");
	int bBackground = backgroundPtr != NULL; // boolean for background process


	if (getProcLength() > 4 && bBackground)
	{
		// too many background proc
		printf("hw1shell: too many background commands running\n");
		return;
	}

	// add new node
	int pid = fork();
	int* status;
	if (pid > 0)
	{
		//parent
		if (bBackground)
		{
			//background parent
			addProcess(pid, line, bBackground);
			printf("hw1shell: pid %d started\n", pid);
		}
		else
		{
			//foreground parent
			waitpid(pid, status, 0);
		}
	}
	if (pid == 0)
	{
		//child

		if (execvp(command, argv) == -1)
		{
			printf("hw1shell: invalid command\n");
			printf("hw1shell: execvp failed, errno is %d\n", errno);
		}
		exit(0);
	}
	if (pid < 0)
	{
		// fork proc creation failure
		printf("hw1shell: fork failed, errno is %d\n", errno);
	}

	terminateZombies();
}

int main()
{
	char input[MAX_CHARS];
	proc = NULL;

	while (1)
	{
		printf("hw1shell$ ");
		fgets(input, MAX_CHARS, stdin);
		process(input);
	}

}
































































































































































































































































































































































































































































































































