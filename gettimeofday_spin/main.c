#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>

void gettimeofday_unwrap(struct timeval *tv) {
	int rc = gettimeofday(tv, NULL);
	if (rc) {
		printf("gettimeofday failed %i\n", rc);
		exit(1);
	}
}

signed long tv_to_usecs(struct timeval *tv) {
	return ((signed long ) tv->tv_sec * 1000 * 1000) + (signed long) tv->tv_usec;
}

int main(void) {
	struct timeval tv_old, tv_new;
	signed long usecs_delta;
	signed long longest_delta = 0;

	gettimeofday_unwrap(&tv_old);

	for(;;) {
		gettimeofday_unwrap(&tv_new);
		usecs_delta = tv_to_usecs(&tv_new) - tv_to_usecs(&tv_old);
		if (usecs_delta < 0) {
			printf("clock went bakwards at %s\n", ctime(&tv_new.tv_sec));
		}
		if (usecs_delta > longest_delta) {
			printf("new longest %li at %s\n",
					usecs_delta,
					ctime(&tv_new.tv_sec));
			longest_delta = usecs_delta;

		}
		tv_old = tv_new;
	}
}
