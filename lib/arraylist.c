#include "arraylist.h"
#include <stdlib.h>
#include <stdio.h>

ArrayList * list() {
	ArrayList *this = malloc(sizeof(ArrayList));
	this->capacity = 4;
	this->size = 0;
	this->items = malloc(this->capacity * sizeof(void *));
	return this;
}

void * get_list(ArrayList *this, int index) {
	if (index < 0 || this->size < index) {
		fprintf(stderr, "reading outside the bounds of an arraylist, index=%d, size=%d\n", index, this->size);
		exit(1);
	}
	return this->items[index];
}

void add_list(ArrayList *this, void *item) {
	if (this->size == this->capacity) {
		this->capacity *= 2;
		this->items = realloc(this->items, this->capacity * sizeof(void *));
	}
	this->items[this->size++] = item;
}

void * fold_list(ArrayList *this, void *init, void * (*f)(void *acc, int index, void *item)) {
	int i;
	for(i = 0; i < this->size; i++) {
		init = f(init, i, this->items[i]);
	}
	return init;
}

void * free_listitem(void *acc, int index, void *item) {
	free(item);
	return NULL;
}

void free_list(ArrayList *this) {
	free(this->items);
	free(this);
}
