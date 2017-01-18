/************************************************************************************
					Implementation of virtual memory manager methods 
************************************************************************************/

#include "mmngr_virt.h"
#include<string.h>

physical_addr _curr_pdbr = 0;

pdirectory* pdir = 0;


// allocates a page in physical memory -- pt_entry represents your page
bool vmmngr_alloc_page(pt_entry* e){
	// first allocate a block in physical memory
	void* addr = pmmngr_alloc_block();
	
	if(!addr) return false; // no viable memory location available
	
	// set the present flag in pt_entry
	pt_entry_add_attrib(e, I86_PTE_PRESENT);
	// now appropriately set the physical address as the frame address
	pt_entry_set_frame(e, (physical_addr)addr);
	return true;
}

// frees a page from physical memory -- pt_entry represents your page
void vmmngr_free_page(pt_entry* e){
	
	// you have to free the block of memory
	void* addr = (void* )pt_entry_pfn(e);
	if(!addr)
		pmmngr_dealloc_block(addr);
	// clear the present flag if set
	if(pt_entry_is_present(e))
		pt_entry_del_attrib(e, I86_PTE_PRESENT);
}

// return the page table entry from the page table index 
inline pt_entry* vmmngr_ptable_lookup_entry(ptable* p, virtual_addr addr){
	
	// access the page table index from the virtual address
	if(p){
		return &(p->m_entries[PAGE_TABLE_INDEX(addr)]);		
	}
}

// get directory entry from directory table by address 
inline pd_entry* vmmngr_pdirectory_lookup_entry (pdirectory* p, virtual_addr addr){
	
	if(p){
		return &p->m_entries[PAGE_DIRECTORY_INDEX(addr)] ;
	}
}

// switch the page directory entry --
// remember here two variables operate in unison -- pdir and _curr_pdbr
// both of them usually refer to the same thing - albeit with two distinct types

bool vmmngr_switch_directory(pdirectory* pd){
	if(!pd) return false;
	pdir = pd;
	
	pmmngr_load_PDBR(_curr_pdbr);
	
	return true;
}

pdirectory* vmmngr_get_directory(){
	return pdir;
}

// flush tlb entries --> invlpg is a ring 0 instruction
// takes the virtual address -> picks up the page table entry
// and invalidates it

void vmmngr_flush_tlb_entry(virtual_addr addr){
	#ifdef _MSC_VER
		_asm {
				cli
				invlpg addr
				sti
			}
	#endif
}

void vmmngr_map_page(void* phys, void* virt){
	
	if(!virt) return;
	// get me the directory address
	pdirectory* pd = pdir;
	pd_entry* pde = &pd->m_entries[PAGE_DIRECTORY_INDEX((virtual_addr)virt)];
	
	if((*pde & I86_PDE_PRESENT) != I86_PDE_PRESENT){
		// not there in memory --> your page table ain't there
		// create a simple memory block
		ptable* p = (ptable* )pmmngr_alloc_block();
		if(!p) return;
		
		memset(p, 0, sizeof(ptable));  // this is where you assign 1024 bytes of pte
		
		pd_entry* pde = &pd->m_entries[PAGE_DIRECTORY_INDEX((virtual_addr)virt)];
		// I recreate pde since I have changed it in an initial stmt(when I performed and : line 94)
		
		// set properties for this block
		pd_entry_add_attrib(pde, I86_PDE_PRESENT);
		pd_entry_add_attrib(pde, I86_PDE_WRITABLE);
		// appropriately marked
		pd_entry_set_frame(pde, (physical_addr)p);
	}
	
	// free to map now :)
	ptable* table = GET_PHYSICAL_ADDRESS(pde);
	
	pt_entry* pte = &table->m_entries[PAGE_TABLE_INDEX((virtual_addr)virt)];
	// you got the page table entry -- just map this entry to physical addr phys
	pt_entry_add_attrib(pte, I86_PTE_PRESENT);
	pt_entry_set_frame(pte, (physical_addr)phys);
	
	return;
}



