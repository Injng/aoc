#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

const int FILE_LINES = 1000;

// entry for the hash table
typedef struct hash_entry {
    int32_t key;
    int32_t value;
} HashEntry;

// initialize a hash entry
HashEntry hcreate (int32_t key, int32_t value) {
    HashEntry entry;
    entry.key = key;
    entry.value = value;
    return entry;
}

// get the index for the hash table by hashing
int hindex(int32_t key) {
    return (key & 0x7FFFFFFF) % (FILE_LINES * 2);
}

// cleanup hash table
void hfree(HashEntry** table) {
    for (int i = 0; i < FILE_LINES * 2; i++) {
        if (table[i] != NULL) {
            free(table[i]);
        }
    }
}

// insert an entry into the hash table
int hinsert(HashEntry** table, HashEntry* entry) {
    int index = hindex(entry->key);
    while (table[index] != NULL && table[index]->key != entry->key) {
        index = (index + 1) % (FILE_LINES * 2);
    }
    table[index] = entry;
    return index;
}

// get an entry from the hash table
int32_t hget(HashEntry** table, int32_t key, bool* found) {
    int index = hindex(key);
    if (table[index] == NULL) {
        *found = false;
        return 0;
    }

    int count = 0;
    while (count <= FILE_LINES * 2) {
        if (table[index] == NULL) {
            index = (index + 1) % (FILE_LINES * 2);
            count++;
            continue;
        } else if (table[index]->key == key) {
            *found = true;
            return table[index]->value;
        }
        index = (index + 1) % (FILE_LINES * 2);
        count++;
    }
    *found = false;
    return 0;
}

int main(void) {
    // open file
    FILE* input_file = fopen("day1/day1.txt", "r");
    if (input_file == NULL) {
        printf("Error: cannot open input file\n");
        return 1;
    }

    // create hash table
    HashEntry* hash_table[FILE_LINES * 2];

    // fill hash table with null pointers to indicate empty slots
    for (int i = 0; i < FILE_LINES * 2; i++) {
        hash_table[i] = NULL;
    }

    // read in each line
    char line[15];
    int32_t left_ids[FILE_LINES];
    int count = 0;
    while (count < FILE_LINES) {
        // get the line and separate into ids
        fgets(&line[0], sizeof(line), input_file);
        int32_t left_id, right_id;
        sscanf(&line[0], "%d %d\n", &left_id, &right_id);

        // add the left id to an array
        left_ids[count] = left_id;

        // add the right id to the hash table, maintaining the count
        bool is_found;
        uint32_t hash_value = hget(&hash_table[0], right_id, &is_found);
        HashEntry* entry = malloc(sizeof(HashEntry));
        if (!is_found) {
            *entry = hcreate(right_id, 1);
        } else {
            *entry = hcreate(right_id, hash_value + 1);
        }
        hinsert(&hash_table[0], entry);
        count++;
    }

    // figure out similarity
    bool is_found;
    int similarity = 0;
    for (int i = 0; i < FILE_LINES; i++) {
        uint32_t hash_value = hget(&hash_table[0], left_ids[i], &is_found);
        if (is_found) {
            similarity += left_ids[i] * hash_value;
        }
    }

    printf("%d\n", similarity);
    hfree(&hash_table[0]);
}
