#include "msdos_fs.h"

//clusters
unsigned int getClusterSize();
unsigned int getFatEntry(unsigned int currentCluster); // gives FAT entry for the current cluster. This will probably either indicate the end of the current file (FAT12_EOF) or the next cluster for the current file.

// load image data
void loadImageData(void* buff, int offset, unsigned int n);
void loadBoot();
void loadEntry(int dirIndex);

// dir, cp functions
void printRootDir(unsigned int index, unsigned int n); // prints all directory information.
int findFileIndex(unsigned int index, unsigned int n, char* targetFile); // finds the file index of file named 'targetFile'. Used to determine the entry of the file which we will copy.
void loadFileData(char* data); // uses 'loadImageData' to load all file clusters until end of file.

// helper functions
int checkErrors(int argc, char* argv[]); // check for error in command syntax.
void FatNameFormat(char* buff, char* target); // turn file name string into a file name that is acceptable by FAT12.
int isEqualStr(char* a, char* b); // compares strings.
void getEntryDateTime(char* date, char* time); // gives date and time their proper fomrat, as described in the hw5.pdf file.
// the following do calculations, as instructed in the Microsoft FAT description. 
unsigned int getRootSector();
unsigned int getFileSize();
unsigned int getDataSector();
