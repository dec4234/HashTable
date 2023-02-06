//
// Created by decar on 1/28/2023.
//

#ifndef HASHTABLE_HASH_TABLE_H
#define HASHTABLE_HASH_TABLE_H

typedef struct {
    char* key;
    char* value;
} ht_item;

typedef struct {
    int size;
    int baseSize;
    int count;
    ht_item** items;
} ht_hash_table;

void ht_insert(ht_hash_table* ht, const char* key, const char* value);
char* ht_search(ht_hash_table* ht, const char* key);
void ht_delete(ht_hash_table* ht, const char* key);

#endif //HASHTABLE_HASH_TABLE_H
