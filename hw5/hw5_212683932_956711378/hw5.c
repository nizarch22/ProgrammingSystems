#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"
#include "hw5.h"

#define BPB_LEN 62
#define DIR_LEN 32

// global variables
struct fat_boot_sector bootSector;
struct msdos_dir_entry entry;

// 'constant' global variables
FILE* imgFP;

unsigned int getClusterSize()
{
	return SECTOR_SIZE * bootSector.sec_per_clus;
}

unsigned int getFatEntry(unsigned int clusterIndex)
{
	__le16 fatEntry;
	unsigned int FATSz, FAToffset, FATsecNum, FATentryOffset;
	unsigned int bytesPerSec;
	unsigned int location;

	// calculate the FAT sector number of current cluster
	FATSz = bootSector.fat_length;
	FAToffset = clusterIndex + clusterIndex/2;
 	bytesPerSec = bootSector.sector_size;

	FATsecNum = bootSector.reserved + (FAToffset/bytesPerSec);

	// get the offset from the sector to reach the FAT entry
	FATentryOffset = FAToffset%bytesPerSec;
	
	// calculate location of the FAT entry of the current cluster
	location = FATsecNum*bytesPerSec;
	location += FATentryOffset; // offset of a few bytes
	
	loadImageData(&fatEntry,location,sizeof(__le16));
	if(clusterIndex & 0x0001)
	{
		// odd
		fatEntry >>=4;
	}
	else
	{
		// even
		fatEntry = fatEntry&0x0FFF;
	}
	return fatEntry;
}


void loadImageData(void* buff, int offset, unsigned int n)
{
	// 
	fseek(imgFP, offset, SEEK_SET);
	//
	fread(buff, n, 1,imgFP);
}

// Load the first 62 bytes of the boot (up until BS_FilSysType) into bootSector (struct fat_boot_sector)
// where bootSector is a global variable.
void loadBoot()
{
	loadImageData(&bootSector,0,BPB_LEN);
}


void loadEntry(int dirIndex)
{
	loadImageData(&entry, dirIndex,DIR_LEN);
}

unsigned int reverseBits(unsigned int num, unsigned int n)
{
    unsigned int NO_OF_BITS = n;
    unsigned int reverse_num = 0;
    int i;
    for (i = 0; i < NO_OF_BITS; i++) {
        if ((num & (1 << i)))
            reverse_num |= 1 << ((NO_OF_BITS - 1) - i);
    }
    return reverse_num;
}

void getEntryDateTime(char* date, char* time)
{
	__le16 d,t;
	unsigned int day, month, year;
	unsigned int hour, minute;

	d = entry.cdate;
	t = entry.ctime;
	
	__le16 mask = d;
	mask <<=11;
	mask >>=11;

	day = mask;
	day = reverseBits(day,5);

	mask = d;
	mask >>=5;
	mask <<=12;
	mask >>=12;

	month = mask;
	month = reverseBits(month,4);

	mask = d;
	mask >>=9;

	year = mask;
	//year = reverseBits(year,7);
	//printf("year:%d\n",year);
	
	// starting from 1980
	year+=1980;
	
	mask = t;
	mask >>=5;
	mask <<= 10;
	mask >>=10;
	minute = mask;
	//minute = reverseBits(minute, 6);

	mask = t;
	mask >>=11;
	hour = mask;
	//hour = reverseBits(hour,5);
	
	char ampm[3]={0};
	if(hour>=12)
	{
		strcpy(ampm,"PM");
		if(hour!=12)
			hour-=12;
	}
	else
	{
		strcpy(ampm,"AM");
	}

	sprintf(date,"%02d/%02d/%d",day,month,year);
	sprintf(time,"%02d:%02d ",hour,minute);
	strcat(time,ampm);
	//printf("time:%s\ndate:%s\n",time,date);
}

int findFileIndex(unsigned int index, unsigned int n, char* targetFile)
{
	char buff[11];
	format(targetFile, buff);
	for(int i =0;i<n;i++)
	{
		loadEntry(index);
		if(entry.name[0]==0x00||entry.name[0]==0xE5)
			break;
		if(isEqualStr(entry.name, buff))
			return i;
		// Each entry is sized at 32 bytes. Therefore our index should increment exactly that much.
		index+=DIR_LEN;  
	}
	return -1;
}



// Print all directories/files in root.
void printRootDir(unsigned int index, unsigned int n)
{
	const char dir[] = "<DIR>";
	char date[50];
	char time[50];
	for(unsigned int i =0;i<n;i++)
	{
		loadEntry(index);
		if(entry.name[0]==0x00)
			break;
		if(entry.name[0]==0xE5)
			continue;
		getEntryDateTime(date,time);
		printf("%s\t%s\t",date,time);
		if(entry.attr == ATTR_DIR)
		{
			printf("%s\t\t",dir);
		}
		else
		{
			printf("\t\t%d\t",entry.size);
		}

		printf("%s\n", entry.name);

		// Each entry is sized at 32 bytes. Therefore our index should increment exactly that much.
		index+=DIR_LEN;  
	}
}

// Checking input arguments' correctness
int checkErrors(int argc, char* argv[])
{
	if(argc < 3)
	{
		printf("[ERROR] must have 3 OR 4 arguments!\n");
		return -1;
	}
	
	int bDir = strcmp(argv[2],"dir")==0;
	int bCp = strcmp(argv[2],"cp")==0;
	if(!bDir && !bCp)
	{
		printf("[ERROR] Invalid command!\n");
		return -1;
	}
}
void format(char* buff, char* target)
{
	int index = 7;
	int flag = 0;
	for(int i=0;i<8;i++)
	{
		if(flag)
		{
			target[i] = ' ';
			continue;
		}
		
		if(buff[i]=='.'||buff[i] == 0)
		{
			index =i;
			flag=1;
			target[i] = ' ';
			continue;
		}
		target[i]=toupper(buff[i]);
	}

	flag = 0;
	for(int i=8;i<11;i++)
	{
		index++;
		if(flag)
		{
			target[i] = ' ';
			continue;
		}
		if(buff[index]=='.')
		{
			index++;
		}
		
		if(buff[index]==0)
		{
			flag = 1;
			target[i] = ' ';
			continue;
		}
		target[i] = toupper(buff[index]);
	}
}

int isEqualStr(char* a, char* b)
{
	for(int i=0;i<11;i++)
	{
		if(a[i]!=b[i])
			return 0;
	}
	return 1;
}


int main(int argc, char* argv[])
{
	// argument variables
	char* targetFile; // fat file
	char* filePath;   // linux file
	char* command;    // 'dir' or 'cp' command
	FILE* fp;	  // file pointer for the data being copied into (linux file or filePath)

	// Simple error checking of arguments
	if (checkErrors(argc,argv)<0)
		return -1;
	                                                  
	// Open image file
	imgFP = fopen(argv[1],"r");
	if(imgFP==NULL)
	{
		printf("[Error] opening file failed!\n");
		exit(-1);
	}
	
	// load BPB (BIOS Parameter Block) of FAT12
	loadBoot();

	// defining variables argv
	command = argv[2];
	filePath = NULL;
	targetFile = NULL;
	if(argc==5)
	{
		filePath = argv[4];  //linux
		targetFile = argv[3];//fat
	}

	// calculating root sector -- where directory entries are located
	unsigned int rootSectorIndex = bootSector.reserved + (bootSector.fats*bootSector.fat_length);
	unsigned int rootSector = rootSectorIndex*SECTOR_SIZE;
	// number of possible entries in the root sector 
	unsigned int numRootEntries = bootSector.entries;

	// Print all directories/files in root.
	if(strcmp(command,"dir") == 0)
	{
		printRootDir(rootSector, numRootEntries);
	}

	// Copy process
	if(strcmp(command,"cp") != 0)
	{
		fclose(imgFP);
		return 0;
	}
	
	// Calculating the file index 
	int fileIndex = findFileIndex(rootSector, numRootEntries, targetFile);
	if(fileIndex<0)
	{
		printf("[ERROR] File requested to copy:%s not found!\n",targetFile);
		exit(-1);
	}

	// file location on disk
	int fileEntryLoc = rootSector + fileIndex * DIR_LEN;

	// loading the file entry
	loadEntry(fileEntryLoc);

	// creating a sufficient data holder for the file 
	unsigned int clusterSize = getClusterSize();
	unsigned int fileClusterCount = (entry.size+clusterSize-1)/clusterSize;
	unsigned int dataSize = fileClusterCount*clusterSize;
	char* data = (char*)malloc(dataSize);

	// Calculating location of the data sector -- where clusters are located
	unsigned int bytesPerSec = bootSector.sector_size;
	unsigned int rootSectorNum = (numRootEntries*32+bytesPerSec-1)/bytesPerSec;
	unsigned int dataSector = rootSector + rootSectorNum*SECTOR_SIZE;

	// cluster handling - finding its location on the disk
	unsigned int currentCluster = entry.start;
	unsigned int clusterLocation = dataSector+clusterSize*(currentCluster-0x2);

	// data loading - copying file into the 'data' buffer
	int dataIndex = 0;
	char* p = data;
	while(currentCluster!=EOF_FAT12)
	{
		// load current cluster data
		loadImageData(p, clusterLocation, clusterSize);

		// get the next cluster and its location
		currentCluster = getFatEntry(currentCluster);
		clusterLocation = dataSector+clusterSize*(currentCluster-0x2);
	
		p+=clusterSize; // next cluster to be written into 'data'
	}
	fclose(imgFP);
	// write data into linux file
	fp = fopen(filePath, "wb");
	if(fp==NULL)
	{
		printf("[Error] opening file failed!\n");
		exit(-1);
	}
	fwrite(data, entry.size, 1, fp);

	// clear 
	fclose(fp);
	free(data);
}

