#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

// The smaller ones need to divide into the bigger ones.
#define BUF_SIZE (1000UL * 1024 * 1024)
#define MEASURE_CHUNK_SIZE (10UL * 1024 * 1024)
#define CACHE_LINE 64

int main () {
	clock_t userspace;
	struct timespec wall;
	// [wall_sec, wall_ns, userspace_someting]
	unsigned long measurements[3 * (BUF_SIZE / MEASURE_CHUNK_SIZE)];
	int measurement_idx = 0;

	char *buf = malloc(BUF_SIZE);
	if (!buf)
		return -ENOMEM;

	for(;;) {
		int i = 0;
		while(i < BUF_SIZE) {

			for(int j = i; j < i + MEASURE_CHUNK_SIZE; j += CACHE_LINE) {
				buf[j] += 1;
			}
			i += MEASURE_CHUNK_SIZE;

			userspace = clock();
			clock_gettime(CLOCK_REALTIME, &wall);

			measurements[measurement_idx++] = wall.tv_sec;
			measurements[measurement_idx++] = wall.tv_nsec;
			measurements[measurement_idx++] = userspace;

			printf("wall time %lu s\n", measurements[measurement_idx - 2]);
			printf("clock() time: %lu s\n", measurements[measurement_idx - 1]);

			if (measurement_idx == sizeof(measurements) / sizeof(*measurements))
				measurement_idx = 0;


		}
	}
   	return 0;
}
