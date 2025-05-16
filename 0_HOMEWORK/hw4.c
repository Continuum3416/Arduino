#include <stdio.h>

#define MIN_TEMP 32.0
#define MAX_TEMP 212.0

typedef struct {
    float temperature;
    float density;
} DensityData;

// Array of temperature and density values
DensityData dataTable[] = {
    {32.0, 0.99987}, 
    {39.2, 1.00000},
    {40.0, 0.99999}, 
    {50.0, 0.99975},
    {60.0, 0.99907}, 
    {70.0, 0.99802}, 
    {80.0, 0.99669}, 
    {90.0, 0.99510},
    {100.0, 0.99318},
    {120.0, 0.98870}, 
    {140.0, 0.98338}, 
    {160.0, 0.97729},
    {180.0, 0.97056}, 
    {200.0, 0.96333}, 
    {212.0, 0.95865}
};

// Linear interpolation
float interpolate(float x) {
    int validRange = sizeof(dataTable) / sizeof(dataTable[0]) - 1;
    for (int i = 0; i < validRange; ++i) {
        if (x >= dataTable[i].temperature && x <= dataTable[i + 1].temperature) {
            // Linear interpolation formula
            float x1 = dataTable[i].temperature;
            float x2 = dataTable[i + 1].temperature;
            float y1 = dataTable[i].density;
            float y2 = dataTable[i + 1].density;
            return y1 + (y2 - y1) * (x - x1) / (x2 - x1);
        }
    }
    // If the temperature is out of range, return -1 (invalid)
    return -1.0f;
}

int main() {
    float temp = 0.0f;
    char choice = 'a';

    while(1) {
        printf("Enter the temperature in Fahrenheit (between %.1fF and %.1fF): ", MIN_TEMP, MAX_TEMP);
        scanf("%f", &temp);
        if (temp < MIN_TEMP || temp > MAX_TEMP) {
            printf("Invalid temperature. Temperature between %.1fF and %.1fF only.\n", MIN_TEMP, MAX_TEMP);
            continue;
        }

        // Interpolated density
        float density = interpolate(temp);

        printf("Estimated density at %.1fF is %.5f grams/cm^3.\n", temp, density);

        // Ask if the user wants to continue
        printf("Do you want to enter another temperature? (y/n): ");
        scanf(" %c", &choice);
        if(choice != 'y' && choice != 'Y')
            break;
    }

    return 0;
}

// gcc -o bin/hw4 hw4.c; ./bin/hw4