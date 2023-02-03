#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "input.h"
#include "history.h"
#include "command.h"

#define OSH(FUNC) FUNC ## _input

int main(int argc, char** argv) {
	int should_run = 1;
	char* inp;
	char** arg;
	int ret;

	while (should_run) {
		printf("osh>");
		fflush(stdout);

		inp = (is_repeat(&g_history) ?
			  load_input()
			: read_input());
		arg = pars_input(inp);
		ret = exec_input(arg);

		free(inp);
		free(arg);
	}
	return 0;
}

