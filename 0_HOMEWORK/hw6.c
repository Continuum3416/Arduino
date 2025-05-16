#include <stdio.h>
#include <math.h>

#define PI 3.14159265358979323846

int main() {
    int i, j;
    double radians;
    FILE *file_out;

    // Open the file for writing
    file_out = fopen("file.txt", "w");
    if (file_out == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    // Write the header
    fprintf(file_out, "Degrees to Radians in 2-degree steps\n");
    fprintf(file_out, "Degrees  0     2     4     6     8\n");

    // Loop through each decade (0 to 350 in steps of 10)
    for (i = 0; i <= 350; i += 10) {
        // Print the decade label
        // - means align left
        // 7 means character spacing
        fprintf(file_out, "%-7d", i);

        // Print 5 values per line: i, i+2, i+4, i+6, i+8
        for (j = 0; j < 5; j++) {
            int degree = i + j * 2;
            radians = degree * (PI / 180.0);
            fprintf(file_out, "%5.3f ", radians);
        }

        fprintf(file_out, "\n");
    }

    fclose(file_out);
    printf("File written successfully.\n");

    return 0;
}

// gcc -o bin/hw6 hw6.c; ./bin/hw6