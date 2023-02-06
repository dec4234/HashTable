//
// Created by decar on 1/28/2023.
//
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "xmalloc.h"

#include "hash_table.h"
#include "prime.h"

// https://github.com/jamesroutley/write-a-hash-table/tree/master/06-resizing

static ht_item HT_DELETED_ITEM = {NULL, NULL};
const int START_SIZE = 20;

static ht_item* ht_new_item(const char* k, const char* v) {
    ht_item* i = malloc(sizeof(ht_item));
    i->key = strdup(k);
    i->value = strdup(v);
    return i;
}

static void ht_del_item(ht_item* i) {
    free(i->key);
    free(i->value);
    free(i);
}

ht_hash_table* ht_new_sized(const int baseSize) {
    ht_hash_table* ht = xmalloc(sizeof(ht_hash_table));
    ht->baseSize = baseSize;

    ht->size = next_prime(ht->baseSize);

    ht->count = 0;
    ht->items = xcalloc((size_t) ht->size, sizeof(ht_item*));
    return ht;
}

ht_hash_table* ht_new() {
    return ht_new_sized(START_SIZE);
}

void ht_del_hash_table(ht_hash_table* ht) {
    for(int i = 0; i < ht->size; i++) {
        ht_item* item = ht->items[i];
        if(item != NULL) {
            ht_del_item(item);
        }
    }

    free(ht->items);
    free(ht);
}

static int ht_hash(const char* s, const int a, const int m) {
    long hash = 0;
    const int len_s = strlen(s);

    for(int i = 0; i < len_s; i++) {
        hash += (long) pow(a, len_s - (i + 1)) * s[i];
        hash = hash % m;
    }

    return (int) hash;
}

static int ht_get_hash(const char* s, const int numBuckets, const int attempt) {
    const int hash_a = ht_hash(s, 137, numBuckets);
    const int hash_b = ht_hash(s, 157, numBuckets);

    return (hash_a + (attempt * (hash_b + 1))) % numBuckets;
}

static void ht_resize(ht_hash_table* ht, const int baseSize) {
    if(baseSize < START_SIZE) {
        return;
    }

    ht_hash_table* newHt = ht_new_sized(baseSize);
    for(int i = 0; i < ht->size; i++) {
        ht_item* item = ht->items[i];

        if(item != NULL && item != &HT_DELETED_ITEM) {
            ht_insert(newHt, item->key, item->value);
        }
    }

    ht->baseSize = newHt->baseSize;
    ht->count = newHt->count;

    const int tempSize = newHt->size;
    ht->size = newHt->size;
    newHt->size = tempSize;

    ht_item** tempItems = ht->items;
    ht->items = newHt->items;
    newHt->items = tempItems;

    ht_del_hash_table(newHt);
}

static void ht_resize_up(ht_hash_table* ht) {
    const int newSize = ht->baseSize * 2;
    ht_resize(ht, newSize);
}

static void ht_resize_down(ht_hash_table* ht) {
    const int newSize = ht->baseSize / 2;
    ht_resize(ht, newSize);
}

void ht_insert(ht_hash_table* ht, const char* key, const char* value) {
    const int load = ht->count * 100 / ht->size;
    if(load > 70) {
        ht_resize_up(ht);
    }

    ht_item* item = ht_new_item(key, value);
    int index = ht_get_hash(item->key, ht->size, 0);
    ht_item* currentItem = ht->items[index];

    int i = 1;
    while(currentItem != NULL) { // Search through the Hash Table until an empty bucket is found
        if(currentItem != &HT_DELETED_ITEM) {
            if(strcmp(currentItem->key, key) == 0) {
                ht_del_item(currentItem);
                ht->items[index] = item;
                return;
            }
        }
    }

    ht->items[index] = item; // Insert the item into the available bucket
    ht->count++;
}

char* ht_search(ht_hash_table* ht, const char* key) {
    int index = ht_get_hash(key, ht->size, 0);
    ht_item* item = ht->items[index];

    int i = 1;
    while(item != NULL) {
        if(item != &HT_DELETED_ITEM) {
            if(strcmp(item->key, key) == 0) {
                return item->value;
            }
        }

        index = ht_get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    }

    return NULL;
}

void ht_delete(ht_hash_table* ht, const char* key) {
    const int load = ht->count * 100 / ht->size;
    if(load < 10) {
        ht_resize_down(ht);
    }

    int index = ht_get_hash(key, ht->size, 0);
    ht_item* item = ht->items[index];

    int i = 1;
    while(item != NULL) {
        if(item != &HT_DELETED_ITEM) {
            if(strcmp(item->key, key) == 0) {
                ht_del_item(item);
                ht->items[index] = &HT_DELETED_ITEM;
            }
        }

        index = ht_get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    }

    ht->count--;
}

int main() {
    printf("Starting Hash Table program.\n");

    ht_hash_table* ht = ht_new();
    ht_new_item("key", "value");
    ht_del_hash_table(ht);

    return 0;
}
