/************************************************************************************
					Boot sector interface 
************************************************************************************/

#ifndef BPB_H
#define BPB_H

#include <stdint.h>

#ifdef _MSC_VER
#pragma pack (push, 1)
#endif

typedef struct _boot_sector {
	uint8_t ignore[3] ; // 3 bytes
	bpbparameterblock bpb;  // this is our oem parameter block (33 bytes)
	bpbparameterblockext bpbExt; // this is an extension to our oem parameter block (probably used over FAT32 file systems)-->28 bytes
	uint8_t padding[448];  // leaves us 448 bytes to fill up 512 bytes of boot sector(actually contains the rest of the bootsector code)
} BOOTSECTOR,  *PBOOTSECTOR;

typedef struct _bios_parameter_block {
	
	uint8_t OEMName[8];
	uint16_t bytesPerSector;
	uint8_t sectorsPerCluster;
	uint16_t reservedSectors;
	uint8_t numberOfFats;
	uint16_t numDirEntries;
	uint16_t numSectors;
	uint8_t media;
	uint16_t sectorsPerFat;
	uin16_t sectorsPerTrack;
	uint16_t headsPerCyl;
	uint32_t hiddenSectors;
	uint32_t longSectors;
}BIOSPARAMETERBLOCK, *PBIOSPARAMETERBLOCK;

typedef struct _bios_parameter_block_ext {
	uint32_t sectorsPerFat32; // sectors per FAT
	uint16_t flags;           // flags
	uint16_t version;         // version
	uint32_t rootCluster;     // starting root directory
	uint16_t infoCluster;
	uint16_t backupBoot;      // location of bootsector copy
	uint16_t reserved[6];
}BIOSPARAMETERBLOCKEXT, *PBIOSPARAMETERBLOCKEXT;

#ifdef _MSC_VER
#pragma pack (pop)
#endif

#endif


