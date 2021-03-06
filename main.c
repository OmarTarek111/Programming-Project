#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define BoldRed "\e[0;91m"
#define RED     "\e[0;31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BoldYellow "\e[0;93m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"
#include "inGameActions.h"
#include "aicomp.h"

struct
{
    char name[10];
    int score; int index;
}player1={.score=0,.index=1},
player2={.score=0,.index=2};

void playerTurn(char playerName[], int playerScore, int *playerIndex, int nd, int ndl, int cells, int gameArr[], int *flag, int input, int *s1, int *s2)
{
    *s1=0; *s2=0;
    input=2*input-1;
    if((input)%(ndl)==0)
    {
        *s1=input+1;
        gameArr[input]=10+*playerIndex; gameArr[*s1]++;
        if(gameArr[*s1]==4) {++*flag;gameArr[*s1]=(int)pow(-1,*playerIndex)*4;}
    }else if((input)%(ndl)==2*nd-2)
    {
        *s1=input-1;
        gameArr[input]=10+*playerIndex; gameArr[*s1]++;
        if(gameArr[*s1]==4){++*flag;gameArr[*s1]=(int)pow(-1,*playerIndex)*4;}
    }else if((input)/(ndl)==0)
    {
        *s1=input+(ndl);
        gameArr[input]=10+*playerIndex; gameArr[*s1]++;
        if(gameArr[*s1]==4){++*flag;gameArr[*s1]=(int)pow(-1,*playerIndex)*4;}
    }else if((input)/(ndl)==2*nd-2)
    {
        *s1=input-(ndl);
        gameArr[input]=10+*playerIndex; gameArr[*s1]++;
        if(gameArr[*s1]==4){++*flag;gameArr[*s1]=(int)pow(-1,*playerIndex)*4;}
    }else if(((input)/(ndl))%2==0)
    {
        *s1=input-(ndl); *s2=input+(ndl);
        gameArr[input]=10+*playerIndex; gameArr[*s1]++; gameArr[*s2]++;
        if(gameArr[*s1]==4){++*flag;gameArr[*s1]=(int)pow(-1,*playerIndex)*4;}
        if(gameArr[*s2]==4){++*flag;gameArr[*s2]=(int)pow(-1,*playerIndex)*4;}
    }else if(((input)/(ndl))%2!=0)
    {
        *s1=input-1; *s2=input+1;
        gameArr[input]=10+*playerIndex; gameArr[*s1]++; gameArr[*s2]++;
        if(gameArr[*s1]==4){++*flag;gameArr[*s1]=(int)pow(-1,*playerIndex)*4;}
        if(gameArr[*s2]==4){++*flag;gameArr[*s2]=(int)pow(-1,*playerIndex)*4;}
    }

}

void changeTurn(char currentPlayer[],int **currentScore,int **currentIndex)
{
    if(**currentIndex%2)
    {
        strcpy(currentPlayer,player2.name); *currentScore=&player2.score;  *currentIndex=&player2.index;
    }
    else
    {
        strcpy(currentPlayer,player1.name); *currentScore=&player1.score;  *currentIndex=&player1.index;
    }
}

void endGame(char player1[], int player1Score, char player2[], int player2Score)
{
    if(player1Score>player2Score)
        printf(RED"\n%s"YELLOW" WON!!!\n"RESET, player1);
    else if(player1Score<player2Score)
        printf(BLUE "\n%s"YELLOW" WON!!!\n"RESET, player2);
    else
        printf(BLUE"\nDraw.\n"RESET);
}

int main()
{
    system("");
    int nd, ndl, cells, gameMode, wrong, input, computerScore, currentIndex, gameArr[300] = {0},saved,turn,remainingSquares,navigate;
    char term;
    printf(YELLOW "\n\n\n\n\n\n\n\t\t\t\t\tWELOCOME TO DOTS AND BOXES!!"RESET);
    printf("\n\n\n\n\t\t\t\t\t          1."BLUE"Start"RESET"\n\n\n\n\t\t\t\t\t          2."BLUE"Load"RESET"\n\n\n\n\t\t\t\t\t          3."BLUE"Exit"RESET);
    do{
        fflush(stdin);
        if(scanf("%d%c",&navigate,&term) != 2 || term != '\n' || navigate < 1 || navigate > 3)
        {
            printf("Enter a correct input!!:\n");
            wrong = 1;
        }else
            wrong = 0;
    }while(wrong);
    if(navigate == 2)
    {
        load(player1.name,&player1.score,player2.name,&player2.score,&currentIndex,&turn,&nd,&ndl,&cells,&remainingSquares,gameArr,&gameMode);
    }else if(navigate == 1){
    system("cls");

    printf("\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t1."RED"Player Vs. Player\n\n\n\n\n\t\t\t\t\t"RESET"2."RED"Player Vs. Computer"RESET );
    do{
        fflush(stdin);
        if(scanf("%d%c",&gameMode,&term) != 2 || term != '\n' || gameMode < 1 || gameMode > 2)
        {
            printf("Enter 1 or 2:\n");
            wrong = 1;
        }else
            wrong = 0;
    }while(wrong);

    system("cls");

    printf(RED"\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\tEnter number of dots"RESET":");
    do{
        fflush(stdin);
        if(scanf("%d%c",&nd,&term) != 2 || term != '\n' || nd < 3 || nd > 8)
        {
            printf("Enter numbers between 3 and 8:\n");
            wrong = 1;
        }else
            wrong = 0;
    }while(wrong);
    remainingSquares=(int)pow(nd-1,2);
    ndl=2*nd-1; cells=(int)pow(ndl,2);
    generateGrid(nd,ndl,cells,gameArr);
    turn=0;computerScore=0;
    }else{
        return 0;
    }
    switch(gameMode)
    {
    case 1:
        if (navigate == 1)
        {
        system("cls");
        printf(YELLOW"\n\n\n\n\n\n\n\t\t\t\tEnter player 1 name: "RESET); scanf("%s", &*player1.name);
        printf(YELLOW"\n\n\n\n\t\t\t\tEnter player 2 name: "RESET); scanf("%s", &*player2.name);
        currentIndex=player1.index;
        }
        PlayerVsPlayer(player1.name,player1.score,player1.index,player2.name,player2.score,player2.index,&currentIndex,remainingSquares,gameArr,turn,nd,ndl,cells);
        break;
    case 2:
        if(navigate == 1)
        {
        printf(RED"\n\t\t\t\t\tEnter player name: "); scanf("%s", &*player1.name);
        }
        PlayerVsComputer(player1.name,player1.score,player1.index,computerScore,remainingSquares,gameArr,turn,nd,ndl,cells);
        break;
    }
    system("pause");
    return 0;
}
