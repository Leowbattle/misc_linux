#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/inotify.h>

int main(int argc, const char* argv[]) {
	int fd = inotify_init();

	const char* dir;
	if (argc < 2) {
		dir = ".";
	}
	else {
		dir = argv[1];
	}
	int wd = inotify_add_watch(fd, dir, IN_ALL_EVENTS);

	char buf[1024];

	for (;;) {
		int ret = read(fd, buf, sizeof(buf));

		if (ret == -1) {
			printf("%s", strerror(errno));
			exit(-1);
		}

		int i = 0;
		while (i < ret) {
			struct inotify_event* e = (struct inotify_event*)&buf[i];

			char buf2[1024] = {0};
			int len = 0;

			uint32_t mask = e->mask;
			#define X(x) \
			if (mask & x) { \
				const char* str = #x " "; \
				strncat(buf2, str, sizeof(buf2) - len); \
				len += strlen(str);\
			}

			X(IN_ACCESS)
			X(IN_ATTRIB)
			X(IN_CLOSE_WRITE)
			X(IN_CLOSE_NOWRITE)
			X(IN_CREATE)
			X(IN_DELETE)
			X(IN_DELETE_SELF)
			X(IN_MODIFY)
			X(IN_MOVE_SELF)
			X(IN_MOVED_FROM)
			X(IN_MOVED_TO)
			X(IN_OPEN)

			#undef X
			
			char* name = "";
			if (e->len > 0) {
				name = e->name;
			}
			printf("%s%s\n", buf2, name);

			i += sizeof(struct inotify_event) + e->len;
		}
	}

	return 0;
}