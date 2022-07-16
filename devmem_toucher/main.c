#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>

int main(void) {
	int fd = open("/dev/mem", O_RDWR);
	void *ptr = mmap(NULL, 4096, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0x1fffffff - 4095 - (100UL << 20));
	printf("Got ptr: 0x%p\n", ptr);
	*(char *)ptr = 42;
	return 0;
}
