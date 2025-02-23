#include <stdio.h>

#ifndef H_HASH_TABLE
#define H_HASH_TABLE

typedef struct HashTable HashTable;
HashTable *ht_create(size_t size);
void *ht_get(HashTable *ht, char *key);
char ht_set(HashTable *ht, char *key, void *value);
char ht_has(HashTable *ht, char *key);

#endif