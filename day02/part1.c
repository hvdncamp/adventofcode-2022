#include <stdio.h>

int score(int opponent, int own){
    if(opponent == own){
        return own + 4;
    } else if (own == ((opponent + 1) % 3)){
        return own + 7;
    } else {
        return own + 1;
    }
}

int main(int argc, char ** argv) {
    FILE * fptr = fopen(argv[3], "r");
    int total_score = 0;
    char read = fgetc(fptr);
    int opponent;
    int own;
    while(read != '\n'){
        opponent = read - 'A';
        fgetc(fptr);
        own = fgetc(fptr) - 'X';
        total_score += score(opponent, own);
        fgetc(fptr);
        read = fgetc(fptr);
    }
    printf("Het antwoord is %d\n", total_score);
    return 0;
}