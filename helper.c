#include "helper.h"

Node* linked_list_append(Node* head, int x, int y)
{
	Node* new_node = (Node*)malloc(sizeof(Node));
	new_node->x = x;
	new_node->y = y;
	new_node->next = head;
	return new_node;
}

void linked_list_remove_back(Node* head)
{
	Node *l = head, *r = head->next;
	if (r == NULL) return;

	while (r->next != NULL) {
		l = r;
		r = r->next;
	}
	free(r);
	l->next = NULL;
}

bool linked_list_contains(Node* head, int x, int y)
{
	Node* n = head;
	do {
		if (n->x == x && n->y == y)
			return TRUE;
		n = n->next;
	} while (n != NULL);
	
	return FALSE;
}

void free_linked_list(Node* head)
{
	Node* n = head;
	do {
		Node* temp = n->next;
		free(n);
		n = temp;
	} while (n != NULL);
}

void set_init(set* s)
{
	s->items = (int*)malloc(SET_INIT_CAPACITY * sizeof(int));
	s->size = 0;
	s->capacity = SET_INIT_CAPACITY;
}

int set_insert(set* s, int item)
{
	// check if item already exists
	for (int i = 0; i < s->size; i++)
		if (s->items[i] == item) return 1;

	if (s->size == s->capacity) {
		int* items_new = (int*)malloc(2 * s->capacity * sizeof(int));

		for (int i = 0; i < s->size; i++)
			items_new[i] = s->items[i];

		free(s->items);
		s->items = items_new;
		s->capacity *= 2;
	}
	s->items[s->size++] = item;
	return 0;
}

int set_remove(set* s, int item)
{
	int index = -1;
	for (int i = 0; i < s->size; i++)
	{
		if (s->items[i] == item) {
			index = i;
			break;
		}
	}

	if (index != -1) {
		for (int i = index; i < s->size - 1; i++)
			s->items[i] = s->items[i + 1];

		s->size--;
		return 0;
	}
	return 1;
}

void multi_concat(char* dest, size_t buffer_size, size_t count, ...)
{
	va_list args;
	va_start(args, count);

	char* curr = va_arg(args, char*);
	for (size_t c = 0; c < count; c++)
	{
		strcat_s(dest, buffer_size, curr);
		curr = va_arg(args, char*);
	}
	va_end(args);
}

bool is_arrow_key(unsigned long long wParam)
{
	return (wParam == KEY_LEFT || wParam == KEY_RIGHT || wParam == KEY_UP || wParam == KEY_DOWN);
}

int generate_random_int(int lower_limit, int upper_limit)
{
	if (lower_limit > upper_limit) return -1;
	return lower_limit + rand() % (upper_limit - lower_limit + 1);
}