#ifdef _MSC_VER   // used for visual studio compilers

#pragma pack (push, 1)
#endif

//! gdt descriptor --> defines the properties of the memory block that
//! we will used

struct gdt_descriptor {
	// bits 0-15 of segment limit
	uint16_t segLimit;
	// bits 0-23 of base address
	uint16_t baseLo;
	
	uint8_t baseMid;
	// descriptor flags. Set using bit masks
	uint8_t flags; // represents access byte
	uint8_t grand; // represents granularity
	
	//bits 24-32 of base address
	unit8_t baseHi;
	
};

// processor gdtr register points to the base of gdt. This
// helps us set the pointer

struct gdtr {
	uint16_t gdtLimit;
	uint32_t gdtBase;
	
};

#ifdef _MSC_VER
#pragma pack (pop,1)
#endif

static struct gdt_descriptor _gdt[MAX_DESCRIPTORS];
static struct gdtr _gdtr;

static void gdt_install(){
	#ifdef _MSC_VER
		_asm lgdt [_gdtr]    // example of inline assembly code, we know what lgdt is 
	#endif
}

void gdt_set_descriptor(uint32_t i, uint64_base, uint64_t limit, uint8_t access, uint8_t grand) {
	if(i > MAX_DESCRIPTORS)
		return;
	memset((void*)&_gdt[i], 0, sizeof(gdt_descriptor));
	// void* memset(void* s, int c, size_t n) // what memset does? -> Fills the first n bytes of the memory
	// area pointed to by s with the constant byte c. It returns a pointer to the memory area pointed to by
	// s.
	_gdt[i].baseLo = base & 0xffff;
	_gdt[i].baseMid = (base >> 16) & 0xff;
	_gdt[i].baseHi = (base >> 24) & 0xff;
	
	_gdt[i].limit = limit & 0xffff;
	
	_gdt[i].flags = access;
	_gdt[i].grand = (limit >> 16) & 0x0f ;
	_gdt[i].grand |= grand & 0xf0 ;
	
}

int i86_gdt_initialize() {
	
	_gdtr.m_limit = (sizeof(struct gdt_descriptor) * MAX_DESCRIPTORS) - 1;
	
	_gdtr.m_base = (uint32_t)&_gdt[0];
	// null descriptor
	gdt_set_descriptor(0,0,0,0,0);
	
	// set default code descriptor
	gdt_set_descriptor(1,0,0xffffffff, I86_GDT_DESC_READWRITE|I86_GDT_DESC_EXEC_CODE|I86_GDT_DESC_CODEDATA|
							I86_GDT_DESC_MEMORY, I86_GDT_GRAND_4K|I86_GDT_GRAND_32BIT|I86_GDT_GRAND_LIMITHI_MASK)
							
	// set default data descriptor
	gdt_set_descriptor(2, 0, 0xffffffff, I86_GDT_DESC_READWRITE| I86_GDT_DESC_CODEDATA| I86_GDT_DESC_MEMORY,
							I86_GDT_GRAND_4K|I86_GDT_GRAND_32BIT|I86_GDT_GRAND_LIMITHI_MASK);
							
	// install gdtr
	gdt_install();
	return 0;
}

// to be added idt descriptors 


