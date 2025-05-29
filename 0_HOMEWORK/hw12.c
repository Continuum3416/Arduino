#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define SIZE 26

char encode_key[SIZE] = "QWERTYUIOPASDFGHJKLZXCVBNM";
char decode_key[SIZE];

// creates a reverse mapping of the encode_key, so you can decode encoded messages
void build_decode_key(char* encode, char* decode) {
    for (int i = 0; i < 26; i++) {
        decode[encode[i] - 'A'] = 'A' + i;
    }
}

// Remove all non-alphabetic characters like punctuation, numbers, and spaces.
// Convert all remaining letters to uppercase.
void clean_input(char* str) {
    int j = 0;
    for (int i = 0; str[i]; i++) {
        if (isalpha(str[i])) {
            str[j++] = toupper(str[i]);
        }
    }
    str[j] = '\0';
}

void encode(char* input, char* output, char* encode) {
    for (int i = 0; input[i]; i++) {
        output[i] = encode[input[i] - 'A'];
    }
    output[strlen(input)] = '\0';
}

void decode(char* input, char* output, char* decode) {
    for (int i = 0; input[i]; i++) {
        output[i] = decode[input[i] - 'A'];
    }
    output[strlen(input)] = '\0';
}

// take in an input file, and then add a enc_ to the output file
void process_file(const char* filename, int encode_flag) {
    FILE *fp_in = fopen(filename, "r");
    if (!fp_in) {
        printf("Cannot open file.\n");
        return;
    }

    // create a new file
    char new_filename[100];
    sprintf(new_filename, "enc_%s", filename);
    FILE *fp_out = fopen(new_filename, "w");

    char line[1024], cleaned[1024], result[1024];
    while (fgets(line, sizeof(line), fp_in)) {
        clean_input(line);
        if (encode_flag)
            encode(line, result, encode_key);
        else
            decode(line, result, decode_key);
        fprintf(fp_out, "%s\n", result);
    }

    fclose(fp_in);
    fclose(fp_out);
    printf("Output written to %s successfully\n", new_filename);
}

int main() {
    build_decode_key(encode_key, decode_key);

    char choice[10], file_choice[10];
    printf("Do you want to encode or decode? ");
    scanf("%s", choice);

    // record if the first letter is e or not
    int encode_flag = (tolower(choice[0]) == 'e');

    printf("Is the message in a text file? (yes/no): ");
    scanf("%s", file_choice);

    if (tolower(file_choice[0]) == 'y') {
        char filename[100];
        printf("Enter filename: ");
        scanf("%s", filename);
        process_file(filename, encode_flag);
    } else {
        char input[1024], cleaned[1024], result[1024];
        printf("Enter your message: ");
        getchar(); // flush newline
        fgets(input, sizeof(input), stdin);
        clean_input(input);

        if(encode_flag) encode(input, result, encode_key);
        else decode(input, result, decode_key);

        printf("Result: %s\n", result);
    }

    return 0;
}

// gcc -o bin/hw12 hw12.c; ./bin/hw12