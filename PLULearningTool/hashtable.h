#include "input.h"

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

char* userInputItem(PLUTable* table, char* itemName) {
    char choice = '\0';
    int* plu = NULL;
    double* price = NULL;

    printf("\nERROR: Cannot find this item to put in our database. Would you like to add it to your cart anyway? (y/n)");
    switch (choice = getch()) {
    case 'y':
        getInteger("What is the PLU of your item?", plu);
        getDouble("What is the price of your item?", price);
        insertItemToTable(table, itemName, *plu, *price);

        return itemName;
    case 'n':
        return NULL;
    }
}

char* searchForItemInformation(PLUTable* table, char* itemName) {
    
    int hash = generateHash(itemName);

    if (table->table[hash] == NULL) {
        return userInputItem(table, itemName);
    }

    Item* current = table->table[hash];

    while (current != NULL) {
        if (strcmp(current->name, itemName) == 0) {
            return current->name;
        }

        current = current->NextItemValuePair;
    }

    return userInputItem(table, itemName);
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

