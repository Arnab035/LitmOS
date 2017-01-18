/******************************* Memory manager interface ************************************
 ************************  We have a bit-mapped memory manager where each block is 4096 bytes 
 *********************************************************************************************/
 
// define global constants
#define PMMNGR_BLOCKS_PER_BYTE 8
#define PMMNGR_BLOCK_SIZE 4096
#define PMMNGR_BLOCK_ALIGN PMMNGR_BLOCK_SIZE

// define important variables which describe the properties of our physical memory
// and the memory manager as a whole

// this describes the size of the physical memory
static uint32_t _mmngr_memory_size = 0;

// this describes the number of blocks already used
static uint32_t _mmngr_used_blocks = 0;

// this is the maximum number of available memory blocks
static uint32_t _mmngr_max_blocks = 0;

// the bit array that describes which blocks are free and which are not
static uint32_t* _mmngr_memory_map = 0;


// important functions used are --

void* pmmngr_alloc_block();

void pmmngr_dealloc_block(void* );

void pmmngr_deinit_region(physical_addr , size_t );

void pmmngr_init_region(physical_addr , size_t );

void pmmngr_init(size_t , physical_addr );

int mmap_first_free();

inline void mmap_set(int );

inline void mmap_unset(int );

inline bool mmap_test(int );

size_t pmmngr_get_memory_size();

uint32_t pmmngr_get_block_count();

uint32_t pmmngr_get_use_block_count();

uint32_t pmmngr_get_free_block_count();

uint32_t pmmngr_get_block_size();

void pmmngr_load_PDBR (physical_addr );

physical_addr pmmngr_get_PDBR();

