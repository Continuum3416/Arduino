#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define WORD_MAX 1024

int main(void) {
    FILE *fin = fopen("P6_In.txt", "r");
    if (!fin) {
        fprintf(stderr, "Error: could not open input file P6_In.txt\n");
        return -1;
    }

    FILE *fout = fopen("P6_Out.txt", "w");
    if (!fout) {
        fprintf(stderr, "Error: could not open output file P6_Out.txt\n");
        fclose(fin);
        return -1;
    }

    char word[WORD_MAX];
    printf("Remember, the maximum length of a word is %d characters\n", WORD_MAX - 1);
    printf("Would you like to proceed? (y/n) ");

    char answer[16];
    if (scanf("%3s", answer) != 1 || (answer[0] != 'y' && answer[0] != 'Y')) {
        printf("Exiting program.\n");
        fclose(fin);
        fclose(fout);
        return 0;
    }

    int word_index = 1;

    // Read words until end of file
    while (fscanf(fin, "%1023s", word) == 1) {
        // convert words to lowercase
        for (char *p = word; *p; ++p) {
            *p = tolower((unsigned char)*p);
        }

        int pattern_pos = (word_index - 1) % 3 + 1;  // 1, 2, or 3 in cyclic order
        size_t len = strlen(word); // length of the word

        if (len >= (size_t)pattern_pos) {
            // If word is long enough, take that letter
            fputc(word[pattern_pos - 1], fout); // similar to putc
        } else { // if word is shorter than index
            // decode as a space
            fputc(' ', fout);
        }

        word_index++;
    }

    fclose(fin);
    fclose(fout);

    printf("File written");
    return 0;
}

// gcc -o bin/final final.c; ./bin/final