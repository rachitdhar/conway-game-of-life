#ifndef HELPER_H
#define HELPER_H

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#define SET_INIT_CAPACITY 2
#define QUEUE_INIT_CAPACITY 2

#define KEY_LEFT	0x25
#define KEY_UP		0x26
#define KEY_RIGHT	0x27
#define KEY_DOWN	0x28

typedef struct {
	int* items;
	size_t size;
	size_t capacity;
} set;

struct Node {
	int x;
	int y;
	struct Node* next;
};

typedef struct Node Node;
typedef enum { FALSE, TRUE } bool;

Node* linked_list_append(Node* head, int x, int y);
void linked_list_remove_back(Node* head);
bool linked_list_contains(Node* head, int x, int y);
void free_linked_list(Node* head);
void set_init(set* s);
int set_insert(set* s, int item);
int set_remove(set* s, int item);
void multi_concat(char* dest, size_t buffer_size, size_t count, ...);
bool is_arrow_key(unsigned long long wParam);
int generate_random_int(int lower_limit, int upper_limit);

#endif