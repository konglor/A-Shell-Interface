#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"
#include "global.h"
#include "history.h"
#include "command.h"

#define DELIM " \n\a\r\t"

char* read_input(void) {
	char* in = malloc(sizeof(char) * MAX_LINE);
	if (!in) {
		printf("insufficient heap\n");
		exit(0);
	}
	fgets(in, MAX_LINE, stdin);
	in[strcspn(in, "\n")] = 0;

	return in;
}

char** pars_input(char* input) {
	char** arg;
	char* input_c;
	char* t;
	int i = 0;
	arg = malloc(MAX_LINE * sizeof(char*));
	if(!arg) {
		printf("insufficient heap\n");
		exit(0);
	}
	input_c = malloc(MAX_LINE * sizeof(char*));
	if(!input_c) {
		printf("insufficient heap\n");
		exit(0);
	}
	strncpy(input_c, input, MAX_LINE); /* copy */
	if (input[0] == '!') {
		arg[i++] = "!";
		t = strtok(input + 1, DELIM);
		while (t != NULL) {
			arg[i++] = t;
			t = strtok(NULL, DELIM);
		}
	} else {
		t = strtok(input, DELIM);
		while (t != NULL) {
			arg[i++] = t;
			t = strtok(NULL, DELIM);
		}
	}
	arg[i] = 0;

#ifdef LOG_HISTORY
	if (!find_cmd(arg[0]))
		add_history(&g_history, input_c);
#endif

	free(input_c);
	return arg;
}

char* load_input(void) {
	char* prev = malloc(sizeof(char) * MAX_LINE);
	if (!prev) {
		printf("insufficient heap\n");
		exit(0);
	}
	unsigned int n = get_nth(&g_history);
	const char* input = get_recent(&g_history, n);

	strncpy(prev, input, MAX_LINE);
	printf("%s\n", prev);

	set_norepeat(&g_history);
	return prev;
}