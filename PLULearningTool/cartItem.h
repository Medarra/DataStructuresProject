
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