#include <proc.h>
#include <elf.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif


extern size_t ramdisk_read(void *buf, size_t offset, size_t len);

static uintptr_t loader(PCB *pcb, const char *filename) {
	Elf_Ehdr elfhdr;
	ramdisk_read(&elfhdr, 0, sizeof(Elf_Ehdr));
	Elf_Phdr prohdr[elfhdr.e_phentsize];
	size_t phentsize = elfhdr.e_phentsize;
	//printf("phentsize=%d\n",phentsize);
	//printf("entry=0x%08x\n",elfhdr.e_entry);
	for(int i = 0;i<elfhdr.e_phnum;i++)
	{
		ramdisk_read(&prohdr[i], elfhdr.e_phoff + i*phentsize, phentsize);
		if(prohdr[i].p_type == PT_LOAD)
		{
			ramdisk_read((void*)prohdr[i].p_vaddr,prohdr[i].p_offset, prohdr[i].p_filesz);
			printf("vaddr = 0x%08x,offset = 0x%08x,filesz = 0x%08x\n",prohdr[i].p_vaddr,prohdr[i].p_offset, prohdr[i].p_filesz);
			memset((void*)(prohdr[i].p_vaddr + prohdr[i].p_filesz), 0, prohdr[i].p_memsz - prohdr[i].p_filesz);
		}
	}
	return elfhdr.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
	uintptr_t entry = loader(pcb, filename);
	Log("Jump to entry = %p", entry);
	((void(*)())entry) ();
}

