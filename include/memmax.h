#ifdef X86_QEMU
#define MAXADDR	0x20000000	/* 512 MB RAM starting from 0x00000000	*/
#endif

#ifdef ARM_QEMU
#define MAXADDR	0x10000000	/* 256 MB RAM starting from 0x00000000	*/
#endif

#ifdef ARM_BBB
#define MAXADDR	0xA0000000	/* 512 MB RAM starting from 0x80000000	*/
//#define MAXADDR	0x90000000	/* 256 MB RAM starting from 0x80000000 (BB-white) */
#endif
