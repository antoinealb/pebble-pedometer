#include <stdio.h>
#include "pedometer.h"

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Usage: %s acc_file.csv\n", argv[0]);
        return 1;
    }

    FILE *input_file = fopen(argv[1], "r");
    if (input_file == NULL) {
        printf("Can't open %s\n, aborting...", argv[1]);
        return 1;
    }

    /* Configure pedometer algorithm. */
    pedometer_t meter;
    float filter_buffer[20];
    float threshold_buffer[50];
    pedometer_init(&meter,
                   filter_buffer, sizeof(filter_buffer) / sizeof(float),
                   threshold_buffer, sizeof(threshold_buffer) / sizeof(float));

    meter.hysteresis = 0.1;

    /* Provide samples to the algorithm. */
    float acc[3];
    while (fscanf(input_file, "%f,%f,%f", &acc[0], &acc[1], &acc[2]) == 3) {
        pedometer_process(&meter, acc[1]);
    }

    /* Display results. */
    printf("Counted %d steps in %s.\n", pedometer_get_step_count(&meter), argv[1]);
}
