#pragma warning (disable: 4996)
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

typedef struct Item {
    char name[100];
    int PLU;
    double price;
} Item;

typedef struct CartItem {
    char name[100];
    double weight;
} CartItem;

//used by the results screen to show mistakes
typedef struct Result {
    Item* itemPTR;
    int incorrectPLU;
} Result;



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