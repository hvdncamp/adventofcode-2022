#include <stdio.h>

#include "../lib/macros.h"

int main(int argc, char ** argv) {
	int calories;
	char next;
	int current_sum = 0;
	int max_sum = 0;
	FILE * fptr = fopen(argv[3], "r");
	while (fscanf(fptr, "%d", &calories) == 1){
		current_sum += calories;
		fgetc(fptr);
		next = fgetc(fptr);
		if (next == '\n'){
			max_sum = MAX(max_sum, current_sum);
			current_sum = 0;
		}
		ungetc(next, fptr);
	}
	printf("Het antwoord is %d\n", max_sum);
	return 0;
}
