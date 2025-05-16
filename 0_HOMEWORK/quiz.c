#include <stdio.h>

int main () {
    float g;
    for(int b = 1; b < 25; b++)
        g = g+b;
    printf("%f\n",g);

    return 0;
}

// gcc -o bin/quiz.exe quiz.c; ./bin/quiz.exe