#ifndef __COMMAND__
#define __COMMAND__

#include <stddef.h>
#include "global.h"

typedef enum {
	FAIL = 0,
	SUCCESS
} RETURN;

typedef struct Command {
	char* name;
	int (*cmd_func)(char**);
} Command;

int exec_input(char**);

static int cmd_history(char**);

static int cmd_exit(char**) __attribute__ ((noreturn));

static int cmd_recent(char**);

const Command* find_cmd(char*);

#endif