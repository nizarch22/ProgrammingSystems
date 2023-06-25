#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"
#include "hw5.h"

#define BPB_LEN 62
#define DIR_LEN 32
#define DIR_ENTRY_YEAR_START 1980

/* *** Description ***
 * Initially we load the BPB (loadBoot()), but only up until BPB_LEN (62 Bytes) as after
 * which, the sector will pose little use. 
 * BPB is used for its many essential properties.
 * 
 * 'DIR' command:
 *
 * We would like to access the root directory entries - as that is where we can find 
 * all the directories' information. We do this using BPB (as specified by the Microsoft
 * FAT12 Specification sheet).
 * We access the entries one after the other from the root sector, using the entry 
 * size (DIR_LEN - 32 bytes) for indexing (BPB property).
 * 
 *
 * 'CP' command:
 *
 * First, in order to copy the data of a file name, we need the entry of its root directory,
 * as it contains the first cluste of the file - which will lead us to the next
 * cluster (getFatEntry()), and the next, and so on until the end of file. 
 * To this end, the index of the root directory entry is acquired (findFileIndex()) using the 
 * file name. To do this, we chose to turn the file name into a FAT appropriate name 
 * (FatNameFormat()).
 *
 * Second, we need the data sector, as that is where all the clusters are located. 
 * We use the BPB propreties once again to acquire it.
 *
 * Thirdly, we allocate a 'data' buffer with an appropriate size to store all the clusters
 * of the file. Then, we proceed to load the file cluster by cluster (loadFileData()) into
 * the 'data' buffer.
 *
 *
 * Finally, we write the 'data' buffer into the new linux file.
 *
 *
 */


// global variables
struct msdos_dir_entry entry;
// 'non-changing' global variables --
FILE* imgFP;
struct fat_boot_sector bootSector;

unsigned int getClusterSize()
{
	return SECTOR_SIZE * bootSector.sec_per_clus;
}

// gets the fat entry of the cluster. This gives us the next cluster in the file
// or end-of-file. Otherwise, we get an exception.
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

// load 'imgFP' data into 'buff'.
// the data from the disk image (offset to offset+n bytes) is saved
// into 'buff'.
void loadImageData(void* buff, int offset, unsigned int n)
{
	// offset from the beginning of the file.
	fseek(imgFP, offset, SEEK_SET);
	// n bytes are stored to 'buff'.
	fread(buff, n, 1,imgFP);
}

// Load the first 62 bytes (i.e. BPB_LEN) of the boot (up until BS_FilSysType) 
// into bootSector (struct fat_boot_sector)
// where bootSector is a global variable.
void loadBoot()
{
	loadImageData(&bootSector,0,BPB_LEN);
}

void loadEntry(int dirIndex)
{
	loadImageData(&entry, dirIndex,DIR_LEN);
}

// Compute the date and time, and create the date and time strings 
// with an acceptable format.
void getEntryDateTime(char* date, char* time)
{
	__le16 d,t;
	__le16 day, month, year;
	__le16 hour, minute;

	d = entry.cdate;
	t = entry.ctime;
	
	// Calculating date.
	
	// day date bits: 0:4.
	// We isolate these bits from the rest (5-15).
	// Irrelevant left-most bits: 15-5+1 = 11.
	// Irrelevant right-most bits: none
	day = d;
	day <<=11;
	day >>=11;

	// month date bits: 5:8.
	// Irrelevant left-most bits: 15-9+1 = 7.
	// Irrelevant right-most bits: 4-0+1=5
	month = d;
	month <<=7;
	month >>=7;
	month >>=5;

	// month date bits: 9:15.
	// Irrelevant left-most bits: none.
	// Irrelevant right-most bits: 8-0+1=9 
	year = d;
	year >>=9;

	// starting from 1980
	year+=DIR_ENTRY_YEAR_START;
	
	// Calculating time.
	
	// minute date bits: 5:10.
	// Irrelevant left-most bits: 5.
	// Irrelevant right-most bits: 5 
	
	minute = t;
	minute <<=5;
	minute >>= 5;
	minute >>=5;

	// hour date bits: 11:15.
	// Irrelevant left-most bits: none.
	// Irrelevant right-most bits: 10-0+1=11 
	hour = t;
	hour >>=11;

	if(entry.start!=0)
		hour+=3;
	char ampm[3]="AM";
	if(hour>=12)
	{
		strcpy(ampm,"PM");
		if(hour!=12)
			hour-=12;
	}
	
	// formatting date and time into acceptable representations.
	sprintf(date,"%02d/%02d/%d",day, month, year);
	sprintf(time,"%02d:%02d ",hour, minute);
	strcat(time,ampm);
}

// find the index of the root directory entry which has the name contained by 'targetFile'.
// This is in order to get the first cluster contained in the entry.
int findFileIndex(unsigned int index, unsigned int n, char* targetFile)
{
	char buff[12];
	FatNameFormat(targetFile, buff);
	buff[11]=0;
	for(int i =0;i<n;i++)
	{
		loadEntry(index);
		if(entry.name[0]==0x00)
			break;
		if(entry.name[0]==0xE5)
			continue;
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
	char date[12];
	char time[9];
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
			printf("\t%d\t",entry.size);
		}

		printf("%s\n", entry.name);

		// Each entry is sized at 32 bytes. Therefore our index should increment exactly that much.
		index+=DIR_LEN;  
	}
}

// Checking input command syntax's correctness.
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

// converting 'buff' to the name that fits the FAT naming convention.
void FatNameFormat(char* buff, char* target)
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
	if(buff[index]==0)
		flag=1;
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

unsigned int getRootSector()
{
	unsigned int rootSectorIndex = bootSector.reserved + (bootSector.fats*bootSector.fat_length);
	unsigned int rootSector = rootSectorIndex*SECTOR_SIZE;

	return rootSector;
}
unsigned int getFileSize()
{
	unsigned int clusterSize = getClusterSize();
	unsigned int fileClusterCount = (entry.size+clusterSize-1)/clusterSize;
	unsigned int dataSize = fileClusterCount*clusterSize;

	return dataSize;
}
unsigned int getDataSector()
{
	// calculating root sector -- where directory entries are located
	unsigned int rootSector = getRootSector();
	// number of possible entries in the root sector 
	unsigned int numRootEntries = bootSector.entries;

	unsigned int bytesPerSec = bootSector.sector_size;
	unsigned int rootSectorNum = (numRootEntries*32+bytesPerSec-1)/bytesPerSec;
	unsigned int dataSector = rootSector + rootSectorNum*SECTOR_SIZE;

	return dataSector;
}

// load all file data: following the cluster after the next, we load the 'data' 
// buffer cluster by cluster, until we reach end-of-file.
void loadFileData(char* data)
{

	// Calculating location of the data sector -- where clusters are located
	unsigned int dataSector = getDataSector();
	// cluster handling - finding its location on the disk
	unsigned int clusterSize = getClusterSize();
	unsigned int currentCluster = entry.start;
	unsigned int clusterLocation = dataSector+clusterSize*(currentCluster-0x2);

	// loading clusters
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
	unsigned int rootSector = getRootSector();
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

	
	// loading the file entry
	int fileEntryLoc = rootSector + fileIndex * DIR_LEN; // file location on disk
	loadEntry(fileEntryLoc);


	// creating a sufficient data holder for the file 
	unsigned int dataSize = getFileSize();
	char* data = (char*)malloc(dataSize);

	// data loading - copying file into the 'data' buffer
	loadFileData(data);

	// closing image file - we are done copying. 
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

