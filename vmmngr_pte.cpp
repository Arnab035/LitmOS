/************************************************************************
	implementation of functions defined in vmmngr_pte.h
************************************************************************/

#include "vmmngr_pte.h"

inline void pt_entry_add_attrib(pt_entry* e, uint32_t attrib){
	*e = *e | attrib ; // remember attrib is already in the format of a page entry flag --> so all we need is an OR operation
}

/*************************************************************************/

inline void pt_entry_del_attrib(pt_entry* e, uint32_t attrib){
	*e = *e & (~attrib);
}

/**************************************************************************/

inline void pt_entry_set_frame(pt_entry* e, physical_addr addr){
	*e = *e & (~I86_PTE_FRAME) ;  // clears the bits associated with the frame address
	*e = *e | addr ;   // then adds the address via an OR (similar to a add)
}

/**************************************************************************/

inline bool pt_entry_is_present(pt_entry e){
	return (e & I86_PTE_PRESENT);
}

/**************************************************************************/

inline bool pt_entry_is_writable(pt_entry e){
	return (e & I86_PTE_WRITABLE);
}

/**************************************************************************/

inline physical_addr pt_entry_pfn(pt_entry e){
	return (e & I86_PTE_FRAME);
}
