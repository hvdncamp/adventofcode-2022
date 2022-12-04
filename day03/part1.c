#include <stdio.h>

int priority(char c) {
	if ('A' <= c && c <= 'Z') {
		return c - 'A' + 26;
	} else {
		return c - 'a';
	}
}

void initialize_occurrences(int * occurrence) {
	int i;
	for (i = 0; i < 52; i++) {
		occurrence[i] = -1;
	}
}

int main(int argc, char ** argv) {
	FILE * file = fopen(argv[3], "r");
	char item;
	int first_occurrence[52];
	int last_occurrence[52];
	int i = 0, j = 0;
	int priority_sum = 0;
	initialize_occurrences(first_occurrence);
	initialize_occurrences(last_occurrence);
	while ((item = fgetc(file)) != EOF) {
		if (item == '\n') {
			for (j = 0; j < 52; j++) {
				if (first_occurrence[j] < i / 2 && last_occurrence[j] >= i / 2) {
					fprintf(stderr, "Shared: %d %d \n", j + 1, i);
					priority_sum += j + 1;
				}
			}
			i = 0;
			initialize_occurrences(first_occurrence);
			initialize_occurrences(last_occurrence);
		} else {
			if (first_occurrence[priority(item)] == -1) {
				first_occurrence[priority(item)] = i;
			}
			last_occurrence[priority(item)] = i;
			i++;
		}
	}
	printf("%d\n", priority_sum);
	return 0;
}
