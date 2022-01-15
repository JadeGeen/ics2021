#include <fs.h>

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
	char *name;
	size_t size;
	size_t disk_offset;
	ReadFn read;
	WriteFn write;
	size_t open_offset;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_EVENTS, FD_FB};

size_t serial_write(const void *buf, size_t offset, size_t len);
size_t events_read(void *buf, size_t offset, size_t len);

size_t invalid_read(void *buf, size_t offset, size_t len) {
	panic("should not reach here");
	return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
	panic("should not reach here");
	return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
	[FD_STDIN]  = {"stdin", 0, 0, invalid_read, invalid_write},
	[FD_STDOUT] = {"stdout", 0, 0, invalid_read, serial_write},
	[FD_STDERR] = {"stderr", 0, 0, invalid_read, serial_write},
	[FD_EVENTS] = {"/dev/events", 0, 0,events_read, invalid_write},
#include "files.h"
};

void init_fs() {
	// TODO: initialize the size of /dev/fb
}

#define file_num (sizeof(file_table)/sizeof(Finfo))

size_t lenchoose(size_t a, size_t b){
	return a<=b?a:b;
}

int fs_open(const char *pathname, int flags, int mode){
	int i = 0;
	for(;i < file_num;i++){
		if(strcmp(pathname, file_table[i].name) == 0){
			file_table[i].open_offset = 0;
			//printf("filename = %s\n",file_table[i].name);
			return i;
		}
	}
	Log("cannot find %s",pathname);
	assert(i!=file_num);
	return -1;
}

extern size_t ramdisk_read(void *buf, size_t offset, size_t len);
extern size_t ramdisk_write(const void *buf, size_t offset, size_t len);

size_t fs_read(int fd, void *buf, size_t len){
	size_t readlen = lenchoose(len, file_table[fd].size - file_table[fd].open_offset);
	assert(readlen + file_table[fd].open_offset <= file_table[fd].size);
	size_t offset = file_table[fd].disk_offset + file_table[fd].open_offset;
	size_t ret = 0;
	if(file_table[fd].read == NULL){
		ret = ramdisk_read(buf, offset, readlen);
		file_table[fd].open_offset += readlen;
	}
	else
		ret = file_table[fd].read(buf, offset, len);
	return ret;
}

size_t fs_write(int fd, const void *buf, size_t len){
	size_t writelen = lenchoose(len, file_table[fd].size - file_table[fd].open_offset);
	assert(writelen + file_table[fd].open_offset <= file_table[fd].size);
	size_t offset = file_table[fd].disk_offset + file_table[fd].open_offset;
	size_t ret = 0;
	if(file_table[fd].write == NULL){
		ret = ramdisk_write(buf, offset, writelen);
		file_table[fd].open_offset += writelen;
	}
	else
		ret = file_table[fd].write(buf, offset, len);//right for fd =1,fd = 2, maybe error for others
	return ret;
}

size_t fs_lseek(int fd, size_t offset, int whence){
	switch(whence){
		case SEEK_SET:
			file_table[fd].open_offset = offset;
			break;
		case SEEK_CUR:
			file_table[fd].open_offset += offset;
			break;
		case SEEK_END:
			file_table[fd].open_offset = file_table[fd].size + offset;
			break;
		default:
			panic("Unknown whence %d", whence);
	}
	return file_table[fd].open_offset;
}

int fs_close(int fd){
	file_table[fd].open_offset = 0;
	return 0;
}
