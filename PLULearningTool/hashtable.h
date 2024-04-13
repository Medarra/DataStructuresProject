#pragma warning (disable: 4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#ifndef HASH_TABLE
#define HASH_TABLE

#define SIZE 20
#define NAME_LENGTH 50

typedef struct Item {
    char name[NAME_LENGTH];
    int PLU;
    double price;
    struct Item* NextItemValuePair;
} Item;

typedef struct PLUTable {
    Item* table[SIZE];
} PLUTable;

int generateHash(char* itemName) {
    int hash = 0;
    for (int i = 0; itemName[i] != '\0'; i++) {
        int asciiValue = itemName[i];
        hash = (hash + asciiValue) % SIZE;
    }
    return hash;
}

Item* initializeItem(char* nameofItem, int PLUOfItem, double priceOfItem) {
    Item* newItem = (Item*)malloc(sizeof(Item));
    if (newItem == NULL) {
        printf("Not enough memory!");
        exit(EXIT_FAILURE);
    }

    strcpy(newItem->name, nameofItem);
    newItem->PLU = PLUOfItem;
    newItem->price = priceOfItem;
    newItem->NextItemValuePair = NULL;

    return newItem;
}

PLUTable* initializePLUTable(void) {
    PLUTable* table = (PLUTable*)malloc(sizeof(PLUTable));
    if (table == NULL) {
        printf("Not enough memory!");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < SIZE; i++) {
        table->table[i] = NULL;
    }

    return table;
}

void insertItemToTable(PLUTable* table, char* nameOfItem, int PLUOfItem, double costOfItem) {
    int hash = generateHash(nameOfItem);

    Item* newItem = initializeItem(nameOfItem, PLUOfItem, costOfItem);

    if (table->table[hash] == NULL) {
        table->table[hash] = newItem;
        return;
    }

    Item* current = table->table[hash];
    while (current->NextItemValuePair != NULL) {
        current = current->NextItemValuePair;
    }

    current->NextItemValuePair = newItem;
}

Item* searchForItemInformation(PLUTable* table, char* itemName) {
    int hash = generateHash(itemName);

    if (table->table[hash] == NULL) {
        printf("ERROR: Cannot find the word!");
        return NULL;
    }

    Item* current = table->table[hash];

    while (current != NULL) {
        if (strcmp(current->name, itemName) == 0) {
            return current;
        }

        current = current->NextItemValuePair;
    }

    return NULL;
}

void printPLUSheet(PLUTable* table) {
    system("CLS");
    printf("PLU Cheat Sheet\n");
    printf("===================\n");
    for (int i = 0; i < SIZE; i++) {
        Item* current = table->table[i];
        while (current != NULL) {
            printf("%s ........................ %d\n", current->name, current->PLU);
            current = current->NextItemValuePair;
        }
    }
}

#endif