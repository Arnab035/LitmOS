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

void vmmngr_initialize(){
	// initialize memory manager
	// Step 1. Create two page tables ---> to illustrate two different mapping schemes 
	
	// 1.a) this one is for the kernel
	ptable* table1 = (ptable*) pmmngr_alloc_block();
	if(!table1) 
		return;
		
	// 1.b) this one is not for the kernel
	ptable* table2 = (ptable*) pmmngr_alloc_block();
	if(!table2)
		return;
	// 1.ab) Clear page table for kernel
	vmmngr_ptable_clear(table);
		
	// Step 2. Start the mapping process --> mapping will be done in 4k blocks
	// 2.a) This is for the non-kernel
	for(int i=0, frame = 0x0, virt=0x00000000; i < 1024; i++, frame+=4096, virt+=4096){
		// create a new page (or a page table entry ;))
		pt_entry page = 0;
		pt_entry_add_attrib(&page, I86_PTE_PRESENT);
		
		
		pt_entry_set_frame(&page, (physical_addr)frame);
		
		table2->m_entries[PAGE_TABLE_INDEX(virt)] = page;
		
	}
	// 2.b) This is for the kernel
	for(int i=0, frame = 0x100000, virt = 0xc0000000; i < 1024; i++, frame+=4096, virt+=4096){
		
		// again do the same as previous loop
		pt_entry page = 0;
		pt_entry_add_attrib(&page, I86_PTE_PRESENT);
		
		pt_entry_set_frame(&page, (physical_addr)frame);
		table1->m_entries[PAGE_TABLE_INDEX(virt)] = page;
	}
		
	// 3. Create the page directory now --> this page directory will map to these two page tables
	pdirectory* dir = (pdirectory*)pmmngr_alloc_blocks(3) ; // this method needs defining
	if(!dir) 
		return;
	memset(dir, 0, sizeof(pdirectory));
	
	// 4. Now that page directory is created successfully. Create two different page directory entries
	// One that maps to table1 , the other maps to table2
	
	// 4. a) Since we use virtual address 0x000.. this maps to non-kernel table2
	pd_entry* dir1 = &dir->m_entries[GET_PAGE_DIRECTORY(0x00000000)];
	pd_entry_add_attrib(dir1, I86_PDE_PRESENT);
	
	pd_entry_add_attrib(dir1, I86_PDE_WRITABLE);
	pd_entry_set_frame(dir1, (physical_addr)table2);
	
	//4.b) This one maps to kernel table table1
	pd_entry* dir2 = &dir->m_entries[GET_PAGE_DIRECTORY(0xc0000000)];
	pd_entry_add_attrib(dir2, I86_PDE_PRESENT);
	pd_entry_add_attrib(dir2, I86_PDE_WRITABLE);
	pd_entry_set_frame(dir2, (physical_addr)table1);
	
	// 5. Finally set the pdbr registers to the current page directory
	_curr_pdbr = (physical_addr)&dir->m_entries; // refers to first element of dir
	bool result = vmmngr_switch_directory(dir);
	
	// 6. Set paging to be true.
	if(result)
		pmmngr_paging_enable(true);
}



