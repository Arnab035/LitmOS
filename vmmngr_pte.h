#ifndef _MMNGR_VIRT_PTE_H
#define _MMNGR_VIRT_PTE_H

/***************************************************************************************************
							virtual memory manager interface for the page table
***************************************************************************************************/

#include<stdint.h>
#include "mmngr_phys.h"

/* describe the structure of a page table entry */

// useful bit masks
enum PAGE_PTE_FLAGS {
	I86_PTE_PRESENT = 1,           	// 00000000000000000000000000000001
	I86_PTE_WRITABLE = 2,			// 0000000000000000000000000000010
	I86_PTE_USER = 4,				// 0000000000000000000000000000100
	I86_PTE_WRITETHROUGH = 8,		// 0000000000000000000000000001000
	I86_PTE_CACHEABLE = 0x10,		// 0000000000000000000000000010000
	I86_PTE_ACCESSED = 0x20,		// 0000000000000000000000000100000
	I86_PTE_DIRTY = 0x40,			// 0000000000000000000000001000000
	I86_PTE_PAT = 0x80,				// 0000000000000000000000010000000
	I86_PTE_CPU_GLOBAL = 0x100,		// 0000000000000000000000100000000
	I86_PTE_LV4_GLOBAL = 0x200,		// 0000000000000000000001000000000
	I86_PTE_FRAME = 0xFFFFF000      // may need to be modified here --> possibly 0xFFFFF000
};

// abstract the page table entry byte
typedef uint32_t pt_entry;


// function prototypes
// sets a flag in the page table entry
extern void pt_entry_add_attrib(pt_entry* e, uint32_t attrib);

// clears a flag in the page table entry
extern void pt_entry_del_attrib(pt_entry* e, uint32_t attrib);

// sets a physical address frame to a page table entry
extern void pt_entry_set_frame(pt_entry* , physical_addr);

// determines if the page is present in physical memory
extern bool pt_entry_is_present(pt_entry e);

// determines if the page is writable
extern bool pt_entry_is_writable(pt_entry e);

// returns the physical address(commonly frame) associated with the page
extern physical_addr pt_entry_pfn(pt_entry e);

#endif


