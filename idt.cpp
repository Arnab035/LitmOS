
#ifdef _MSC_VER
#pragma pack (push,1)
#endif


struct idt_descriptor {
    // bits 0-16 of interrupt routine address
    uint16_t baseLo;
    // code selector in GDT
    uint16_t sel;
    // reserved, should be 0
    uint8_t reserved;
    // bit flags
    uint8_t flags;
    // bits 16-32 of interrupt routine address
    uint16_t baseHi;
    
};

struct idtr {
    // size of the interrupt descriptor table
    uint16_t limit;
    // base address of idt
    uint32_t base;
};


#ifdef _MSC_VER
#pragma pack (push,1)
#endif

static struct idt_descriptor _idt[I86_MAX_DESCRIPTORS] ;

static struct idtr _idtr;

static void idt_install() {
    #ifdef _MSC_VER
        _asm lidt [_idtr]
    #endif
    }
    
// default interrupt handler for interrupts that are not caught
    
static void i86_default_handler() {
    #ifdef _DEBUG
        DebugClrScr(0x18);
        DebugGotoXY(0,0);
        DebugSetColor(0x1e);
        DebugPrintf("***[i86 Hal]: i86_default_handler: Unhandled exception");
    #endif
        for(;;);
    
    }
    
// generate any interrupt call

void geninterrupt(int n){
    #ifdef _MSC_VER
    _asm {
        mov al, byte ptr [n]
        mov byte ptr [genint + 1], al
        jmp genint
    genint:
        int 0
    }
    #endif    
}

// install a new interrupt handler

int i86_install_ir (uint32_t i, uint16_t flags, uint16_t sel, I86_IRQ_HANDLER irq) {
    if(i > MAX_INTERRUPTS)
        return 0
    if(!irq)
        return 0
    // irq is the function pointer to the interrupt handler that we have
    // get base address of the interrupt handler
    
    uint64_t uiBase = (uint64_t)&(*irq) ;
    
    // store base address into idt -- these are the low bits of the base address
    _idt[i].baseLo = (uint16_t)((uiBase) & 0xffff);
    // store the high bits of the base address -- by right shifting by 16 positions
    _idt[i].baseHi = (uint16_t)((uiBase >> 16) & 0xffff);
    // reserved value is always 0
    _idt[i].reserved = 0;
    // the flags bit
    _idt[i].flags = (uint8_t)flags;
    // this is the segment selector
    _idt[i].sel = sel;  
    
    return 0;
    
}


int i86_idt_initialize (uint16_t codeSel){
    
    // setup idtr register
    // idtr.limit refers to the size of the segment limit or segment size
    
    _idtr.limit = sizeof(struct idt_descriptor) * I86_MAX_INTERRUPTS - 1;
    // idtr.base refers to the start of the idt
    _idtr.base = (uint32_t) &_idt[0];
    
    memset((void*)&_int[0], 0, sizeof(struct idt_descriptor) * I86_MAX_INTERRUPTS - 1);
    
    // register any number of default interrupt handlers
    for (int i = 0; i < I86_MAX_INTERRUPTS; i++){
        i86_install_ir(i, I86_IDT_DESC_PRESENT|I86_IDT_DESC_BIT32, codeSel, (I86_IRQ_HANDLER)i86_default_handler);
        }
    
    //now install the idt --> store the idt value into the register idtr
    idt_install();
    
    return 0; 
}


