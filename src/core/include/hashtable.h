#ifndef I_HASHTABLE_H
#define I_HASHTABLE_H

typedef struct hash_table hashtable;

struct ht_entry {
    struct ht_entry *next;
    struct e_reference *data;
    char *key;
    void *object;
};

struct ht_bucket {
    struct ht_entry *head;
    struct ht_entry *tail;
    unsigned int index;
};

struct hash_table {
    struct ht_bucket *table;
    unsigned int size;
    unsigned int free;
    // index for current position on ht_prime_numbers
    unsigned int i_prime_arr;
};

unsigned int ht_key_calc(char *key);
int ht_set_key(hashtable *ht, char *key, void *object);
struct ht_entry *ht_get(hashtable *ht, char *key);
struct ht_entry *ht_get_by_hash(hashtable *ht, char *key, unsigned int hash);
hashtable *ht_init(unsigned int size);
void ht_resize_table(hashtable *ht);
void ht_reindex(hashtable *ht);
unsigned int ht_find_best_size(unsigned int size);
void ht_destroy(hashtable *ht);
void ht_dump(hashtable *ht);

#endif //#define I_HASHTABLE_H