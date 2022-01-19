#include <stdio.h>

#include <stdlib.h>

#include <math.h>

#include <string.h>

#define BoldRed "\e[0;91m"
#define RED "\e[0;31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BoldYellow "\e[0;93m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define RESET "\x1b[0m"
#include "inGameActions.h"

#include "GameModes.h"

struct {
    char name[10];
    int score;
    int index;
}
player1 = {.score = 0,.index = 1},
player2 = {.score = 0,.index = 2};

void changeTurn(char currentPlayer[], int ** currentScore, int ** currentIndex) {
    if ( ** currentIndex % 2) {
        strcpy(currentPlayer, player2.name);* currentScore = & player2.score;* currentIndex = & player2.index;
    } else {
        strcpy(currentPlayer, player1.name);* currentScore = & player1.score;* currentIndex = & player1.index;
    }
}

int main() {
    int wrong, navigate; char term;
    /*wrong and term are used to determine whether the input is wrong or not, navigate is used to navigate the main menu*/
    int nd, ndl, cells, gameMode, computerScore, currentIndex, gameArr[300] = {0}, turn, remainingSquares;
    /*The main variables used in the game loop
    nd is the number of dots in a row, ndl is the number of dots and lines in a row, cells is the required size of the array,
    remaininingSquares is the number of squares still empty in the game.*/
    jmp:
    system("cls");
    printf(YELLOW "\n\n\n\n\n\n\n\t\t\t\t\tWELOCOME TO DOTS AND BOXES!!"
        RESET);
    printf("\n\n\n\n\t\t\t\t\t          1."
        BLUE "Start"
        RESET "\n\n\n\n\t\t\t\t\t          2."
        BLUE "Load"
        RESET "\n\n\n\n\t\t\t\t\t          3."
        BLUE "Leader Board"
        RESET"\n\n\n\n\t\t\t\t\t          4."
        BLUE "Exit"RESET);
    do {
        fflush(stdin);
        if (scanf("%d%c", & navigate, & term) != 2 || term != '\n' || navigate < 1 || navigate > 4) {
            printf("Enter a correct input!!:\n");
            wrong = 1;
        } else
            wrong = 0;
    } while (wrong);
    if (navigate == 2) { //if the player enters two then he wants to load a game

        load(player1.name, & player1.score, player2.name, & player2.score, & computerScore, & currentIndex, & turn, & nd, & ndl, & cells, & remainingSquares, gameArr, & gameMode);

    } else if (navigate == 1) {//if the player enters one then start a new game

        system("cls");

        printf("\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t1."
            RED "Player Vs. Player\n\n\n\n\n\t\t\t\t\t"
            RESET "2."
            RED "Player Vs. Computer"
            RESET);
        do {
            fflush(stdin);
            if (scanf("%d%c", & gameMode, & term) != 2 || term != '\n' || gameMode < 1 || gameMode > 2) {
                printf("Enter 1 or 2:\n");
                wrong = 1;
            } else
                wrong = 0;
        } while (wrong);

        system("cls");

        printf(RED "\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\tEnter number of dots"
            RESET ":");
        do {
            fflush(stdin);
            if (scanf("%d%c", & nd, & term) != 2 || term != '\n' || nd < 3 || nd > 8) {
                printf("Enter numbers between 3 and 8:\n");
                wrong = 1;
            } else
                wrong = 0;
        } while (wrong);
        //formulas i used to calcluate each of remainingSquares, number of dots and lines per row, and cells
        remainingSquares = (int) pow(nd - 1, 2);
        ndl = 2 * nd - 1;
        cells = (int) pow(ndl, 2);
        generateGrid(nd, ndl, cells, gameArr);
        turn = 1;
        computerScore = 0;

    } else if(navigate==3){
        leaderboard(10);
        system("pause");
        goto jmp;
    }
    else
        return 0;
    switch (gameMode) {
    case 1: //game mode 1 is the player vs player and everything is handled in its function
        if (navigate == 1) {
            system("cls");
            printf(YELLOW "\n\n\n\n\n\n\n\t\t\t\tEnter player 1 name: "
                RESET);
            scanf("%s", & * player1.name);
            printf(YELLOW "\n\n\n\n\t\t\t\tEnter player 2 name: "
                RESET);
            scanf("%s", & * player2.name);
            currentIndex = player1.index;
        }
        PlayerVsPlayer(player1.name, player1.score, player1.index, player2.name, player2.score, player2.index, currentIndex, remainingSquares, gameArr, turn, nd, ndl, cells, &navigate);
        break;
    case 2: //game mode 2 is the player vs computer and everything is handled in its function
        if (navigate == 1) {
            printf(RED "\n\t\t\t\t\tEnter player name: ");
            scanf("%s", & * player1.name);
        }
        PlayerVsComputer(player1.name, player1.score, player1.index, computerScore, remainingSquares, gameArr, turn, nd, ndl, cells, &navigate);
        break;
    }
    /*when both the game modes functions end the navigate is set to -3 and then the program goes back to the beginning of the main function to open the main menu*/
    system("pause");
    if(navigate==-3)
        goto jmp;
    return 0;
}
