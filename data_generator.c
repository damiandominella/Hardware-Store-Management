/* Including standard libraries */
#include <stdio.h>
#include <stdlib.h>

/* Definition of constants */
#define INPUT_FILE "input.txt"
#define ARRAY_LENGTH 10

int main() {

	/* Defining some constant values */
	const char *ids[ARRAY_LENGTH] = {"N924", "H610", "S234", "P102", "G908", "W489", "W490", "G100", "T286", "C418"};
	const char *names[ARRAY_LENGTH] = {"Nail_L_2.5", "Hammer", "Screwdriver", "Pliers", "Glue", "Wrench_13", "Wrench_14", "Grinder", "Tape", "Chisel"};
	const int quantities[ARRAY_LENGTH] = {300, 127, 432, 250, 78, 58, 120, 300, 1000, 78};
	const float prices[ARRAY_LENGTH] = {1.20, 12.50, 4.55, 14.99, 1.99, 8.59, 8.59, 152.62, 0.49, 19.50};

	int i = 0;

	int max;
	printf("\nInsert number of articles to generate (0-10): ");
	scanf("%d", &max);

	FILE *f = fopen(INPUT_FILE, "w");
	if (f != NULL)
	{
		/* Loop until max value */
		for (i = 0; i < max; i++) {
			fprintf(f,"%s %s %.2f %d\n", names[i], ids[i], prices[i], quantities[i]); /* Writing to file */
		}
		fclose(f);
	} else {
		printf("Error opening file!\n");
		return 1;
	}

	printf("\n%d articles generated!\n\n", max);

	return 0;
}