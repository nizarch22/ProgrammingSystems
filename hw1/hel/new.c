


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

#define MAX_CHARS 1024 // max characters in line
#define MAX_ARGS 64	  // max arguments in line (incluiding command)
#define MAX_BACKGROUND_PROC 4	  // max running background processes

typedef struct Process
{
	int pid;
	int bBackground;
	char line[MAX_CHARS];
	struct Process* next;
} Process;

Process* createProc();
Process* addProc(Process* processes, Process* new_process);
Process* removeProc(Process* processes, int pid);
int getProcLen(Process* processes);
void printJobs(Process* processes);
void splitLine(char line[], char* args[]);
void parseInput(char line[], char* argv[], Process* myproc);
Process* terminateZombies(Process* processes);
Process* executeCommand(char line[], Process* processes);

//void executeCat(char* argv[]);
//
//
//
//long getFileLen(FILE* file)
//{
//	fseek(file, 0L, SEEK_END);
//	long fileLen = ftell(file);
//	rewind(file);
//
//	return fileLen;
//}
//
//
//void readFile(FILE* file, char* buffer)
//{
//	long fileLen = getFileLen(file);
//	int bufferCount = 0;
//	while (!feof(file))
//	{
//		char ch = fgetc(file);
//		buffer[bufferCount] = ch;
//		if (bufferCount == fileLen - 1)
//			break;
//		bufferCount++;
//	}
//	fclose(file);
//	buffer[fileLen] = '\0';
//}


//void executeCat(char* argv[])
//{
//	char* filePath = argv[1];
//	char* filePath2 = NULL;  // file to be written to 
//
//	FILE* file = fopen(filePath, "r");
//	//check error of open
//	if (file == NULL)
//	{
//		// ERROR - opening file 
//	}
//	long fileLen = getFileLen(file);
//	char* buffer = (char*)malloc(fileLen + 1); // buffer to load file data into
//
//	FILE* file2 = NULL;
//
//	readFile(file, buffer);
//	//determine whether to copy or read (and print) a file
//	int bCopy = 0;
//
//	if (!strcmp(argv[2], ">"))
//		bCopy = 1;
//
//	if (bCopy)
//	{
//		//copy
//		filePath2 = argv[3];
//		file2 = fopen(filePath2, "w");
//		if (file2 == NULL)
//		{
//			// ERROR - opening file 
//		}
//
//		// writing buffer into file2
//		fprintf(file2, buffer);
//		fclose(file2);
//	}
//	else
//	{
//		/*UNNEEDED*/
//		//print
//		//printf("%s\n", buffer);
//	}
//	free(buffer);
//}



Process* createProc()
{
	Process* temp = (Process*)malloc(sizeof(Process));
	if (temp == NULL)
		printf("hw1shell: malloc failed\n");
	else
		temp->next = NULL;

	return temp;
}

Process* addProc(Process* processes, Process* new_process)
{
	new_process->next = processes;
	return new_process;
}

int getProcLen(Process* processes)
{
	Process* curr = processes;
	int len = 0;
	while (curr != NULL)
	{
		len++;
		curr = curr->next;
	}
	return len;
}

Process* removeProc(Process* processes, int pid)
{
	Process* temp;
	Process* curr = processes;
	if (processes->pid == pid)
	{
		temp = processes;
		processes = processes->next;
		free(temp);
	}
	else
	{
		while (curr->next != NULL)
		{
			if (curr->next->pid == pid)
			{
				temp = curr->next;
				curr->next = curr->next->next;
				free(temp);
				break;
			}
			else
				curr = curr->next;
		}
	}
	return processes;
}

void printJobs(Process* processes)
{
	Process* curr = processes;
	while (curr != NULL)
	{
		printf("%d %s", curr->pid, curr->line);
		curr = curr->next;
	}
}

void splitLine(char line[], char* args[])
{
	char* token;
	int arg_count = 0;

	token = strtok(line, " \t\n\r");
	while (token != NULL)
	{
		args[arg_count] = token;
		arg_count++;
		token = strtok(NULL, " \t\n\r");
	}

	args[arg_count] = NULL;
}

void parseInput(char line[], char* argv[], Process* new_proc)
{
	int amp_loc, bBg;

	strncpy(new_proc->line, line, MAX_CHARS);
	amp_loc = strcspn(line, "&");	   // offset of first &
	bBg = amp_loc < strlen(line); // background process
	if (bBg)
		line[amp_loc] = '\0'; // remove & and text after it
	splitLine(line, argv);
	new_proc->bBackground = bBg;
}

Process* terminateZombies(Process* processes)
{
	int pid, status;
	while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
	{
		printf("hw1shell: pid %d finished\n", pid);
		processes = removeProc(processes, pid);
	}

	return processes;
}

Process* executeCommand(char line[], Process* processes)
{
	char* argv[MAX_ARGS];
	int pid, status;
	Process* new_proc;

	new_proc = createProc();
	if (new_proc == NULL) // malloc failed
	{
		while (getProcLen(processes) != 0)
			processes = terminateZombies(processes);
		exit(1);
	}

	parseInput(line, argv, new_proc);

	if (argv[0] != NULL) // not empty command
	{
		if (!strcmp(argv[0], "cd")) // Internal command: cd
		{
			if (argv[2] != NULL)
				printf("hw1shell: invalid command\n");
			else
				status = chdir(argv[1]);
			if (status == -1)
				printf("hw1shell: invalid command\n");
		}
		else if (!strcmp(argv[0], "jobs")) // Internal command: jobs
			printJobs(processes);
		else if (!strcmp(argv[0], "exit")) // Internal command: exit
		{
			while (getProcLen(processes) != 0)
				processes = terminateZombies(processes);
			exit(0);
		}
		else if (!strcmp(argv[0], "cat"))
		{
			if ((!strcmp(argv[2], "<")))
			{
				
			}
		}
		else // external commands
		{
			if (getProcLen(processes) == MAX_BACKGROUND_PROC && new_proc->bBackground) // check for max background processes
				printf("hw1shell: too many background commands running\n");
			else //  foreground process or enough space for another backround process
			{
				fflush(stdout);
				pid = fork();
				if (pid > 0) // parent process
				{
					new_proc->pid = pid;
					if (!new_proc->bBackground) // foregroud process
						waitpid(pid, &status, 0);
					else // backgroud process
					{
						processes = addProc(processes, new_proc);
						printf("hw1shell: pid %d started\n", pid);
					}
				}
				else if (pid == 0) // child process
				{
					status = execvp(argv[0], argv);
					if (status == -1)
					{
						printf("hw1shell: invalid command\n");
						printf("hw1shell: execvp failed, errno is %d\n", errno);
					}
					exit(0);
				}
				else // Fork failed
					printf("hw1shell: fork failed, errno is %d\n", errno);
			}
		}
	}
	processes = terminateZombies(processes);

	return processes;
}

int main()
{
	char line[MAX_CHARS];
	Process* processes = NULL;

	while (1)
	{
		printf("hw1shell$ ");
		fgets(line, MAX_CHARS, stdin);
		processes = executeCommand(line, processes);
	}

	return 0;
}



























































































































































































































































































































































































































































































