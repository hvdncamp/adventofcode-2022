#include <stdio.h>

int score(int opponent, int result){
    switch (result) {
        case 0:
            return (opponent + 2) % 3 + 1;
        case 1:
            return opponent + 4;
        case 2:
            return (opponent + 1) % 3 + 7;
        default:
            fprintf(stderr, "%d is not a valid value for result in function 'score'", result);
            return 0;
    }
}

int main(int argc, char ** argv) {
    FILE * fptr = fopen(argv[3], "r");
    int total_score = 0;
    char read = fgetc(fptr);
    int opponent;
    int result;
    while(read != '\n'){
        opponent = read - 'A';
        fgetc(fptr);
        result = fgetc(fptr) - 'X';
        total_score += score(opponent, result);
        fgetc(fptr);
        read = fgetc(fptr);
    }
    printf("Het antwoord is %d\n", total_score);
    return 0;
}