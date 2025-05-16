#include <stdio.h>
#include <math.h>
#include <stdint.h>

float distance(const float vec1[], const float vec2[]) 
{
    const float a = vec1[0] - vec2[0];
    const float b = vec1[1] - vec2[1];
    return sqrt(a * a + b * b);
}

int main(int argc, char *argv[])
{
    float vec1[2] = {-1.f, 6.f};
    float vec2[2] = {2.f, 4.f};
    float dist = distance(vec1, vec2);

    printf("The distance between two points is: %5.2f", dist);

    return 0;
}

// gcc -o bin/sqrt sqrt.c; ./bin/sqrt