#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#include "command.h"
#include "history.h"
#include "input.h"

#define SYNC_SYMBOL "&"

static const Command cmd[] = {
	{ "history", cmd_history },
	{ "exit", cmd_exit },
	{ "!", cmd_recent },
	{ NULL, NULL }
};

const Command* find_cmd(char* str) {
	int i;
	for (i = 0; cmd[i].name != NULL; i++) {
		if (strcmp(str, cmd[i].name) == 0)
			return &cmd[i];
	}
	return NULL;
}

static BOOL is_sync(char** arg) {
	int i;
	for (i = 0; arg[i]; i++) {
		if (strcmp(arg[i], SYNC_SYMBOL) == 0)
			return TRUE;
	}
	return FALSE;
}

int exec_input(char** arg) {
	pid_t pid;
	int ret;
	int status;
	const Command* cmd = find_cmd(arg[0]);
	if (cmd != NULL) {
		ret = (cmd->cmd_func)(arg);
		return ret;
	}
	BOOL sync_child = is_sync(arg);
	pid = fork();
	if (pid < 0) {
		printf("fork() failed\n");
		exit(0);
	}
	if (pid > 0) { 
		if (!sync_child)
			while (wait(&status) != pid)
				;
	} else {
		if (execvp(arg[0], arg) == -1) {
			printf("%s fail, errno: %d\n", arg[0], errno);
			exit(errno);
		}
	}
	return SUCCESS;
}

int cmd_history(__attribute__((unused))char** arg) {
	prt_history(&g_history);
	return SUCCESS;
}

int cmd_exit(__attribute__((unused))char** arg) /* no return */ {
	exit(0);
}

int cmd_recent(char** arg) {
	int i;
	if (strcmp(arg[1], "!") == 0)
		i = 1;
	else 
		sscanf(arg[1], "%d", &i);
	--i;
	const char* last = get_recent(&g_history, i);
	if (last)
		set_repeat(&g_history, i);
	return SUCCESS;
}