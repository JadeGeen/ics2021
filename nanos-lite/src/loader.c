#include <proc.h>
#include <elf.h>
#include <fs.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif


size_t ramdisk_read(void *buf, size_t offset, size_t len);
int fs_open(const char *pathname, int flags, int mode);
size_t fs_read(int fd, void *buf, size_t len);
size_t fs_write(int fd, const void *buf, size_t len);
size_t fs_lseek(int fd, size_t offset, int whence);
int fs_close(int fd);

static uintptr_t loader(PCB *pcb, const char *filename) {
	int fd = fs_open(filename, 0, 0);
	Elf_Ehdr elfhdr;
	fs_read(fd, &elfhdr, sizeof(Elf_Ehdr));
	Elf_Phdr prohdr[elfhdr.e_phentsize];
	size_t phentsize = elfhdr.e_phentsize;
	printf("phentsize=%d\n",phentsize);
	printf("entry=0x%08x\n",elfhdr.e_entry);
	for(int i = 0;i<elfhdr.e_phnum;i++)
	{
		fs_lseek(fd, elfhdr.e_phoff + i*phentsize, SEEK_SET);
		fs_read(fd, &prohdr[i], phentsize);
		if(prohdr[i].p_type == PT_LOAD)
		{
			//fs_lseek(fd, prohdr[i].p_offset, SEEK_SET);
			ramdisk_read((void*)prohdr[i].p_vaddr, prohdr[i].p_offset, prohdr[i].p_filesz);
			printf("vaddr = 0x%08x,offset = 0x%08x,filesz = 0x%08x\n",prohdr[i].p_vaddr,prohdr[i].p_offset, prohdr[i].p_filesz);
			memset((void*)(prohdr[i].p_vaddr + prohdr[i].p_filesz), 0, prohdr[i].p_memsz - prohdr[i].p_filesz);
		}
	}
	fd = fs_close(fd);
	return elfhdr.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
	uintptr_t entry = loader(pcb, filename);
	Log("Jump to entry = %p", entry);
	((void(*)())entry) ();
}

