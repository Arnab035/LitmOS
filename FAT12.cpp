/*****************************************************************************************
					FAT 12 Implementation Methods 
******************************************************************************************/

#include "FAT12.h"
#include "fsys.h"
#include "bpb.h"
#include <string.h>

MOUNT_INFO _mountInfo;

void fsysFatMount(){

	PBOOTSECTOR bootsector = 0;
	bootsector = (PBOOTSECTOR) flpydsk_read_sector (0); // read boot sector from floppy disk.
	// I will need to study the implementation of interfaces to read data from floppy disk
	// Currently this method is not implemented.
	//------------------------!!!----------------------//
	// fill in values of the mount point structure
	_mountInfo.numSectors = bootsector->bpb.numSectors;
	_mountInfo.fatOffset = 1;
	_mountInfo.numRootEntries = bootsector->bpb.numDirEntries;
	_mountInfo.rootOffset = (bootsector->bpb.numberOfFats * bootsector->bpb.sectorsPerFat) + 1;
	_mountInfo.rootSize = (bootsector->bpb.numDirEntries * 32)/ (bootsector->bpb.bytesPerSector) ;
	_mountInfo.fatSize = bootsector->bpb.sectorsPerFat ;
	_mountInfo.fatEntrySize = 8;     // how ??? -- to be updated.
}

FILE fsysFatDirectory(const char* filename){
	if(!filename) return 0;
	PDIRECTORY directory;
	
	unsigned char* buf;
	FILE file;
	// step 1. First for easier comparison, convert the filename to DOS format
	char dosFileName[11];
	
	// ToDOSFileName will perform the conversion 
	ToDOSFileName(filename, dosFileName, 11);
	
	dosFileName[11] = 0; // string ends with a NULL
	// step 2. start comparison . first loop over all the max possible sectors in a root dir
	// the first sector is rootOffset obtained from bpb.
	// the next sectors are incremented by 1 every time till it becomes 13.(14 sectors are possible in a root dir)
	for(int i = 0; i < 14; i++){
		buf = (unsigned char*) flpydsk_read_sector(_mountOffset.rootOffset + i);
		directory = (PDIRECTORY) buf;
		
		// step 3. Now each sector will have 16 directory entries , each of size 32 bits (32 * 16 = 512 bits )
		// Loop across each of them-- process and compare with the first 11 bits
		for(int j = 0; j < 15; j++){
			char presentFileName[11]; 
			// remember memcpy is only needed to be performed once. VVI. 
			// It has already copied data from directory->extension(since we gave 11 bits to copy) 
			memcpy(presentFileName, directory->filename, 11);
			
			presentFileName[11] = 0;
			
			// found a match ?
			if(strcmp(dosFileName, presentFileName) == 0){
				
				strcpy(file.fileName, presentFileName);
				// complete the other properties
				file.eof = 0;
				file.fileLength = directory->fileSize;
				file.currentCluster = directory->firstCluster;
				file.id = 0;
				if(directory->attributes == 0x10){
					// means it is a subdirectory, not a file
					file.flags = FS_DIRECTORY;
				}
				else file.flags = FS_FILE;
				return file;
			}
		directory++ ; // 16 directory entries, increment one by one
		}
	}
	// cannot find the file sadly
	file.flags = FS_INVALID;
	return file;
	
}






