/*****************************************************************************************
						File System Driver Interface (FAT 12)							
******************************************************************************************/

#ifndef _FAT12_H
#define _FAT12_H

#include <stdint.h>

typedef struct _mount_info {
	uint32_t numSectors;
	uint32_t fatOffset;
	uint32_t numRootEntries;
	uint32_t rootOffset;
	uint32_t rootSize
	uint32_t fatSize;
	uint32_t fatEntrySize;
}MOUNT_INFO, *PMOUNT_INFO;


#ifdef _MSC_VER
#pragma pack (push, 1)
#endif

typedef struct _directory {
	uint8_t filename[8];
	uint8_t extension[3];
	uint8_t attributes;
	uint8_t reserved;
	uint8_t createdTimeMs;   // created time in milliseconds
	uint16_t createdTime;
	uint16_t createdYear;
	uint16_t lastAccessDate;
	uint16_t firstClusterHiBytes; //  EA index (in tutorial)
	uint16_t lastModifiedTime;
	uint16_t lastModifiedDate;
	uint16_t firstCluster;
	uint32_t fileSize;
}DIRECTORY, *PDIRECTORY;

#ifdef _MSC_VER
#pragma pack (pop)
#endif

#endif
