#include <time.h>

#define MAX_ITEM      10        // Represents the maximum number of items in the cart allowed.
#define MAX_CONVEYOR_BELT 5     // Represents the maximum number of items on the belt allowed.

typedef struct CartItem {
    char name[NAME_LENGTH];
    double weight;
} CartItem;

typedef struct Cart {
    CartItem* data;
    int topIndex;
} Cart;

typedef struct ConveyorBelt {
	CartItem data[MAX_CONVEYOR_BELT];
	int front;
	int back;
} ConveyorBelt;

//function prototype for cartItem
CartItem createNewCartItem(char* itemName, double itemWeight);
//function prototypes for cart
Cart* initializeCart(void);
void pushCart(Cart* cart, char* itemName, double itemWeight);
char* popCart(Cart* cart);
bool isCartFull(Cart* cart);
bool isCartEmpty(Cart* cart);
//function prototypes for belt
ConveyorBelt* initializingConveyorBelt(void);
void enqueueBelt(ConveyorBelt* belt, CartItem itemToEnqueue);
CartItem dequeueBelt(ConveyorBelt* belt);
bool isBeltFull(ConveyorBelt* belt);
bool isBeltEmpty(ConveyorBelt* belt);

/*********************************************************************/
/*********************************************************************/
/**************************		  CartItem		**********************/
/*********************************************************************/
/*********************************************************************/
CartItem createNewCartItem(char* itemName, double itemWeight) {
	CartItem newCartItem;

	strcpy(newCartItem.name, itemName);
	newCartItem.weight = itemWeight;

	return newCartItem;
}

/*********************************************************************/
/*********************************************************************/
/*****************************    Stack	  ****************************/
/*********************************************************************/
/*********************************************************************/

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

void pushCart(Cart* cart, char* itemName, double itemWeight) {
	if (isCartFull(cart)) {
		printf("Cart overflow");
		exit(EXIT_FAILURE);
	}

	cart->data[++cart->topIndex] = createNewCartItem(itemName, itemWeight);
}

char* popCart(Cart* cart) {
	if (isCartEmpty(cart)) {
		printf("Cart underflow");
		return NULL;
	}

	return cart->data[cart->topIndex--].name;
}

bool isCartFull(Cart* cart) {
	return cart->topIndex == MAX_ITEM - 1;
}

bool isCartEmpty(Cart* cart) {
	return cart->topIndex == -1;
}

/*********************************************************************/
/*********************************************************************/
/*****************************    Queue	  ****************************/
/*********************************************************************/
/*********************************************************************/

ConveyorBelt* initializingConveyorBelt(void) {
	ConveyorBelt* belt = (ConveyorBelt*)malloc(sizeof(ConveyorBelt));


	if (belt == NULL) {
		printf("Not enough memory!");
		exit(EXIT_FAILURE);
	}


	belt->front = -1;
	belt->back = -1;

	return belt;
}

void enqueueBelt(ConveyorBelt* belt, CartItem itemToEnqueue) {
	if (isBeltFull(belt)) {
		printf("Belt is full!");
		return;
	}
	
	if (isBeltEmpty(belt)) {
		belt->front = 0;
		belt->back = 0;
	}
	else {
		belt->back = (belt->back + 1) % MAX_CONVEYOR_BELT;
	}

	belt->data[belt->back] = itemToEnqueue;
}

CartItem dequeueBelt(ConveyorBelt* belt) {
	CartItem retItem = { "", 0.0 };

	if (isBeltEmpty(belt)) {
		printf("Belt is empty!");
		return retItem;
	}

	retItem = belt->data[belt->front];

	if (belt->front == belt->back) {
		belt->front = -1;
		belt->back = -1;
	}
	else {
		belt->front = (belt->front + 1) % MAX_CONVEYOR_BELT;
	}

	return retItem;
}

bool isBeltFull(ConveyorBelt* belt) {
	return (belt->back + 1) % MAX_CONVEYOR_BELT == belt->front;
}

bool isBeltEmpty(ConveyorBelt* belt) {
	return belt->front == -1;
}

//maybe add push part
/*Cart* fillCart(PLUTable* PLUtable) {
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
}*/