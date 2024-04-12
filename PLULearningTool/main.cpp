#include <stdio.h>
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

//==== = Database.txt==== =
//
//Text file listing all Item data
//
//
//
//-Format -
//
//<Item Name(string)> | <PLU Code(int)> | <Price(double)>





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





//==== = ARRAY==== =
//
//Array that holds the inccorect answers from a game session
//
//- holds up to 10 items

int main()
{
    char mainMenuChoice;

    //==== = USER INTERFACE==== =
    //
    //4 Pages: Main Menu, PLU Code Lookup, Game Screen, Result Screen
    //
    //(More can be added but we probably don't have time)

    do
    {
        printf("\n\nPLU Learning Tool\n");
        printf("=========\n\n");
        printf("1.  Main Menu\n");
        printf("2.  PLU Code Lookup\n");
        printf("3.  Game Screen\n");
        printf("4.  Result Screen\n");
        printf("5.  Exit\n");

        char choice = getch();

        switch (choice)
        {
            //	--Main Menu--
            //
            //	Contains 3 options:
            //
            //1. Start Game Session
            //
            //2. Review PLU Codes
            //
            //3. Exit
            //
            //
            //
            //-Option selected through options number(getch())
            //
            //- Any other key press does nothing

        case '1':
            do
            {
                printf("\n\nMAIN MENU\n");
                printf("=========\n\n");
                printf("1.  Start Game Session\n");
                printf("2.  Review PLU Codes\n");
                printf("3.  Exit\n");

                mainMenuChoice = getch();

                switch (mainMenuChoice)
                {
                case '1':
                    // Start Game Session
                    break;
                case '2':
                    // Review PLU Codes
                    break;
                case '3':
                    // Exit from main menu
                    break;
                }
            } while (mainMenuChoice != '3');

            break;
        case '2':
            //--PLU Code Lookup--
            //
            //Displays entire contents of the Hash Table
            //
            //- First displays message to user : "Press <<ESCAPE>> or <<ENTER>> to return to main menu"
            //
            //- Use getch() to check for escape / enter key
            //
            //- Any other key press does nothing
            break;
        case '3':

            //--Game Screen--
            //
            //I LOST THE GAME!
            //
            //-Creates a new Cart Stack with random items and a new empty Conveyor Queue
            //
            //<WORK IN PROGRESS>

            break;
        case '4':

            //--Result Screen--
            //
            //Displays results of last game session
            //
            //<WORK IN PROGRESS>
            //
            //-Last displays message to user : "Press <<ESCAPE>> or <<ENTER>> to return to main menu"
            //
            //- Use getch() to check for escape / enter key
            //
            //- Any other key press does nothing

            break;
        case '5':
            return 0; // Exit the program
        default:
            break;
        }
    } while (1);

    return 0;
}