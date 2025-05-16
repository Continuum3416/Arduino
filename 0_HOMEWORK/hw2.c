#include <stdio.h>

int main(int argc, char *argv[]) {
    int a, b;
    
    printf("Press 0 0 to quit.\n");
    while (1) {
        // Input
        printf("Please enter the numerator: ");
        while (scanf("%d", &a) != 1) {
            printf("Invalid input! Please enter a valid integer: ");
            while (getchar() != '\n'); // Clear the input buffer
        }
        printf("Please enter the denominator: ");
        while (scanf("%d", &b) != 1) {
            printf("Invalid input! Please enter a valid integer: ");
            while (getchar() != '\n'); // Clear the input buffer
        }
        
        // Exit condition
        if (a == 0 && b == 0) {
            printf("Exiting program.\n\n");
            break;
        }
        
        // Handle division by zero
        if (b == 0) {
            printf("You have commited a crime.\n\n");
            continue;
        }
        
        int quotient = a / b;
        int remainder = a % b;
        
        // Print the results
        printf("%d divides into %d with a result of %d and a remainder of %d.\n\n", b, a, quotient, remainder);
    }
    
    return 0;
}

// gcc -o bin/hw2 hw2.c; ./bin/hw2