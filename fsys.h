#ifndef _FSYS_H
#define _FSYS_H


/***************************************************************************************
				Interfaces to a virtual file system 
****************************************************************************************/

#include<stdint.h>

typedef struct _FILE {
	char fileName[32];
	uint32_t fileLength;
	uint32_t flags;
	uint32_t eof; 
	uint32_t position;         // position/offset into the current cluster
	uint32_t currentCluster;   // current cluster
	uint32_t id;        // for identification purposes
	uint32_t device;    // which device is it in ?
} FILE, *PFILE;

// different types of file

#define FS_FILE 0
#define FS_DIRECTORY 1
#define FS_INVALID 2

// define useful file system interface methods like opening/closing/reading/mounting

extern FILE volOpenFile(const char* filename);
extern void volReadFile(PFILE file, unsigned char* buffer, unsigned int length);
extern void volCloseFile(PFILE file);
extern void volRegisterFileSystem(PFILESYSTEM , unsigned int deviceId);
extern void volDeRegisterFileSystem(PFILESYSTEM );
extern void volDeRegisterFileSystemByID(unsigned int deviceId);

// all the functions performed by our files are implemented via using function pointers
// Why is this so? Understand that if we defined functions separately we could only have
// a single set of functions for a wide range of filesystems.
// Now for each different filesystem, we could have a different function for reading/writing etc. altogether..

typedef struct _FILE_SYSTEM{
	char name[8];
	FILE (*directory) (const char* directoryName);
	void (*mount) () ;
	void (*read) (PFILE file, unsigned char* buffer, unsigned int length);
	void (*close) (PFILE file);
	void (*open) (const char* fileName);
} FILESYSTEM, *PFILESYSTEM; 


#endif 
