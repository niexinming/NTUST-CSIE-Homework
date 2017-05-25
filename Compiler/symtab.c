#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symtab.h"


SYMTAB* symtab_create(SYMTAB *parent)
{
	SYMTAB *symtab = malloc(sizeof(SYMTAB));
	memset(symtab, 0, sizeof(SYMTAB));
	symtab->parent = parent;
	return symtab;
}

SYMTAB* symtab_destroy(SYMTAB * symtab)
{
	SYMTAB *parent = symtab->parent;
	SYMTAB_ENTRY *p, *n;
	for(int i = 0; i < SYMTAB_HASH_SIZE; i++) {
		p = symtab->entries[i];
		while(p) {
			n = p->next;
			free(p);
			p = n;
		}
	}
	free(symtab);
	return parent;
}

uint64_t symtab_hash_symbol(const char *s)
{
	uint64_t h = 0xfaceb00cdeadbeef;
	if(s != NULL)
		while(*s) {
			h = h * 0x777 ^ (h >> 3) ^ 0x555 * (uint8_t)*s++;
		}
	return h % SYMTAB_HASH_SIZE;
}

int symtab_insert(const char *s, void *meta,
				  SYMTAB *symtab, SYMTAB_ENTRY **out_entry)
{
	int id = symtab_lookup(s, symtab, 0, out_entry);
	if(id) return id;

	size_t len = strlen(s);
	uint64_t hash = symtab_hash_symbol(s);

	id = ++symtab->count;

	SYMTAB_ENTRY *entry = malloc(sizeof(SYMTAB_ENTRY) + len),
				 *prev = symtab->entries[hash];

	entry->id = id;
	entry-> meta = meta;
	entry->next = prev;
	memcpy(entry->name, s, len);
	entry->name[len] = 0;

	symtab->entries[hash] = entry;

	if(out_entry) {
		*out_entry = entry;
	}

	return id;
}

int symtab_lookup(const char * s, SYMTAB *symtab, int deep,
				  SYMTAB_ENTRY **out_entry)
{
	if(symtab == NULL) {
		return 0;
	}
	uint64_t slot = symtab_hash_symbol(s);
	SYMTAB_ENTRY *entry = symtab->entries[slot];
	while(entry) {
		if(strcmp(s, entry->name) == 0) {
			if(out_entry) {
				*out_entry = entry;
			}
			return entry->id;
		}
		entry = entry->next;
	}
	if(deep) {
		return symtab_lookup(s, symtab->parent, deep, out_entry);
	}
	return 0;
}

int symtab_depth(SYMTAB *symtab)
{
	int d = 0;
	while(symtab) {
		symtab = symtab->parent;
		d++;
	}
	return d;
}

void symtab_dump(SYMTAB *symtab)
{
	int i, depth = symtab_depth(symtab);
	SYMTAB_ENTRY *p;

	printf("  depth |  id | symbol\n"
		   " -------+-----+-------------------------\n");

	while(symtab) {
		for(i = 0; i < SYMTAB_HASH_SIZE; i++) {
			p = symtab->entries[i];
			while(p) {
				printf(" %6d | %5d | %s\n", depth, p->id, p->name);
				p = p->next;
			}
		}
		depth--;
		symtab = symtab->parent;
	}
}
