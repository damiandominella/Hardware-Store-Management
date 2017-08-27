/* Including standard libraries */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

/* Definition of constants */
#define INPUT_FILE "input.txt"
#define MIN_DATA_LENGTH 3
#define ARRAY_LENGTH 10

/* Declaration of functions */
int in_array(int val, int *arr, int size);

int main() {

	/* Defining some constant values */
	const char *ids[ARRAY_LENGTH] = {"N924", "H610", "S234", "P102", "G908", "W489", "W490", "G100", "T286", "C418"};
	const char *names[ARRAY_LENGTH] = {"Nail_L_2.5", "Hammer", "Screwdriver", "Pliers", "Glue", "Wrench_13", "Wrench_14", "Grinder", "Tape", "Chisel"};
	const int quantities[ARRAY_LENGTH] = {300, 127, 432, 250, 78, 58, 120, 300, 1000, 78};
	const float prices[ARRAY_LENGTH] = {1.20, 12.50, 4.55, 14.99, 1.99, 8.59, 8.59, 152.62, 0.49, 19.50};

	int i = 0;
	srand(time(NULL));

	int max = rand() % ARRAY_LENGTH;
	if (max <= MIN_DATA_LENGTH) max = MIN_DATA_LENGTH;

	int randoms[100];

	FILE *f = fopen(INPUT_FILE, "w");
	if (f != NULL)
	{
		/* Loop until random max value */
		for (i = 0; i < max; i++) {
			int r;
			int break_loop = 0;
			/* Generating always different random index of constant arrays */
			do {
				r = rand() % ARRAY_LENGTH;
				if (i == 0) {
					fprintf(f,"%s %s %.2f %d\n", names[r], ids[r], prices[r], quantities[r]); /* Writing to file */
					break_loop = 1;
				} else {
					if (!in_array(r, randoms, 100)) { /* Checking if value exists in array */
						fprintf(f,"%s %s %.2f %d\n", names[r], ids[r], prices[r], quantities[r]); /* Writing to file */
						break_loop = 1;
					}
				}
			} while (break_loop == 0);
			randoms[i] = r;
		}
		fclose(f);
	} else {
		printf("Error opening file!\n");
		return 1;
	}

	return 0;
}

/* Implementation of functions */
int in_array(int val, int *arr, int size){
    int i;
    for (i=0; i < size; i++) {
        if (arr[i] == val)
            return 1;
    }
    return 0;
}