/************************************************************************
			This is the interface for our virtual memory manager
*************************************************************************/

#ifndef _MMNGR_VIRT_H
#define _MMNGR_VIRT_H

// required header files

#include "vmmngr_pte.h"
#include "vmmngr_pde.h"

// abstract a virtual address

typedef uint32_t virtual_addr;

// define the number of page table entries and the number of page directory entries

#define PAGES_PER_TABLE 1024            // number of page table entries
#define PAGES_PER_DIR 1024             // number of directory entries

// useful macro functions that return each of the components of the virtual address

#define PAGE_DIRECTORY_INDEX(x) (((x) >> 22) & 0x3ff) 
#define PAGE_TABLE_INDEX(x)  (((x) >> 12) & 0x3ff)
#define GET_PHYSICAL_ADDRESS(x)  (*x & ~0xfff)  // remember what does *x return --> returns the page table entry or the 
												// particular memory page (obtain the last 20 bits to get your frame address)

// define virtual sizes of page table and page directory

#define PTABLE_ADDR_SPACE_SIZE 0x400000    // 4MB
#define DTABLE_ADDR_SPACE_SIZE 0x100000000  // 4GB

// page sizes are 4k

#define PAGE_SIZE 4096

struct ptable {
	pt_entry m_entries[PAGES_PER_TABLE];
};

struct dtable {
	pd_entry m_entries[PAGES_PER_DIR];
};

// define useful functions for virtual manager

extern bool vmmngr_alloc_page(pt_entry* ); // allocates a page in physical memory

// maps physical address to virtual address

extern void MmMapPage(void* phys, void* virt);

// initialize the memory manager

extern void vmmngr_initialize();

// frees a page in physical memory
extern void vmmngr_free_page(pt_entry* e);

// switch to a new page directory
extern bool vmmngr_switch_pdirectory(pdirectory* );

// get current page directory
extern pdirectory* vmmngr_get_directory();

// flushes a cached translation lookaside buffer (TLB) entry
extern void vmmngr_flush_tlb_entry(virtual_addr addr);

// clears a page table
extern void vmmngr_ptable_clear(ptable* p);

// convert virtual address to page table index
extern uint32_t vmmngr_ptable_virt_to_index(virtual_addr addr);

// get page entry from page table
extern pt_entry* vmmngr_ptable_lookup_entry(ptable* p, virtual_addr addr);

// convert virtual address to page directory index
extern uint32_t vmmngr_pdirectory_virt_to_index(virtual_addr addr);

// clears a page directory table
extern void vmmngr_pdirectory_clear(pdirectory* dir);

// get directory entry from directory table
extern pd_entry* vmmngr_pdirectory_lookup_entry(pdirectory* p, virtual_addr addr);

#endif



