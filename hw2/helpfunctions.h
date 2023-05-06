#include <string.h>

long long getFileLen(FILE* file)
{                                      	
	fseek(file, 0L, SEEK_END);
	long fileLen = ftell(file);
	rewind(file);

	return fileLen;
}


void readFile(const char* filePath, char* buffer)
{
	long bufferCount = 0;
	FILE* file = fopen(filePath,"r");
	if(file==NULL)
	{
		printf("[ERROR] opening read file failed!\n");
		exit(-1);
	}
	long fileLen = getFileLen(file);
	while (!feof(file))
	{
		char ch = fgetc(file);
		buffer[bufferCount] = ch;
		if (bufferCount == fileLen)
			break;
		if(ch=='\0')
			break;
		//if(bufferCount >= bufferLength)
		//{
		//	printf("[ERROR] line is too large!\n");
		//}
		bufferCount++;
	}
	fclose(file);
	buffer[bufferCount] = '\0';
}
























