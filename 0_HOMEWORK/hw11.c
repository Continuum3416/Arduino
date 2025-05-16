#include <stdio.h>
#include <time.h>
#include "statistics.h"


int main() {
    int dice_sides, dice_count, throw_count;

    srand((unsigned int)time(NULL));

    // Input
    printf("Enter number of sides on each die: "); scanf("%d", &dice_sides);
    printf("Enter number of dice to roll per throw: "); scanf("%d", &dice_count);
    printf("Enter number of throws to make: "); scanf("%d", &throw_count);

    // Allocate memory
    int *results = (int*)malloc(throw_count * sizeof(int));
    if (results == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    // Perform dice throws
    for (int i = 0; i < throw_count; i++) {
        int sum = 0;
        for (int j = 0; j < dice_count; j++) {
            int roll = (rand() % dice_sides) + 1;
            sum += roll;
        }
        results[i] = sum;
    }

    double mean = Mean(results, throw_count);
    double median = Median(results, throw_count);
    double std_dev = Std_Dev(results, throw_count, mean);

    // Save to file
    FILE *fp = fopen("dice_results.txt", "w");
    if (fp == NULL) {
        printf("Failed to open output file.\n");
        free(results);
        return -1;
    }

    fprintf(fp, "Number of throws: %d\n", throw_count);
    fprintf(fp, "Results of each throw:\n");
    for (int i = 0; i < throw_count; i++) {
        // display results will be sorted
        fprintf(fp, "%d ", results[i]);
    }

    fprintf(fp, "\nStatistical Summary:\n");
    fprintf(fp, "Mean: %.2f\n", mean);
    fprintf(fp, "Median: %.2f\n", median);
    fprintf(fp, "Standard Deviation: %.2f\n", std_dev);

    fclose(fp);

    // Display summary
    printf("\nResults saved to dice_results.txt\n");
    printf("Mean: %.2f\n", mean);
    printf("Median: %.2f\n", median);
    printf("Standard Deviation: %.2f\n", std_dev);

    // (Extra Credit) Print histogram
    int min_sum = dice_count * 1;
    int max_sum = dice_count * dice_sides;
    print_histogram(results, throw_count, min_sum, max_sum);

    free(results);
    return 0;
}

// gcc -o bin/hw11 hw11.c; ./bin/hw11