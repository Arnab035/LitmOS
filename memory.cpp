/*************************************************************************************************
					Implememntation of helper functions in our memory manager interface
**************************************************************************************************/

#include "memory.h"

// sets a bit in the bit-mapped array --> 
inline void mmap_set(int bit){
	_mmngr_memory_map[bit/32] = _mmngr_memory_map[bit/32] | (1 << (bit % 32)) ;
	return;
}

// clears a bit in the bit-mapped array 

inline void mmap_unset(int bit){
	_mmngr_memory_map[bit/32] = _mmngr_memory_map[bit/32] & ~(1 << (bit % 32)) ;
	return ;
}

// tests if a bit is set or cleared

inline bool mmap_test(int bit){
	return _mmngr_memory_map[bit/32] & (1 << (bit%32)) ;
}

// returns index of first free bit in the bit-map

int mmap_first_free(){
	for(uint32_t i = 0; i < pmmngr_get_block_count()/32 ; i++){
		if(_mmngr_memory_map[i] != 0xFFFFFFFF){
			for(int j = 0; j < 32; j++){
				if(!(_mmngr_memory_map[i] & (1 << j) ))
					return (32 * i + j) ;
			}
		}
	}
	return -1;
}

// define methods in physical memory allocator

// initialize the physical memory manager
// by default all of the physical memory must be in use

void pmmngr_init(size_t memSize, physical_addr bitmap){
	_mmngr_memory_size = (uint32_t )memSize ;
	_mmngr_memory_map = (uint32_t *)bitmap ;
	
	_mmngr_max_blocks = (pmmngr_get_memory_size() * 1024)/ PMMNGR_BLOCK_SIZE ;
	
	_mmngr_used_blocks = pmmngr_get_block_count();
	
	memset(_mmngr_memory_map, 0xf, pmmngr_get_block_count()/PMMNGR_BLOCKS_PER_BYTE);
	return ;
}

// initializes a region of memory for use-- remember most of the blocks are already in use initially
void pmmngr_init_region(physical_addr base, size_t size){
	int alignment = base/PMMNGR_BLOCK_SIZE ; // give me the block number
	
	int blocks = size/PMMNGR_BLOCK_SIZE; // give me the number of blocks I have
	
	for(; blocks > 0; blocks--){
		mmap_unset(alignment++) ;
		_mmngr_used_blocks-- ;
	}
	mmap_set(0) ;  // remember to ensure that no data structures defined within the first 64kbs are overwritten
	// this region of memory may contain important system procedures like IVT
	
}

//deinitialize a region of memory that is, instead of clearing the bit, set it up
// so that the region of memory becomes used again

void pmmngr_deinit_region(physical_addr base, size_t size){
	int alignment = base/PMMNGR_BLOCK_SIZE;
	
	int blocks = size/PMMNGR_BLOCK_SIZE;
	
	for(; blocks >0 ; blocks--){
		mmap_set(alignment++);
		_mmngr_used_blocks++;
	}
}

// allocation functions --> you are ready to perform allocation and deallocation functions now :)

void* pmmngr_alloc_block(){
	// find the first bit that is clear --> so as to give you the first block that is free
	if(pmmngr_get_free_block_count() <= 0) return 0; // no free blocks
	int frame = mmap_first_free() ;
	
	if(frame == -1) return 0; // no free blocks here too
	mmap_set(frame);
	
	physical_addr addr = frame * PMMNGR_BLOCK_SIZE;
	_mmngr_used_blocks++;
	return (void*)addr ;
}

// deallocate memory - quite similar to the previous method

void pmmngr_dealloc_block(void* p){
	physical_addr addr = (physical_addr)p ;
	
	int toDealloc = addr/PMMNGR_BLOCK_SIZE;
	mmap_unset(toDealloc);
	
	_mmngr_used_blocks-- ;
}
