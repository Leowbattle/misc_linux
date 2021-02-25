// This program shows how you can use mmap to control memory access.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>

int* ptr;

void action(int sig, siginfo_t* info, void* p) {
	printf("uh oh\n");

	mprotect(ptr, sysconf(_SC_PAGE_SIZE), PROT_READ | PROT_WRITE);
}

int main() {
	struct sigaction act = {0};
	act.sa_sigaction = action;

	sigaction(SIGSEGV, &act, NULL);

	ptr = mmap(NULL, sysconf(_SC_PAGE_SIZE), 0, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	ptr[0] = 0;

	// MAP_FIXED forces the kernel to map an address range starting at the first argument, in this case: NULL.
	// If you do this, NULL becomes a perfectly valid memory address to write to - no segfault.
	// This needs sudo priveliges though.

	// void* ptr = mmap(NULL, sysconf(_SC_PAGE_SIZE), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0);
	// printf("%p %s\n", ptr, strerror(errno));

	return 0;
}