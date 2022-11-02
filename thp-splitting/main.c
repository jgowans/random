#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                       } while (0)

#define BUFFER_SIZE (100UL * (2 << 20))
char *buffer;

int main(void) {
    int rc;

    printf("Running, PID: %i\n", getpid());

    /* Currently, Transparent Huge Pages work only with private anonymous pages (see mmap(2)) */
    /* Why don't you give me an aligned addres.... really. */
    buffer = mmap((void *) 0x7fd000000000, BUFFER_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (!buffer)
        errExit("mmap");
    printf("Buffer: %p\n", buffer);

    rc = madvise(buffer, BUFFER_SIZE, MADV_HUGEPAGE);
    if (rc)
        errExit("MADV_HUGEPAGE");

    for (long int idx = 0; idx < BUFFER_SIZE; idx++) {
        buffer[idx] = 0xAA;
    }

    while(1) {
        sleep(1);
    }
    return 0;
}
