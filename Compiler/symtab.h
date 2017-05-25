#ifndef SYMTAB_H
#define SYMTAB_H

#define SYMTAB_HASH_SIZE 4096

typedef struct SYMTAB_ENTRY_s {
	struct SYMTAB_ENTRY_s *next;
	void                  *meta;
	int  id;
	char name[1];
} SYMTAB_ENTRY;

typedef struct SYMTAB_s {
	struct SYMTAB_s       *parent;
	int                   count;
	struct SYMTAB_ENTRY_s *entries[SYMTAB_HASH_SIZE];
} SYMTAB;

SYMTAB* symtab_create(SYMTAB *);
SYMTAB* symtab_destroy(SYMTAB *);
int     symtab_insert(const char *, void *, SYMTAB *, SYMTAB_ENTRY **);
int     symtab_lookup(const char *, SYMTAB *, int, SYMTAB_ENTRY **);
int     symtab_depth(SYMTAB *);
void    symtab_dump(SYMTAB *);

#endif
