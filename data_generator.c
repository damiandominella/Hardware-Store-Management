/* Including standard libraries */
#include <stdio.h>
#include <stdlib.h>

/* Definition of constants */
#define INPUT_FILE "input.txt"
#define ARRAY_LENGTH 10

void clear_buffer();
int get_valid_int(char *field_name);

int main() {

	/* Defining some constant values */
	const char *ids[ARRAY_LENGTH] = {"N924", "H610", "S234", "P102", "G908", "W489", "W490", "G100", "T286", "C418"};
	const char *names[ARRAY_LENGTH] = {"Nail_L_2.5", "Hammer", "Screwdriver", "Pliers", "Glue", "Wrench_13", "Wrench_14", "Grinder", "Tape", "Chisel"};
	const int quantities[ARRAY_LENGTH] = {300, 127, 432, 250, 78, 58, 120, 300, 1000, 78};
	const float prices[ARRAY_LENGTH] = {1.20, 12.50, 4.55, 14.99, 1.99, 8.59, 8.59, 152.62, 0.49, 19.50};

	int i = 0;

	int max;
	printf("\nInsert number of articles to generate (0-10): ");
	max = get_valid_int("Num articles to generate");

	FILE *f = fopen(INPUT_FILE, "w");
	if (f != NULL)
	{
		/* Loop until max value */
		for (i = 0; i < max; i++) {
			fprintf(f, "%s %s %.2f %d\n", names[i], ids[i], prices[i], quantities[i]); /* Writing to file */
		}
		fclose(f);
	} else {
		printf("Error opening file!\n");
		return 1;
	}

	printf("\n%d articles generated!\n\n", max);

	return 0;
}

void clear_buffer()
{
	char c;
	while ((c = getchar()) != '\n' && c != EOF);
}

int get_valid_int(char *field_name)
{
	int is_valid = 0;
	int value;
	char term;
	while (!is_valid) {
		if (scanf("%d%c", &value, &term) != 2 || term != '\n') {
			clear_buffer();
			printf("%s must be an integer, try again: ", field_name);
		} else {
			if (value < 0 || value > ARRAY_LENGTH) {
				printf("%s must be between 0 and 10, try again: ", field_name);
			} else {
				is_valid = 1;
			}
		}
	}

	return value;
}