#ifndef STATISTICS_H
#define STATISTICS_H

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

// Compare two integers (for qsort)
int Compare(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

// calculate mean
double Mean(int *data, int size) {
    long sum = 0;
    for (int i = 0; i < size; i++)
        sum += data[i];
    return (double)sum / size;
}

// Calculate median
double Median(int *data, int size) {
    qsort(data, size, sizeof(int), Compare);
    if (size % 2 == 0)
        return (data[size/2 - 1] + data[size/2]) / 2.0;
    else
        return data[size/2];
}

// Standard deviation
double Std_Dev(int *data, int size, double mean) {
    double variance = 0.0;
    for (int i = 0; i < size; i++)
        variance += pow(data[i] - mean, 2);
    return sqrt(variance / size);
}

// Histogram generation
void print_histogram(int *data, int size, int min, int max) {
    int range = max - min + 1;
    int *hist = (int*)calloc(range, sizeof(int));

    for (int i = 0; i < size; i++) {
        hist[data[i] - min]++;
    }

    printf("\nHistogram of throw sums:\n");
    for (int i = 0; i < range; i++) {
        if (hist[i] > 0) {
            printf("%2d: ", i + min);
            for (int j = 0; j < hist[i]; j++)
                printf("*");
            printf("\n");
        }
    }

    free(hist);
}


#endif