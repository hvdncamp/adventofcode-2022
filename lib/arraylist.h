#ifndef ARRAY_LIST
#define ARRAY_LIST
typedef struct arraylist {
	int capacity;
	int size;
	void **items;
} ArrayList;

ArrayList * list();
void * get_list(ArrayList *this, int index);
void add_list(ArrayList *this, void *item);
void * fold_list(ArrayList *this, void *init, void * (*f)(void *acc, int index, void *item));
void * free_listitem(void *acc, int index, void *item);
void free_list(ArrayList *this);
#endif
