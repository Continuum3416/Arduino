#include <stdio.h>
#include <stdlib.h>

#define MAX 25

// Function to check if a point is a peak
int is_peak(double grid[MAX][MAX], int i, int j) {
    double val = grid[i][j];
    for (int di = -1; di <= 1; di++) {
        for (int dj = -1; dj <= 1; dj++) {
            if (di == 0 && dj == 0) continue;
            if (grid[i + di][j + dj] >= val) return 0;
        }
    }
    return 1;
}

int main() {
    FILE *fp = fopen("grid1.txt", "r");
    if (!fp) {
        perror("Could not open file");
        return 1;
    }

    int rows, cols;
    double grid[MAX][MAX];

    if (fscanf(fp, "%d %d", &rows, &cols) != 2) {
        printf("Invalid file format.\n");
        fclose(fp);
        return 1;
    }

    if (rows > MAX || cols > MAX) {
        printf("Grid size exceeds maximum allowed size of %d.\n", MAX);
        fclose(fp);
        return 1;
    }

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            fscanf(fp, "%lf", &grid[i][j]);

    fclose(fp);

    int count = 0;
    printf("Peak locations (1-based indexing):\n");

    for (int i = 1; i < rows - 1; i++) {
        for (int j = 1; j < cols - 1; j++) {
            if (is_peak(grid, i, j)) {
                printf("  Peak at (%d, %d) = %.2f meters\n", i + 1, j + 1, grid[i][j]);
                count++;
            }
        }
    }

    printf("Total number of peaks: %d\n", count);
    return 0;
}


// gcc -o bin/hw8 hw8.c; ./bin/hw8