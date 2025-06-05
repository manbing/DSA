#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 10

typedef struct Node {
    char* key;
    int value;
    struct Node* next;
} Node;

Node* hashTable[TABLE_SIZE];

// Simple hash function
unsigned int hash(char* key) {
    unsigned int hashVal = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        hashVal = 31 * hashVal + key[i];
    }
    return hashVal % TABLE_SIZE;
}

void insert(char* key, int value) {
    unsigned int index = hash(key);
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->key = strdup(key);
    newNode->value = value;
    newNode->next = hashTable[index];
    hashTable[index] = newNode;
}

Node* get(char* key) {
    unsigned int index = hash(key);
    Node* current = hashTable[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

int main() {
    insert("apple", 1);
    insert("banana", 2);
    insert("cherry", 3);
    insert("apricot", 4);

    Node* found = get("banana");
    if (found != NULL) {
        printf("Found: %s, Value: %d\n", found->key, found->value);
    }
    return 0;
}
