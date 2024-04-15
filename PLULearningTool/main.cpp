#include "hashtable.h"
#include "input.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


#define CHANGE_NUMBER 7         // Represents the number of different coins/bills that can be given
// as change to the customer (dimes, quarters, etc.)
#define MAX_ITEM      10        // Represents the maximum number of items in the cart allowed.
#define MAX_CONVEYOR_BELT 5     // Represents the maximum number of items on the belt allowed.

typedef struct CartItem {
    char name[NAME_LENGTH];
    double weight;
    CartItem* nextItem;
} CartItem;

typedef struct Cart {
    CartItem* data;
    int topIndex;
} Cart;

typedef struct ConveyorBelt {
    CartItem* front;
    CartItem* back;
} ConveyorBelt;

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
Cart* initializeCart(void) {
	Cart* cart = (Cart*)malloc(sizeof(Cart));

	if (cart == NULL) {                 //error checking 
		printf("Not enough memory!");
		exit(EXIT_FAILURE);
	}

	cart->data = (CartItem*)malloc(MAX_ITEM * sizeof(CartItem));

	if (cart->data == NULL) {           //error checking
		printf("Not enough memory!");
		exit(EXIT_FAILURE);
	}

	cart->topIndex = -1;

	return cart;
}

//maybe add push part
Cart* fillCart(PLUTable* PLUtable) {
	if (PLUtable == NULL) {             //error checking
		printf("Table is empty!!!");
		return NULL;
	}

	Cart* cart = initializeCart();

	int itemCount = countItemInTable(PLUtable);

	if (itemCount <= 0) {               //error checking
		printf("Table is empty!!!");
		return NULL;
	}

	srand(time(NULL));
	int chosenPLU[MAX_ITEM] = { 0 };

	while (isStackFull(cart) == false) {    //loop until cart is full
		bool alreadyChosen = false;
		int randomIndex = 0;

		do {
			randomIndex = rand() % itemCount;   //random seed

			for (int j = 0; j < cart->topIndex; j++) {       //loop to ensure index has not been chosen yet.
				//add another data structure for efficiency?
				if (chosenPLU[j] == randomIndex) {
					alreadyChosen = true;
				}

			}
		} while (alreadyChosen);

		strcpy(cart->data[cart->topIndex].name, PLUtable->table[randomIndex]->name);
		cart->topIndex++;

		if (cart->topIndex == 0) {
			continue;                       //to skip assigning value of nextItem.
		}

		cart->data[cart->topIndex--].nextItem = cart->data;
	}

	return cart;
	//fill up cart from hash table
}

bool isStackFull(Cart* cart) {
	return cart->topIndex == MAX_ITEM - 1;
}

int countItemInTable(PLUTable* PLUtable) {
	Item* current = PLUtable->table[0];

	if (current == NULL) {			//error check
		printf("No item in the table!!!");
		return 0;
	}

	int counter = 1;			//initialized to 1 because of loop structure

	while (current->NextItemValuePair != NULL) {	//loop to count until no next element
		current = current->NextItemValuePair;
		counter++;
	}

	return counter;
}

ConveyorBelt* initializingConveyorBelt(void) {
	ConveyorBelt* belt = (ConveyorBelt*)malloc(sizeof(ConveyorBelt));


	if (belt == NULL) {
		printf("Not enough memory!");
		exit(EXIT_FAILURE);
	}


	belt->front = NULL;
	belt->back = NULL;

	return belt;
}

ConveyorBelt* fillingConveyorBelt(Cart* cart) {
	ConveyorBelt* belt = initializingConveyorBelt();

	for (int i = 0; i < MAX_CONVEYOR_BELT - 1; i++) {               //adds 5 items to belt
		CartItem* newItem = (CartItem*)malloc(sizeof(CartItem));
		*newItem = cart->data[i];

		if (belt->front == NULL) {
			belt->front = newItem;
			belt->back = newItem;

			//need popping stack
		}

		belt->back->nextItem = newItem;
		belt->back = newItem;
		//need popping stack
	}

	return belt;
}
//==== = QUEUE==== =
//
//Cashier conveyor filled with CartItems
//
//- holds up to 5 items

// function prototypes
void fillLookupTable(PLUTable*);
double scanItem(PLUTable* lookupTable, CartItem* queue[]);
int makeChange(double, double);
double generateRandomPayment(double totalBill);
void playtest(PLUTable* lookupTable, CartItem* stack[], CartItem* queue[]);


int main(void)
{
    char choice = '\0';
    PLUTable* lookupTable = initializePLUTable();
    fillLookupTable(lookupTable);
    srand(time(NULL));        //

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
            playtest();
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

double scanItem(PLUTable* lookupTable, CartItem* queue[])
{
    CartItem* ptr;
    int pluOnConveyer = 0;
    int pluToScan = 0;
    double itemPrice = 0;

    while (!isQueueEmpty()) // Call isQueueEmpty function to check if the queue is empty
    {
        ptr = queuePop(queue);   // Get an element from the queue

        if (ptr == NULL) {
            break; // break if ptr is NULL
        }

        pluOnConveyer = searchPluByName(lookupTable, ptr->name);

        getInteger("please enter the PLU in order to scan the item", &pluToScan);

        if (pluToScan == pluOnConveyer)
        {
            itemPrice = searchPriceByName(lookupTable, ptr->name);
            printf("Item: %s    Weight: %f    Price: %f \n", ptr->name, ptr->weight, itemPrice * ptr->weight);
            return itemPrice * ptr->weight;
        }
        else
        {
            while (pluToScan != pluOnConveyer)
            {
                getInteger("PLU does not match. Please enter the correct PLU: ", &pluToScan);
            }

            // If the name matches, calculate the price again
            itemPrice = searchPriceByName(lookupTable, ptr->name);
            printf("Item: %s    Weight: %f    Price: %f \n", ptr->name, ptr->weight, itemPrice * ptr->weight);
            return itemPrice * ptr->weight;
        }
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

double generateRandomPayment(double totalBill) {
    int integerPart = rand() % 100 + 1; 
    int decimalPart = rand() % 100;     

    double randomPayment = totalBill + integerPart + (double)decimalPart / 100;

    return randomPayment;
}

void playtest(PLUTable* lookupTable, CartItem* stack[], CartItem* queue[])
{
    CartItem* ptr;
    double totalBill = 0.0;

    if (isStackEmpty()) {
        printf("nothing on the cart!")
            return -1;
    }

    while (!isQueuefull() && !isStackEmpty())
    {
        ptr = StackPop(stack);
        QueuePush(queue, ptr);
        printf("%s on the conveyer\n", ptr->name);

        if (isQueuefull())
        {
            printf("Conveyer is full, you need to scan the item before you put more on it\n");
            totalBill += scanItem(lookupTable, queue);
        }
    }

    printf(makeChange(totalBill, generateRandomPayment(totalBill)) == 0 ? "good\n" : "bad\n");
}
