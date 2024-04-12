#pragma warning (disable: 4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

# define SIZE 20

typedef struct Item {
    char name[100];
    int PLU;
    double price;
    struct Item* NextItemValuePair;
} Item;

typedef struct CartItem {
    char name[100];
    double weight;
} CartItem;

typedef struct PLUTable {
    Item* table[SIZE];
};

//used by the results screen to show mistakes
typedef struct Result {
    Item* itemPTR;
    int incorrectPLU;
} Result;

//==== = Database.txt==== =
//
//Text file listing all Item data
//
//
//
//-Format -
//
//<Item Name(string)> | <PLU Code(int)> | <Price(double)>

int generateHash(char* itemName, int PLU) {
    int hash = 0;
    for (int i = 0; itemName[i] != '\0'; i++) {
        int asciiValue = itemName[i];
        hash = (hash + asciiValue + PLU) % SIZE;
    }
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
}

void insertItemToTable(PLUTable* table, char* nameOfItem, int PLUOfItem, double costOfItem) {
    int hash = generateHash(nameOfItem, PLUOfItem);

    Item* newItem = initializeItem(nameOfItem, PLUOfItem, costOfItem);

    if (table->table[hash] == NULL) {
        table->table[hash] = newItem;
        return;
    }

    Item* current = table->table[hash]

}




//==== = HASH TABLE==== =
//
//PLU lookup table filled with Items
//
//- 20 total item slots
//
//- Hash Function : (name(total ASCII value) + price) % 20 == NOT FINAL ==
//
//-implement separate chaining
//
//- items add be added through the program(add this at the end if we have time)
//
//- this will also require writing to the Items file





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



int main(void)
{
    char choice = '\0';

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
            break;
        case '2': /*--Play test session--*/
            break;
        case '3': /*--View PLU Codes--*/
            break;
        }
    } while (choice != '4');

    return 0;
}