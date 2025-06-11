#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// initial size of the dynamically allocated array quarters
#define INITIAL_CAPACITY 100

typedef struct {
    int year;
    int quarter;
    double sumOfAnomalies; // sum of all ENSO anomaly values for a specific quarter
    int count; // number of months that have been added into sumOfAnomalies
} QuarterData;

int get_quarter(int month) {
    if (month >= 1 && month <= 3) return 1;
    if (month >= 4 && month <= 6) return 2;
    if (month >= 7 && month <= 9) return 3;
    return 4;
}

char* quarter_name(int q) {
    static char buf[3];
    snprintf(buf, sizeof(buf), "Q%d", q);
    return buf;
}

int main() {
    FILE *fin = fopen("Oceandata.txt", "r");
    FILE *fout = fopen("Elnino15.dat", "w");
    if (!fin || !fout) {
        perror("File error");
        return -1;
    }

    // Skip the first two lines
    char buffer[256];
    fgets(buffer, sizeof(buffer), fin); // skip "831 records"
    fgets(buffer, sizeof(buffer), fin); // skip column headers

    int year, month;
    double total, climadj, anom;

    int capacity = INITIAL_CAPACITY;
    int size = 0;

    // allocate a block of memory for quarters
    QuarterData *quarters = malloc(capacity * sizeof(QuarterData));
    if (!quarters) {
        fprintf(stderr, "Memory allocation failed\n");
        return -1;
    }

    int record_count = 0;

    // Read
    while (fscanf(fin, "%d %d %lf %lf %lf", &year, &month, &total, &climadj, &anom) == 5) {
        record_count++;  // Keep track of how many records were processed

        int quarter = get_quarter(month);  // Convert the month into a quarter (1 to 4)
        int found = 0;  // Flag to check if this year/quarter already exists in the array

        // Search the existing array to find a matching year and quarter
        for (int i = 0; i < size; i++) {
            if (quarters[i].year == year && quarters[i].quarter == quarter) {
                quarters[i].sumOfAnomalies += anom;  // Add anomaly to the existing sum
                quarters[i].count++;                // Increment the count of months in that quarter
                found = 1;                           // Mark that we found an existing record
                break;
            }
        }

        // If this year/quarter combination doesn't exist, add a new entry
        if (!found) {
            // If we've reached current capacity, double it to make space
            if (size == capacity) {
                capacity *= 2;
                quarters = realloc(quarters, capacity * sizeof(QuarterData));
                if (!quarters) {
                    fprintf(stderr, "Memory reallocation failed\n");
                    return 1;
                }
            }

            // Initialize new quarter entry with current data
            quarters[size].year = year;
            quarters[size].quarter = quarter;
            quarters[size].sumOfAnomalies = anom;
            quarters[size].count = 1;
            size++;  // Increase the size of the used array
        }
    }

    // initialize with some large/small numbers
    double maxElNino = -1e9, maxLaNina = 1e9;
    int maxElNinoIndex = -1, maxLaNinaIndex = -1;

    fprintf(fout, "Year,Quarter,Index\n");

    for (int i = 0; i < size; i++) {
        double avg = quarters[i].sumOfAnomalies / quarters[i].count;

        if (avg > maxElNino) {
            maxElNino = avg;
            maxElNinoIndex = i;
        }
        if (avg < maxLaNina) {
            maxLaNina = avg;
            maxLaNinaIndex = i;
        }

        if (avg > 1.0) {
            fprintf(fout, "%d,%s,%.2f\n", quarters[i].year, quarter_name(quarters[i].quarter), avg);
        }
    }

    if (maxElNinoIndex != -1) {
        printf("Maximum El Nino Conditions in Data File\n");
        printf("Year: %d, Quarter: %s\n\n", quarters[maxElNinoIndex].year,
               quarter_name(quarters[maxElNinoIndex].quarter));
    }

    if (maxLaNinaIndex != -1) {
        printf("Maximum La Nina Conditions in Data File\n");
        printf("Year: %d, Quarter: %s\n", quarters[maxLaNinaIndex].year,
               quarter_name(quarters[maxLaNinaIndex].quarter));
    }
    printf("%d records processed", record_count);

    free(quarters);
    fclose(fin);
    fclose(fout);
    return 0;
}

// gcc -o bin/hw14 hw14.c; ./bin/hw14