#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "map_stp.h"

typedef enum {LESS, GREATER, EQUAL} compare;

typedef struct key_data {
	struct key_data *left;
	struct key_data *right;
	char            *id_str;
	int              id_len;
	void            *data;
} key_data;

compare
string_compare(char *astr, int alen, char *bstr, int blen) {
	int i;

	if(alen < blen) return LESS;
	if(alen > blen) return GREATER;
	for(i = 0; i < alen; i++) {
		if(astr[i] < bstr[i]) return LESS;
		if(astr[i] > bstr[i]) return GREATER;
	}
	return EQUAL;
}

void
msp_init(msp_data *map) {
	*map = NULL;
}

void
dump_node(key_data *node) {
	int i;

	if(!node) return;

	if(node->left) dump_node(node->left);
	
	for(i = 0; i < node->id_len; i++) {
		printf("%02X", node->id_str[i]);
	}
	printf(" = %p\n", node->data);

	if(node->right) dump_node(node->right);
}

void
msp_dump(msp_data *map) {
	dump_node(*map);
}

void
foreach(key_data *node, void (*func)(void *, int, void *)) {
	if(!node) return;
	if(node->left) foreach(node->left, func);
	func(node->id_str, node->id_len, node->data);
	if(node->right) foreach(node->right, func);
}

void
msp_foreach(msp_data *map, void (*func)(void *, int, void *)) {
	foreach(*map, func);
}

void
del_node(key_data *node) {
	if(node->left) del_node(node->left);
	if(node->right) del_node(node->right);
	free(node->id_str);
	free(node);
}

void
msp_erase(msp_data *map) {
	if(*map) del_node(*map);
}

key_data *
new_node(void *id_str, int id_len, void *data) {
	key_data *node;

	node = calloc(1, sizeof(key_data));
	if(!node) return NULL;

	node->id_str = malloc(id_len);
	if(!node->id_str) {
		free(node);
		return NULL;
	}
	memcpy(node->id_str, id_str, id_len);
	node->id_len = id_len;
	node->data = data;
	return node;
}

void
locate(void *idstr, int idlen, key_data **node, key_data **parent) {
	key_data *last, *cursor;

	last = *parent;
	cursor = *parent;
	while(cursor) {
		switch(string_compare(idstr, idlen, cursor->id_str, cursor->id_len)) {
			case LESS:
				last = cursor;
				cursor = cursor->left;
				continue;
			case GREATER:
				last = cursor;
				cursor = cursor->right;
				continue;
			case EQUAL:
				*node = cursor;
				*parent = last;
				return;
			default:
				return;
		}
	}
	*node = cursor;
	*parent = last;
}

int
insert_node(msp_data *map, key_data *parent, key_data *node) {
	key_data *find;


	if(*map == NULL) { // we have an empty map
		*map = node;
		return 0;
	}

	locate(node->id_str, node->id_len, &find, &parent);
	if(find) { // map collision
		return -1;
	}

	switch(string_compare(node->id_str, node->id_len, parent->id_str, parent->id_len)) {
		case LESS:
			if(parent->left != NULL) return -1; // left occupied
			parent->left = node;
			return 0;
		case GREATER:
			if(parent->right != NULL) return -1; // right occupied
			parent->right = node;
			return 0;
		default:
			return -1;
	}
}

int
msp_push(msp_data *map, void *idstr, int idlen, void *data) {
	key_data *node;

	node = new_node(idstr, idlen, data);
	if(!node) return 0;

	if(insert_node(map, *map, node)) {
		del_node(node);
		return 0;
	}

	return 1;
}

void *
msp_pop(msp_data *map, void *idstr, int idlen) {
	key_data *parent, *node, *left, *right;
	void *data;

	parent = *map;

	// find the node
	locate(idstr, idlen, &node, &parent);
	if(node == NULL) return NULL;

	// copy node components
	data = node->data;
	left = node->left;
	right = node->right;

	// remove the node from the tree
	if(*map == node) { // it's the top!
		*map = parent = NULL;
	} else { // its somewhere else!
		if(parent->left == node) parent->left = NULL;
		else if(parent->right == node) parent->right = NULL;
	}

	// free the removed node
	node->left = NULL;
	node->right = NULL;
	del_node(node);

	// insert orphaned nodes
	if(left) {
		if(insert_node(map, parent, left)) {
			del_node(left);
		} else {
			// top was reassigned, copy value to parent
			if(parent == NULL && *map != NULL) {
				parent = *map;
			}
		}
	}
	if(right) {
		if(insert_node(map, parent, right)) {
			del_node(right);
		}
	}
	return data;
}

void *
msp_find(msp_data *map, void *idstr, int idlen) {
	key_data *parent, *node;

	parent = *map;

	locate(idstr, idlen, &node, &parent);

	if(node) return node->data;
	return NULL;
}
