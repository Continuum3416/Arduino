#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LONG_LEN 10000
#define MAX_SHORT_LEN 100

// Convert string to uppercase
void to_uppercase(char *str) {
    while (*str) {
        *str = toupper((unsigned char)*str);
        str++;
    }
}

int main() {
    FILE *fp_in, *fp_out;
    char long_str[MAX_LONG_LEN];
    char short_str[MAX_SHORT_LEN];
    int count = 0;

    // Open DNA file for reading
    fp_in = fopen("LongDNAseq.dat", "r");
    if (fp_in == NULL) {
        printf("Error opening LongDNAseq.dat");
        return -1;
    }

    // Read the long DNA string from file
    if (fscanf(fp_in, "%s", long_str) != 1) {
        fprintf(stderr, "Error reading from file.\n");
        fclose(fp_in);
        return 1;
    }
    fclose(fp_in);

    // Get short string from user
    printf("Enter the short DNA sequence to search for: ");
    if (scanf("%s", short_str) != 1) {
        fprintf(stderr, "Invalid input.\n");
        return -1;
    }

    // Convert both strings to uppercase
    to_uppercase(long_str);
    to_uppercase(short_str);

    // Check that short_str is shorter than long_str
    if (strlen(short_str) >= strlen(long_str)) {
        fprintf(stderr, "Your entered gene must be shorter than the long DNA sequence.\n");
        return -1;
    }

    // Open output file
    fp_out = fopen("dna_results.txt", "w");
    if (fp_out == NULL) {
        printf("Error opening dna_results.txt");
        return -1;
    }

    // Search for short_str in long_str
    char *ptr = long_str;
    while ((ptr = strstr(ptr, short_str)) != NULL) {
        int location = ptr - long_str + 1;
        printf("Match found at position: %d\n", location);
        fprintf(fp_out, "Match found at position: %d\n", location);
        count++;
        ptr++;  // Move past current match
    }

    // Output to console and file
    printf("Total occurrences: %d\n", count);
    fprintf(fp_out, "Total occurrences: %d\n", count);

    fclose(fp_out);
    return 0;
}

// gcc -o bin/hw13 hw13.c; ./bin/hw13