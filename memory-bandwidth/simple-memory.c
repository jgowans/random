#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// The smaller ones need to divide into the bigger ones.
#define BUF_SIZE (1000UL * 1024 * 1024)
#define CACHE_LINE 64
// Chunk this many accesses into a measurement.
// (MEASUREMENT_ACCESSES * CACHE_LINE) should divide into BUF_SIZE to keep our
// loops easy below.
#define MEASUREMENT_ACCESSES 163840
// Just so happens to come out as 100.... :-)
#define MEASUREMENTS_PER_ITER (BUF_SIZE / (MEASUREMENT_ACCESSES * CACHE_LINE))

#define INDEXES (MEASUREMENTS_PER_ITER * MEASUREMENT_ACCESSES)
#define PAGE_SIZE 4096
#define ITERATIONS 100

#define RANDOM_ACCESS 0

int main () {
	clock_t userspace;
	struct timespec wall;
	// [wall_sec, wall_ns, userspace_someting]
	unsigned long measurements[3 * MEASUREMENTS_PER_ITER];
	int measurement_idx = 0;

	char *buf = malloc(BUF_SIZE);
	unsigned int *idx_arr = malloc(INDEXES * sizeof(*idx_arr));
	if (!buf || !idx_arr)
		return -ENOMEM;

	for(int i = 0; i < INDEXES; ++i)
#if RANDOM_ACCESS
		idx_arr[i] = rand() % BUF_SIZE;
#else
		idx_arr[i] = i * CACHE_LINE; // technically should mod BUF_SIZE....
#endif

	// pre-fault
	for(int i = 0; i < BUF_SIZE; i += PAGE_SIZE)
		buf[i] = 0;

	clock_gettime(CLOCK_REALTIME, &wall);
	char file_name[64];
	sprintf(file_name, "%li", wall.tv_sec);
	int measure_log_fd = open(file_name, O_WRONLY | O_CREAT, 0664);
	if (measure_log_fd < 0) {
		printf("open errno: %i\n", errno);
		return -errno;
	}

	for(int iteration = 0; iteration < ITERATIONS; ++iteration) {
		clock_t iteration_userspace = clock();
		int i = 0;
		while(i < (MEASUREMENTS_PER_ITER * MEASUREMENT_ACCESSES)) {
			for(int j = i; j < i + MEASUREMENT_ACCESSES; ++j) {
				buf[idx_arr[j]] += 1;
			}
			i += MEASUREMENT_ACCESSES;

			userspace = clock();
			clock_gettime(CLOCK_REALTIME, &wall);

			measurements[measurement_idx++] = wall.tv_sec;
			measurements[measurement_idx++] = wall.tv_nsec;
			measurements[measurement_idx++] = userspace;
#if 0
			printf("wall time %lu s\n", measurements[measurement_idx - 3]);
			printf("clock() time: %lu s\n", measurements[measurement_idx - 1]);
#endif

		}
		measurement_idx = 0;
		printf("iter clock() time: %lu s\n", clock() - iteration_userspace);
		int rc = write(measure_log_fd, measurements, sizeof(measurements));
		if (rc == -1) {
			printf("write failed with %i\n", errno);
			return -errno;
		}
		printf("iter: %i\n", iteration);
	}
   	return 0;
}
