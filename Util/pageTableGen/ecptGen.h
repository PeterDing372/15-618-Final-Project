/* page table entry (PTE) fields */
#define PTE_V     0x001 /* Valid */
#define PTE_R     0x002 /* Read */
#define PTE_W     0x004 /* Write */
#define PTE_X     0x008 /* Execute */
#define PTE_U     0x010 /* User */
#define PTE_G     0x020 /* Global */
#define PTE_A     0x040 /* Accessed */
#define PTE_D     0x080 /* Dirty */
#define PTE_SOFT  0x300 /* Reserved for Software */
#define PTE_RSVD  0x1FC0000000000000 /* Reserved for future standard use */
#define PTE_PBMT  0x6000000000000000 /* Svpbmt: Page-based memory types */
#define PTE_N     0x8000000000000000 /* Svnapot: NAPOT translation contiguity */
#define PTE_ATTR  0xFFC0000000000000 /* All attributes and reserved bits */
#define PTE_PPN_SHIFT 10
#define PGSHIFT 12
#define RISCV_PGSHIFT 12

