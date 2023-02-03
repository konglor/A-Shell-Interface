#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "history.h"
#include "global.h"

History g_history = {
	.head = 0,
	.tail = MAX_HISTORY - 1,
	.size = 0,
	.capacity = MAX_HISTORY,
	.data = {0},
	.next = {
		.is = FALSE,
		.nth = 0
	}
};

static BOOL is_full(History* h) {
	return ((h->size == h->capacity) ? TRUE : FALSE);
}

static void make_room(History* h) {
	free(h->data[h->head]);
	h->data[h->head] = NULL;
	h->head = (h->head + 1) % h->capacity;
	h->size--;
}

void add_history(History* h, char* entry) {
	char* in;
	if (is_full(h))
		make_room(h);

	in = malloc(sizeof(char) * MAX_LINE);
	if (!in) {
		printf("insufficient heap\n");
		exit(0);
	}
	strncpy(in, entry, MAX_LINE);
	h->tail = (h->tail + 1) % h->capacity;
	if (h->data[h->tail] != NULL)
		free(h->data[h->tail]);

	h->data[h->tail] = in;
	h->size++;
}

void prt_history(History* h) {
	int i = h->head;
	int index = h->size;
	do {
		printf("%d %s\n", index, h->data[i]);
		i = (i + 1) % h->capacity;
		if (h->data[i] == NULL)
			break;
		index--;
	} while (i != h->head);
}

const char* get_recent(History* h, int nth) {
	assert(nth >= 0 && nth <= h->size);
	if (nth >= 0 && nth <= h->size) {
		int i = (h->tail - nth) % h->capacity;
		return h->data[i];
	}
	return NULL;
}

const char* get_last(History* h) {
	return get_recent(h, 0);
}

void set_repeat(History* h, unsigned int n) {
	assert(n >= 0 && n <= h->size);
	h->next.is = TRUE;
	h->next.nth = n;
}

void set_norepeat(History* h) {
	h->next.is = FALSE;
}

BOOL is_repeat(History* h) {
	return h->next.is;
}

unsigned int get_nth(History* h) {
	return h->next.nth;
}