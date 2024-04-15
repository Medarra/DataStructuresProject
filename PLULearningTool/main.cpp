#include "hashtable.h"
#include "input.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define CHANGE_NUMBER 7         // Represents the number of different coins/bills that can be given
// as change to the customer (dimes, quarters, etc.)

typedef struct CartItem {
    char name[NAME_LENGTH];
    double weight;
} CartItem;

//used by the results screen to show mistakes
typedef struct Result {
    Item* itemPTR;
    int incorrectPLU;
} Result;



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
void fillLookupTable(PLUTable*);
double caculateBill(PLUTable* lookupTable, CartItem* queue[]);
int makeChange(double, double);
void playtest(CartItem* stack[], CartItem* queue[]);


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
            printf(makeChange(25, 47.20) == 0 ? "good\n" : "bad\n");
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

double caculateBill(PLUTable* lookupTable, CartItem* queue[])
{
    CartItem* ptr;
    char name[NAME_LENGTH];
    double itemPrice = 0.0;
    double totalBill = 0.0;

    while (!isQueueEmpty()) // Call isQueueEmpty function to check if the queue is empty
    {
        getString("please enter the name of the items", name); // Use strcpy for string copying
        ptr = queuePop(queue);   // Get an element from the queue

        if (ptr == NULL) {
            continue; // Skip if ptr is NULL
        }

        if (strcmp(name, ptr->name) == 0)
        {
            itemPrice = searchPriceByName(lookupTable , name);
            printf("Item: %s,  price: %f ", ptr->name, itemPrice * ptr->weight);
            totalBill += itemPrice * ptr->weight;
        }
        else
        {
            // Re-enter the name and compare again
            while (strcmp(name, ptr->name) != 0)
            {
                getString("Name does not match. Please enter the correct name: ", name); // Re-enter the name
            }

            // If the name matches, calculate the price again
            itemPrice = searchPriceByName(lookupTable, name);
            totalBill += itemPrice * ptr->weight;
            printf("Item: %s,  price: %f ", ptr->name, itemPrice * ptr->weight);
        }
    }
    return totalBill;
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

void playtest(CartItem* stack[], CartItem* queue[])
{
    CartItem* ptr;
    double totalBill = 0.0;

    if (isStackEmpty()) {
        printf("nothing on the cart!")
            return -1;
    }

    while (!isQueuefull() && !isQueueEmpty())
    {
        ptr = StackPop(stack);
        QueuePush(queue, ptr);

        if (isQueuefull())
        {
            totalBill += caculateBill(queue);
        }
    }

    printf(makeChange(totalBill, 47.20) == 0 ? "good\n" : "bad\n");
}
