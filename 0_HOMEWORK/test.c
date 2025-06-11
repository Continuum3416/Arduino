#include <stdio.h>

int main()
{
    float temp = 0.1;
    for (int j = 0; j<5; j= ++j + 1)
    {
        for (int i = 1; i<(j+2); i++){
            temp += j/i*temp;
            printf("%f\n", temp);
            //printf("Run\n");
        }
            
        //std::cout << j << std::endl;
    }
    printf("%f", temp);

    return 0;
}

// gcc -o bin/test test.c; ./bin/test