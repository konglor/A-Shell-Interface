#ifndef __HISTORY__
#define __HISTORY__

#define LOG_HISTORY

#include <stddef.h>
#include "global.h"

typedef struct {
	BOOL is;
	unsigned int nth;
} Repeat;

typedef volatile struct {
	int head;
	int tail;
	size_t size;
	size_t capacity;
	char* data[MAX_HISTORY];
	Repeat next;
} History;

void add_history(History*, char*);

void prt_history(History*);

const char* get_recent(History*, int);

const char* get_last(History*);

void set_repeat(History*, unsigned int);

void set_norepeat(History*);

BOOL is_repeat(History*);

unsigned int get_nth(History*);

extern History g_history;

#endif