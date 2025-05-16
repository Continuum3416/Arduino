#include <stdio.h> 
#include <stdint.h>
#include <math.h>

// Assuming make additions at start of each compounding period
float interest(float current_saving, float annual_contribution, float rate, uint16_t saving_years) 
{
    while(saving_years--) {
        current_saving += annual_contribution;
        current_saving *= (1.f + rate);
    }
    return current_saving;
}


int main(int argc, char *argv[])
{
    const float interest_rate = 0.07f;
    float engineer_1_amount = interest(0.f, 10000.f, interest_rate, 35);
    float engineer_2_amount = interest(0.f, 30000.f, interest_rate, 15);
    printf("Final amount for engineer 1: $%.2f\n", engineer_1_amount);
    printf("Final amount for engineer 2: $%.2f\n", engineer_2_amount);
    return 0;
}

// gcc -o bin/hw1 hw1.c; ./bin/hw1