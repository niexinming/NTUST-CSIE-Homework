#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symtab.h"


SYMTAB* symtab_create()
{
	SYMTAB *symtab = malloc(sizeof(SYMTAB));
	memset(symtab, 0, sizeof(SYMTAB));
	return symtab;
}

void symtab_destroy(SYMTAB * symtab)
{
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

int symtab_insert(const char * s, SYMTAB *symtab)
{
	int id = symtab_lookup(s, symtab);
	if(id) return id;

	size_t len = strlen(s);
	uint64_t hash = symtab_hash_symbol(s);

	id = ++symtab->count;

	SYMTAB_ENTRY *entry = malloc(sizeof(SYMTAB_ENTRY) + len), *prev = symtab->entries[hash];

	entry->id = id;
	entry->next = prev;
	memcpy(entry->name, s, len);
	entry->name[len] = 0;

	symtab->entries[hash] = entry;

	return id;
}

int symtab_lookup(const char * s, SYMTAB *symtab)
{
	uint64_t slot = symtab_hash_symbol(s);
	SYMTAB_ENTRY *entry = symtab->entries[slot];
	while(entry) {
		if(strcmp(s, entry->name) == 0) {
			return entry->id;
		}
	}
	return 0;
}

void symtab_dump(SYMTAB *symtab)
{
	SYMTAB_ENTRY *p;
	printf("   id | symbol\n"
		   " -----+-------------------------\n");
	for(int i = 0; i < SYMTAB_HASH_SIZE; i++) {
		p = symtab->entries[i];
		while(p) {
			printf("%5d | %s\n", p->id, p->name);
			p = p->next;
		}
	}
}
