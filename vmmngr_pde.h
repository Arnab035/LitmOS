/*****************************************************************************************
				Virtual memory interface for the page directory table
*****************************************************************************************/

#ifndef _MMNGR_VIRT_PDE_H
#define _MMNGR_VIRT_PDE_H

#include <stdint.h>
#include "mmngr_phys.h"

/* describe the structure of a page directory table */

// useful bit masks for the page directory table
enum PAGE_PDE_FLAGS {
	I86_PDE_PRESENT = 1,
	I86_PDE_WRITABLE = 2,
	I86_PDE_USER = 4,
	I86_PDE_PWT = 8,
	I86_PDE_PCD = 0x10,
	I86_PDE_ACCESSED = 0x20,
	I86_PDE_DIRTY = 0x40,
	I86_PDE_4MB = 0x80,  // page sizes
	I86_PDE_CPU_GLOBAL = 0x100,
	I86_PDE_LV4_GLOBAL = 0x200,
	I86_PDE_FRAME = 0xFFFFF000   // this is the pointer to a page table ~ page table base address
};

typedef uint32_t pd_entry;

// set a flag in the pde_entry
extern void pd_entry_add_attrib(pd_entry* e, uint32_t attrib);

// clear a flag in the pde entry
extern void pd_entry_del_attrib(pd_entry* e, uint32_t attrib);

// set the frame address i.e. the page table base address
extern void pd_entry_set_frame(pd_entry* , physical_addr );

// is the page in memory
extern bool pd_entry_is_present(pd_entry e);

// is the directory in user/supervisor mode
extern bool pd_entry_is_user(pd_entry e);

// is the page size in the directory 4MB/ 4KB
extern bool pd_entry_is_4mb(pd_entry e);

// is the page writable
extern bool pd_entry_is_writable(pd_entry e);

// return the physical address of the 4k frame associated with the page directory
extern physical_addr pd_entry_pfn(pd_entry e);

// enable global pages ?
extern void pd_entry_enable_global(pd_entry e);

#endif


