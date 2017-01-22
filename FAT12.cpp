/*****************************************************************************************
					FAT 12 Implementation Methods 
******************************************************************************************/

#include "FAT12.h"
#include "fsys.h"
#include "bpb.h"
#include <string.h>

#define SECTOR_SIZE 512

MOUNT_INFO _mountInfo;

uint8_t FAT[SECTOR_SIZE * 2];

// opens a file (note the file may be an absoulte file, may also include a path)
// like this a:\mydir\mytext.txt

void fsysFatOpen(const char* filename){
	if(!filename) return;
	char* p = 0;
	char pathFile[16];
	bool rootDir = true;
	char* path = (char *)filename; // const 
	// find the first occurence of '/'
	p = strchr(path, '\\');
	
	if (!p){
		// this is probably a file in rootDir
		FILE currDirectory = fsysFatDirectory(path);
		if(currDirectory->flags == FS_FILE){
			// file not present in rootdir
			return currDirectory;
		}
		FILE ret;
		ret.flags = FS_INVALID
		return ret;
	}
	// '/' is present
	p++ ;
	while(p){
		// file or directory names bounded by 16 characters
		char fileOrDir[16];
		for(i = 0; i<16; i++){
			if(p[i] == '\\' || p[i] == 0){
				break;
			}
			fileOrDir[i] = p[i] ;
		}
		fileOrDir[i] = 0;
		// check if it is a root directory or a sub directory.
		if(rootDir){
			FILE currDirectoryOrFile = fsysFatDirectory(fileOrDir);	
			rootDir = false; // root directory has been accessed. Now set rootDir to false
		}	
		else{
			FILE currDirectoryOrFile = fsysFatOpenSubDir(fileOrDir);
		}
		
		if(currDirectoryOrFile->flags == FS_FILE){
			return currDirectoryOrFile;
		}
		if(currDirectoryOrFile->flags == FS_INVALID)
			break;
		}
		// is this useful ?
		p=strchr (p+1, '\\');
		if (p)
			p++;
	}
	FILE ret;
	ret->flags = FS_INVALID;
	return ret;
}

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

// function to read in appropriate files
void fsysFatRead(PFILE file, unsigned char* buffer, unsigned int length){
	if(!file) return;
	// 1. Find the first cluster to be read from and finish reading the first 512 bytes off the sector
	unsigned int currSector = 32 + file->currentCluster - 1;
	
	unsigned char* sector = (unsigned char*) flpydsk_read_sector(currSector);
	memcpy(buffer, sector, 512);
	// 2. Now find the next cluster to read from
	// 2.a ) find the byte offset into the fat table
	unsigned int byteOffset = (file->currentCluster) * 1.5 ; // for FAT 12 devices, a FAT entry consists of 12 bits)
	// find the sector to read and also the offset into the sector
	unsigned int sectorToRead = byteOffset/(SECTOR_SIZE) + 1 ; // sectors counted from 1?
	unsigned int sectorOffset = byteOffset % (SECTOR_SIZE) ;
	
	// 3. now load both the sectors into our global FAT array.
	// two sectors need to be read or else it is likely that
	// the last cluster/ FAT entry would contain garbage values 
	unsigned char* sect1 = (unsigned_char* )flpydsk_read_sector(sectorToRead);
	memcpy(FAT, sect1, 512);
	
	unsigned char* sect2 = (unsigned char* )flpydsk_read_sector(sectorToRead + 1);
	memcpy(FAT+512, sect2, 512);
	
	// 4. Now that we are done. We need to read from the DATA stored in FAT array.
	// Why so? Because it is after all the FAT Table Entry
	// Read from the sector offset but read two bytes from it
	// why because a fat table entry is 12 bits
	// reading only a single bit won't help :0
	uint16_t fatEntry = *(uint16_t * )&FAT[sectorOffset];
	// 4. a) Determine if the current cluster is odd/even
	if(file->currentCluster & 0x0001 == 0){
		// even
		fatEntry = fatEntry >> 4 ;
	} 
	else{
		fatEntry = fatEntry & 0xfff;
	}
	// determine additional properties of this FAT entry-- check for file endings etc.
	if(fatEntry >= 0xFF8 ) // value marks last cluster in file
	{
		file->eof = 1;
		return;
	}	
	if(fatEntry == 0)   // value marks free cluster
	{
		file->eof = 1;
		return;
	}	
	
	file->currentCluster = fatEntry ;
}

// method to identify a file is present/not inside of a subdirectory 
// This method might look similar to a file being searched inside of
// a root directory

FILE fsysFatOpenSubDir(FILE kFile, const char* filename){
	
	if(!kFile) return;
	
	
	unsigned char buffer[512];
	
	unsigned char presentFileName[11];
	
	FILE file;
	unsigned char dosFileName[11];
	
	ToDOSFileName(filename, dosFileName, 11);
	dosFileName[11] = 0;
	
	// remember just like a root directory, a subdirectory entry contains
	// alot of directory entries, you need to keep reading those
	// entries just like to read off of a file.
	while(!kFile.eof)
	{
		fsysFatRead(&kFile, buffer, 512);
	
		PDIRECTORY pdir = (PDIRECTORY) buffer;
		// there will be 16 directory entries per sector...
		for(int i = 0; i < 16; i++){
			memcpy(presentFileName, pdir->filename, 11); // reads off extension as well
			presentFileName[11] = 0;
			// compare now
			if(strcmp(presentFileName, dosFileName) == 0){
				strcpy(file.fileName, presentFileName);
				file.eof = 0;
				file.currentCluster = pdir->firstCluster;
				file.fileSize = pdir->fileSize;
				file.id = 0;
				// check for flags
				if(pdir->attributes == 0x10){
					file.flags = FS_DIRECTORY;
				}
				else{
					file.flags = FS_FILE;
				}
				return file;
			}
			pdir++;
		}
	}
	
	// file not found :(
	file.flags = FS_INVALID;
	return file;
}











