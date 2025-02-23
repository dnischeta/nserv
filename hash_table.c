#include <stdio.h>
#include <stdlib.h>

#include "hash_table.h"

// Problems/limitations:
//  - string premutations produce same hash
//  - different strings will produce same hash in case ASCII sum is similar
//  - hash calculation depends on hash table size (base param)
int hash(char *string, size_t base) {
    int hash = 0, idx = 0;

    while (string[idx] != 0) {
        hash += string[idx];
        idx++;
    }

    return hash % base;
}

typedef struct HashTable
{
    void **data;
    size_t size;
} HashTable;

HashTable *ht_create(size_t size) {
    void **data = malloc(sizeof(void *) * size);

    for (int idx = 0; idx < size; idx++) {
        data[idx] = 0;
    }

    HashTable *ht = malloc(sizeof(HashTable));
    ht->data = data;
    ht->size = size;
    return ht;
}

char ht_set(HashTable *ht, char *key, void *value) {
    int idx = hash(key, ht->size);
    ht->data[idx] = value;
    return 1;
}

char ht_has(HashTable *ht, char *key) {
    int idx = hash(key, ht->size);
    return ht->data[idx] != 0;
}

void *ht_get(HashTable *ht, char *key) {
    int idx = hash(key, ht->size);
    if (ht->data[idx] == 0) {
        return NULL;
    }
    return ht->data[idx];
}
