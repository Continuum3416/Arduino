#include <stdio.h> 
#include <stdlib.h> 

int main() 
{ 
    int* ptr = NULL; 
    int size = 0; 

    printf("Enter size of elements: "); 
    scanf("%d", &size); 

    // Memory allocates dynamically using malloc()
    ptr = (int*)malloc(size * sizeof(int)); 

    if (ptr == NULL) { 
        printf("Memory not allocated.\n"); 
    } else { 
        // Memory allocated 
        printf("Memory successfully allocated using "
            "malloc.\n"); 

        // Get the elements of the array
        for (int j = 0; j < size; ++j) { 
            ptr[j] = j + 1; 
        } 

        printf("The elements of the array are: "); 
        for (int k = 0; k < size; ++k) { 
            printf("%d, ", ptr[k]); 
        } 
    } 

    return 0; 
}

// gcc -o bin/main.exe main.c; ./bin/main.exe