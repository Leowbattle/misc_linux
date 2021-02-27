// A clone of tree

#define _DEFAULT_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <limits.h>
#include <errno.h>

struct state {
	char* path;
	int indentation;
	bool showHidden;
};

void print_dir(struct state state) {
	DIR* d = opendir(state.path);

	if (d == NULL) {
		printf("Cannot access %s: %s\n", state.path, strerror(errno));
		return;
	}

	struct dirent* ent;
	while ((ent = readdir(d)) != NULL) {
		if (!state.showHidden && ent->d_name[0] == '.') {
			continue;
		}

		for (int i = 0; i < state.indentation; i++) {
			printf("\t");
		}

		printf("%s\n", ent->d_name);

		if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
			continue;
		}

		if (ent->d_type == DT_DIR) {
			int rem = PATH_MAX;
			char* pathBuf = malloc(rem);
			memset(pathBuf, 0, rem);

			strncat(pathBuf, state.path, rem);
			rem -= strlen(state.path);

			strncat(pathBuf, "/", rem);
			rem -= 1;

			strncat(pathBuf, ent->d_name, rem);
			rem -= strlen(ent->d_name);

			struct state newState = {
				.path = pathBuf,
				.indentation = state.indentation + 1,
				.showHidden = state.showHidden
			};
			print_dir(newState);

			free(pathBuf);
		}
	}

	closedir(d);
}

int main(int argc, char* argv[]) {
	char* path = ".";
	if (argc > 1) {
		path = argv[1];
	}

	struct state state = {
		.path = path,
		.indentation = 0,
		.showHidden = false
	};

	print_dir(state);

	return 0;
}