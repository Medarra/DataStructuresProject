#pragma warning (disable: 4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

# define SIZE 20
#define NAME_LENGTH 50
#define CHANGE_NUMBER 7         // Represents the number of different coins/bills that can be given
                                // as change to the customer (dimes, quarters, etc.)

typedef struct Item {
    char name[NAME_LENGTH];
    int PLU;
    double price;
    struct Item* NextItemValuePair;
} Item;

typedef struct CartItem {
    char name[NAME_LENGTH];
    double weight;
} CartItem;

//used by the results screen to show mistakes
typedef struct Result {
    Item* itemPTR;
    int incorrectPLU;
} Result;


/*-----------------------------------------HASH TABLE--------------------------------------------*/

typedef struct PLUTable {
    Item* table[SIZE];
} PLUTable;

int generateHash(char* itemName) {
    int hash = 0;
    for (int i = 0; itemName[i] != '\0'; i++) {
        int asciiValue = itemName[i];
        hash = (hash + asciiValue)  % SIZE;
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



//==== = STACK==== =
//
//A cart filled with CartItems
//
//- contains exactly 10 items





//==== = QUEUE==== =
//
//Cashier conveyor filled with CartItems
//
//- holds up to 5 items

// function prototypes
int makeChange(double, double);
void fillLookupTable(PLUTable*);

int main(void)
{
    char choice = '\0';
    PLUTable* lookupTable = initializePLUTable();
    fillLookupTable(lookupTable);

    do
    {
        system("CLS");
        printf("PLU Learning Tool\n");
        printf("=================\n\n");
        printf("1. Create Testing Cart\n");
        printf("1. Play Testing Session\n");
        printf("3. View PLU Codes\n");
        printf("4. Exit\n");


        switch (choice = getch()) {
        case '1': /*--Create a Cart for testing--*/
            if (makeChange(25, 47.20) == 0) {
                printf("good\n");
            }
            else {
                printf("bad\n");
            }
            getch();
            break;
        case '2': /*--Play test session--*/
            break;
        case '3': /*--View PLU Codes--*/
            printPLUSheet(lookupTable);
            getch();
            break;
        }
    } while (choice != '4');

    // Free resources 
    free(lookupTable);
    return 0;
}

void fillLookupTable(PLUTable* lookupTable) {
    FILE* database = NULL;
    if ((database = fopen("Database.txt", "r")) == NULL) {
        printf("An error occured while attempting to open the database file...");
        exit(EXIT_FAILURE);
    }

    while (!feof(database)) {
        char data[NAME_LENGTH] = { 0 };
        if (fgets(data, NAME_LENGTH - 1, database) == NULL) {
            continue;
        }

        // create temporary parsing variables
        char name[NAME_LENGTH] = { 0 };
        char pluTemp[NAME_LENGTH] = { 0 };

        char* firstSplit = strchr(data, '|');
        char* secondSplit = strrchr(data, '|');

        strncpy(name, data, (firstSplit - data));
        strncpy(pluTemp, firstSplit + 1, (secondSplit - firstSplit));
        pluTemp[NAME_LENGTH - 1] = '\0';

        insertItemToTable(lookupTable, name, atoi(pluTemp), atof(secondSplit + 1));
    }

    if ((fclose(database)) != 0) {
        printf("An error occured while attempting to close file");
        exit(EXIT_FAILURE);
    }
}

int makeChange(double cost, double received) {
    char userChoice = '\0';

    // Setup for change calculation
    const char* changeTypes[10] = { "Nickels", "Dimes", "Quarters", "Loonies", "Toonies", "5's", "10's" };
    double changeAmount[CHANGE_NUMBER] = { 0.05, 0.10, 0.25, 1.00, 2.00, 5.00, 10.00 };
    int changeCounter[CHANGE_NUMBER] = { 0 };

    do {
        // Displays instructions to the user
        system("CLS");
        printf("Total Cart Cost: $%.2lf\nMoney Received from Customer: $%.2lf\n\n", cost, received);
        printf("Use the numberpad to select change.\nPress <ENTER> to finish and 'r' to reset.\n\n");
        for (int i = 0; i < CHANGE_NUMBER; i++) {
            printf("%d. Number of %s:\t%10d\n", (i + 1), changeTypes[i], changeCounter[i]);
        }

        // User input
        userChoice = getch();
        if (userChoice > '0' && userChoice < '8') {
            changeCounter[userChoice - '0' - 1]++;
        }
        else if (userChoice == 'r') {
            for (int i = 0; i < CHANGE_NUMBER; i++) {
                changeCounter[i] = 0;
            }
        }
    } while (userChoice != '\r');

    // Calculates total change given
    double change = 0;
    for (int i = 0; i < CHANGE_NUMBER; i++) {
        change += (changeAmount[i] * changeCounter[i]);
    }

    // Print and return result
    printf("\nChange Required: $%.2lf\n", received - cost);
    printf("Change Given: $%.2lf\n", change);
    return change == (received - cost) ? 0 : 1;
}