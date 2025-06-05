#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define HASH_TABLE_SIZE 10

typedef struct Node {
    char *key;
    struct Node *next;
} Node;

typedef struct HashSet {
    Node *buckets[HASH_TABLE_SIZE];
} HashSet;

// Simple hash function (for strings)
unsigned int hash(const char *key) {
    unsigned int hashValue = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        hashValue = hashValue * 31 + key[i];
    }
    return hashValue % HASH_TABLE_SIZE;
}

// Create a new hash set
HashSet *hashSetCreate() {
    HashSet *set = (HashSet *)malloc(sizeof(HashSet));
    if (set != NULL) {
        for (int i = 0; i < HASH_TABLE_SIZE; i++) {
            set->buckets[i] = NULL;
        }
    }
    return set;
}

// Add an element to the hash set
void hashSetAdd(HashSet *set, const char *key) {
    if (set == NULL || key == NULL) return;

    unsigned int index = hash(key);
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode != NULL) {
        newNode->key = strdup(key);
        newNode->next = set->buckets[index];
        set->buckets[index] = newNode;
    }
}

// Check if an element is in the hash set
bool hashSetContains(HashSet *set, const char *key) {
    if (set == NULL || key == NULL) return false;

    unsigned int index = hash(key);
    Node *current = set->buckets[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return true;
        }
        current = current->next;
    }
    return false;
}

// Destroy the hash set
void hashSetDestroy(HashSet *set) {
    if (set != NULL) {
        for (int i = 0; i < HASH_TABLE_SIZE; i++) {
            Node *current = set->buckets[i];
            while (current != NULL) {
                Node *temp = current;
                current = current->next;
                free(temp->key);
                free(temp);
            }
        }
        free(set);
    }
}

// Example usage
int main() {
    HashSet *set = hashSetCreate();
    hashSetAdd(set, "apple");
    hashSetAdd(set, "banana");
    hashSetAdd(set, "cherry");

    printf("Contains 'banana': %s\n", hashSetContains(set, "banana") ? "true" : "false");
    printf("Contains 'grape': %s\n", hashSetContains(set, "grape") ? "true" : "false");

    hashSetDestroy(set);
    return 0;
}
