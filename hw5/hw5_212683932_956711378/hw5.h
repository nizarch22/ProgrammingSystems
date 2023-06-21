#include "msdos_fs.h"
unsigned int getClusterSize();
unsigned int getNextCluster(unsigned int currentCluster);
void loadImageData(void* buff, int offset, unsigned int n);
void loadBoot();
void loadEntry(int dirIndex);
void printRootDir(unsigned int index, unsigned int n);
int checkErrors(int argc, char* argv[]);
void format(char* buff, char* target);
int isEqualStr(char* a, char* b);
