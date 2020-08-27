#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdint.h>

struct bob {
	uint64_t x;
	uint64_t y;
	uint8_t str[32];
	uint64_t z;
} __attribute((packed));

int main(int argc, char **argv) {
	int sv[2];
	int ret;

	struct bob b = {
		.x = 111111111,
		.y = 222222222,
		.str = "ummagumma",
		.z = 33333333,
	};

	ret = socketpair(AF_UNIX, SOCK_DGRAM, 0, sv);
	printf("socket pair returned: %i\n", ret);
	printf("got socket fds: %i and %i\n", sv[0], sv[1]);

	if (!fork()) {
		execl("./rust-side/target/debug/rust-side", "rust-side", NULL);
	}

	send(sv[0], &b, sizeof(b), 0);

	printf("Init's pid is %i\n", getpid());
	printf("Init's job is done. Kill me whenever\n");
	for(;;) {
		sleep(1);
	}
}

