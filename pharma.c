#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MEDICINES 100
#define MAX_NAME_LENGTH 50

// Structure to represent a medicine
typedef struct {
    char name[MAX_NAME_LENGTH];
    float price;
    int quantity;
} Medicine;

// Function to load medicines from file
void loadMedicinesFromFile(const char *filename, Medicine medicines[], int *numMedicines) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    *numMedicines = 0;
    while ((*numMedicines < MAX_MEDICINES) && (fscanf(file, "%s %f %d", medicines[*numMedicines].name,
                                                      &medicines[*numMedicines].price,
                                                      &medicines[*numMedicines].quantity) == 3)) {
        (*numMedicines)++;
    }

    fclose(file);
}

// Function to add medicine to cart
void addToCart(Medicine medicines[], int numMedicines, char *medicineName, int quantity, Medicine cart[], int *numItemsInCart) {
    for (int i = 0; i < numMedicines; i++) {
        if (strcmp(medicines[i].name, medicineName) == 0) {
            if (quantity <= medicines[i].quantity) {
                strcpy(cart[*numItemsInCart].name, medicineName);
                cart[*numItemsInCart].price = medicines[i].price;
                cart[*numItemsInCart].quantity = quantity;
                (*numItemsInCart)++;
                printf("Added to cart: %s (Quantity: %d)\n", medicineName, quantity);
                return;
            } else {
                printf("Not enough quantity available for %s\n", medicineName);
                return;
            }
        }
    }
    printf("%s not found in inventory\n", medicineName);
}

// Function to display cart contents
void displayCart(Medicine cart[], int numItemsInCart) {
    if (numItemsInCart == 0) {
        printf("Cart is empty\n");
        return;
    }
    printf("Items in Cart:\n");
    printf("Name\t\tPrice\tQuantity\n");
    for (int i = 0; i < numItemsInCart; i++) {
        printf("%s\t\t%.2f\t%d\n", cart[i].name, cart[i].price, cart[i].quantity);
    }
}

// Function to calculate total price
float calculateTotalPrice(Medicine cart[], int numItemsInCart) {
    float totalPrice = 0;
    for (int i = 0; i < numItemsInCart; i++) {
        totalPrice += cart[i].price * cart[i].quantity;
    }
    return totalPrice;
}

// Function to confirm purchase and deduct cart from inventory
void confirmPurchase(Medicine cart[], int numItemsInCart, Medicine medicines[], int numMedicines, const char *filename) {
    printf("Confirm purchase? (y/n): ");
    char choice;
    scanf(" %c", &choice);

    if (choice == 'y' || choice == 'Y') {
        // Deduct cart from inventory
        for (int i = 0; i < numItemsInCart; i++) {
            for (int j = 0; j < numMedicines; j++) {
                if (strcmp(cart[i].name, medicines[j].name) == 0) {
                    medicines[j].quantity -= cart[i].quantity;
                    break;
                }
            }
        }

        // Update the file with new quantities
        FILE *file = fopen(filename, "w");
        if (file == NULL) {
            printf("Error opening file for writing!\n");
            return;
        }

        for (int i = 0; i < numMedicines; i++) {
            fprintf(file, "%s %.2f %d\n", medicines[i].name, medicines[i].price, medicines[i].quantity);
        }

        fclose(file);

        printf("Purchase confirmed. Cart deducted from inventory.\n");
    } else {
        printf("Purchase cancelled.\n");
    }
}

int main() {
    Medicine medicines[MAX_MEDICINES];
    int numMedicines = 0;

    // Load medicines from file
    loadMedicinesFromFile("medicines.txt", medicines, &numMedicines);

    Medicine cart[MAX_MEDICINES];
    int numItemsInCart = 0;

    char medicineName[MAX_NAME_LENGTH];
    int quantity;

    // Interactive loop
    char continueShopping = 'y';
    while (continueShopping == 'y' || continueShopping == 'Y') {
        printf("Enter medicine name: ");
        scanf("%s", medicineName);
        printf("Enter quantity: ");
        scanf("%d", &quantity);

        addToCart(medicines, numMedicines, medicineName, quantity, cart, &numItemsInCart);

        printf("Continue shopping? (y/n): ");
        scanf(" %c", &continueShopping);
    }

    // Display cart contents
    displayCart(cart, numItemsInCart);

    // Calculate and display total price
    float totalPrice = calculateTotalPrice(cart, numItemsInCart);
    printf("Total price: %.2f\n", totalPrice);

    // Confirm purchase and deduct from inventory
    confirmPurchase(cart, numItemsInCart, medicines, numMedicines, "medicines.txt");

    return 0;
}
