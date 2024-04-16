#include "input.h"
#include "hashtable.h"
#include "cartItem.h"

#include <time.h>
#include <stdbool.h>

#define CURRENCY_NUMBER 9       /* Represents the number of different coins/bills that can be given
                                   as change to the customer (dimes, quarters, etc.) */
#define MAX_TRIES       3       // Maximum number of tries for the user to guess the PLU

//used by the results screen to show mistakes
typedef struct Result {
    Item* itemPTR;
    int incorrectPLU;
} Result;

// function prototypes
void fillLookupTable(PLUTable*);
double scanItem(PLUTable* lookupTable, ConveyorBelt* conveyor, int* score);
bool makeChange(double, double);
double generateRandomPayment(double totalBill);
void playTest(PLUTable* lookupTable, Cart* cart, ConveyorBelt* conveyor);

int main(void) {
    char choice = '\0';
    char potentialItemName[MAX_INPUT] = { 0 };
    char* itemReturnName = NULL;
    double itemWeight = 0;
    PLUTable* lookupTable = initializePLUTable();
    Cart* cart = initializeCart();
    ConveyorBelt* conveyor = NULL;
    fillLookupTable(lookupTable);
    srand(time(NULL));        //

    do {
        system("CLS");
        printf("PLU Learning Tool\n");
        printf("=================\n\n");
        printf("1. Create Testing Cart\n");
        printf("2. Play Testing Session\n");
        printf("3. View PLU Codes\n");
        printf("4. Exit\n");


        switch (choice = getch()) {
        case '1': /*--Create a Cart for testing--*/
            if (!isCartEmpty(cart)) {
                // empty cart
            }
            system("CLS");
            printf("Let's fill this cart up so we can begin testing!");
            while (!isCartFull(cart)) {
                getString("\nPlease enter an item you would like to add to your cart.", potentialItemName);
                itemReturnName = searchForItemInformation(lookupTable, potentialItemName);
                if (itemReturnName == NULL) {
                    printf("\nUnable to add this item to the cart.");
                    continue;
                }
                getDouble("\nPlease enter the weight of this item.", &itemWeight);
                pushCart(cart, itemReturnName, itemWeight);
            }
            printf("\nYou now have a full cart! Let's head over to a testing session!");
            break;
        case '2': /*--Play test session--*/
            playTest(lookupTable, cart, conveyor);
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

double scanItem(PLUTable* lookupTable, ConveyorBelt* conveyor, int* score) {
    CartItem* ptr = NULL;
    int pluOnConveyer = 0;
    int pluToScan = 0;
    double itemPrice = 0;

    while (!isBeltEmpty(conveyor)) { // Call isQueueEmpty function to check if the queue is empty
        ptr = dequeueBelt(conveyor);   // Get an element from the queue

        pluOnConveyer = searchPluByName(lookupTable, ptr->name);

        getInteger("please enter the PLU in order to scan the item", &pluToScan);
        for (int i = 0; i < MAX_TRIES; i++) {
            if (pluToScan == pluOnConveyer) {
                itemPrice = searchPriceByName(lookupTable, ptr->name);
                printf("Item: %s    Weight: %.2fkg    Price: $%.2f \n", ptr->name, ptr->weight, itemPrice * ptr->weight);

                switch (i) {
                case 0:
                    *score += 50;
                    break;
                case 1:
                    *score += 30;
                    break;
                case 2:
                    *score += 10;
                    break;
                }

                return itemPrice * ptr->weight;
            }
            else {
                getInteger("PLU does not match. Please enter the correct PLU: ", &pluToScan);
            }
        }

        // If the name matches, calculate the price again
        itemPrice = searchPriceByName(lookupTable, ptr->name);
        printf("Item: %s    PLU: #%d    Weight: %.2fkg    Price: $%.2f \n", ptr->name, pluOnConveyer, ptr->weight, itemPrice * ptr->weight);
        return itemPrice * ptr->weight;
    }
}

bool makeChange(double cost, double received) {
    char userChoice = '\0';

    // Setup for change calculation
    /* Creates arrays for holding the names and values of each usable currency
       Creates array to hold the number of each currency the cashier is giving the customer */
    const char* changeTypes[10] = { "Pennies", "Nickels", "Dimes", "Quarters", "Loonies", "Toonies", "5's", "10's", "20's" };
    double changeAmount[CURRENCY_NUMBER] = { 0.01, 0.05, 0.10, 0.25, 1.00, 2.00, 5.00, 10.00, 20.00 };
    int changeCounter[CURRENCY_NUMBER] = { 0 };

    do {
        // Displays instructions to the user
        system("CLS");
        printf("Total Cart Cost: $%.2lf\nMoney Received from Customer: $%.2lf\n\n", cost, received);
        printf("Use the numberpad to select change.\nPress <ENTER> to finish and 'r' to reset.\n\n");
        for (int i = 0; i < CURRENCY_NUMBER; i++) {
            printf("%d. Number of %s:\t%10d\n", (i + 1), changeTypes[i], changeCounter[i]);
        }

        // User input
        userChoice = getch();
        if (userChoice > '0' && userChoice < ':') {
            /* Subtracts the value of '0' to convert the ASCII value to an int. Example: '4'(52) - '0'(48) = 4 */
            changeCounter[userChoice - '0' - 1]++;
        }
        else if (userChoice == 'r') {
            for (int i = 0; i < CURRENCY_NUMBER; i++) {
                changeCounter[i] = 0;
            }
        }
    } while (userChoice != '\r');

    // Calculates total change given
    double change = 0;
    for (int i = 0; i < CURRENCY_NUMBER; i++) {
        change += (changeAmount[i] * changeCounter[i]);
    }

    // Print and return result
    printf("\nChange Required: $%.2lf\n", received - cost);
    printf("Change Given: $%.2lf\n", change);

    return (int)(change * 1000) == ((int)(received * 1000) - (int)(cost * 1000)) ? true : false;
}

double generateRandomPayment(double totalBill) {
    int integerPart = rand() % 100 + 1; 
    int decimalPart = rand() % 100;     

    double randomPayment = totalBill + integerPart + (double)decimalPart / 100;

    return randomPayment;
}

void playTest(PLUTable* lookupTable, Cart* cart, ConveyorBelt* conveyor) {
    CartItem* ptr;
    CartItem* currentItem;
    int score = 0;
    double totalBill = 0.0;

    conveyor = initializingConveyorBelt();
    if (isCartEmpty(cart)) {
        printf("\nYou haven't put anything in your cart! Please put something in your cart before starting a session.");
        return;
    }
    do {

        while (!isBeltFull(conveyor) && !isCartEmpty(cart)) {
            ptr = popCart(cart);
            enqueueBelt(conveyor, *ptr);
            printf("\n%s has been placed on the conveyor.", ptr->name);
        }
        
        totalBill += scanItem(lookupTable, conveyor, &score);
    
    } while (!isBeltEmpty(conveyor));

    getch();
    printf(makeChange(totalBill, generateRandomPayment(totalBill)) ? "good\n" : "bad\n");
    getch();
}
