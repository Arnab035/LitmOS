/*****************************************************************************
				Useful method implementations of our filesystem
*****************************************************************************/


#include "fsys.h"

#define MAX_DEVICE 26

PFILESYSTEM _file_system_list[MAX_DEVICE];

// implement file open operations 
// remember there are two ways of sending in the filename --> either include the 
// device name like "a:\filename.txt" or pass in the direct name, like 
//"filename.txt", by default search will be in the drive a: 

FILE volOpenFile(const char* fname){
	if(fname){
		unsigned char device = 'a'; // by default
		char* filename = (char*) fname;
		
		// first character is the device name, if the 2nd char is ':'
		if(filename[1] == ':'){
			device = filename[0];
			filename += 2;  // filename now starts pointing two bytes ahead-->gets me the actual filename
		}
		
		if(_file_system_list[device - 'a']{
			PFILESYSTEM _some_file_system = _file_system_list[device - 'a'];
			FILE file = _some_file_system->open(filename);
			
			file.device = device;
			return file;
		}
		
	}
	FILE file;
	file.flags = FS_INVALID;
	return file;
}

void volRegisterFileSystem(PFILESYSTEM fsys, unsigned int deviceId){
	
	if(deviceId < MAX_DEVICE){
		if(fsys){
			_file_system_list[deviceId] = fsys ; // register file system for the particular device
		}
	}
}

// define the other functions

void volDeRegisterFileSystem(PFILESYSTEM fsys){
	if(!fsys) return;
	// iterate through all the devices in the device list
	for(int i = 0; i< MAX_DEVICE; i++){
		if(_file_system_list[i] == fsys)
			_file_system_list[i] = 0;
	}
}

void volDeRegisterFileSystemByID(unsigned int deviceId){
	if(deviceId >= MAX_DEVICE) return;
	
	_file_system_list[deviceId] = 0;
}

void volCloseFile(PFILE file){
	// get me the device where the file is in
	if(!file) return;
	if(_file_system_list[file->device - 'a'])
		(_file_system_list[file->device - 'a'])->close(file); 
}


void volReadFile(PFILE file, unsigned char* buffer, unsigned int length){
	if(!file) return;
	// identify which file system the file is in --> then call the method defined for that file system
	if(_file_system_list[file->device - 'a'])
		(_file_system_list[file->device - 'a'])->read(file, buffer, length);
	
}


