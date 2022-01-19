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

//the values used in the save and load arguments are all the values that the game modes functions need to operate the game

void save(char player1Name[],int player1Score,char player2Name[],int player2Score,int currentIndex,int turn,int nd,int cells,int remainingSquares,int gameArr[],int gameMode)
{
    system("cls");
    printf(YELLOW "\n\n\t\t\t\tChoose a save file!!"RESET);
    printf("\n\n\n\n\t\t\t\t          1."BLUE"Save 1"RESET"\n\n\n\n\t\t\t\t          2."BLUE"Save 2"RESET"\n\n\n\n\t\t\t\t          3."BLUE"Save 3"RESET);
    int navigate,wrong;char term;
    do{
        fflush(stdin);
        if(scanf("%d%c",&navigate,&term) != 2 || term != '\n' || navigate < 1 || navigate > 3)
        {
            printf(RED"Enter a correct input!!:\n"RESET);
            wrong = 1;
        }else
        wrong = 0;
    }while(wrong);
    FILE *fsave;
    switch (navigate)
    {
        case 1:
        fsave = fopen("save1.txt","w");
        break;
        case 2:
        fsave = fopen("save2.txt","w");
        break;
        case 3:
        fsave = fopen("save3.txt","w");
        break;
    }
    fprintf(fsave,"%d\n%s\n%d\n%s\n%d\n%d\n%d\n%d\n%d\n%d",gameMode,player1Name,player1Score,player2Name,player2Score,currentIndex,turn,nd,cells,remainingSquares);
     for(int i = 0;i<cells;i++)
    {
        fprintf(fsave ,"\n%d",gameArr[i]);
    }
    fclose(fsave);
}

void load(char player1Name[],int *player1Score,char player2Name[],int *player2Score,int *computerScore,int *currentIndex,int *turn,int *nd,int *ndl, int *cells, int *remainingSquares,int gameArr[],int *gameMode)
{
    int saved, wrong; char term;
    system("cls");
    printf(YELLOW "\n\n\n\n\n\n\n\t\t\t\t\tChoose a save file!!"RESET);
    printf("\n\n\n\n\t\t\t\t          1."BLUE"Save 1"RESET"\n\n\n\n\t\t\t\t          2."BLUE"Save 2"RESET"\n\n\n\n\t\t\t\t          3."BLUE"Save 3"RESET);
    do{
    fflush(stdin);
    if(scanf("%d%c",&saved,&term) != 2 || term != '\n' || saved < 1 || saved > 3)
    {
        printf(RED"Enter a correct input!!:\n"RESET);
        wrong = 1;
    }else
    wrong = 0;
    }while(wrong);
    FILE *fsave;
    switch (saved)
    {
    case 1:
        fsave = fopen("save1.txt","r");
    break;
    case 2:
        fsave = fopen("save2.txt","r");
    break;
    case 3:
        fsave = fopen("save3.txt","r");
    break;
    }
    fscanf(fsave,"%d",&*gameMode);
    fscanf(fsave,"%s",player1Name);
    fscanf(fsave,"%d",&*player1Score);
    fscanf(fsave,"%s",player2Name);
    if(*gameMode==1)
    {
        fscanf(fsave,"%d",&*player2Score);
    }
    else
    {
        fscanf(fsave,"%d",&*computerScore);
    }
    fscanf(fsave,"%d",&*currentIndex);
    fscanf(fsave,"%d",&*turn);
    fscanf(fsave,"%d",&*nd);
    fscanf(fsave,"%d",&*cells);
    fscanf(fsave,"%d",&*remainingSquares);
    *ndl=2*(*nd)-1;
    for(int i=0;i<*cells;i++){
        fscanf(fsave,"%d",&gameArr[i]);
    }
    system("pause");
fclose(fsave);
}

void playerTurn(char playerName[], int *playerScore, int *playerIndex, int nd, int ndl, int cells, int gameArr[], int *flag, int input, int *s1, int *s2)
{
    *s1=0; *s2=0;
    input=2*input-1; //after the player enters his input i need to convert it to what it's actually is in the game array
    if((input)%(ndl)==0)//this condition indicates that the player entered a line that's on the left outer edge of the grid
    {
        *s1=input+1;//setting the square index to the square next to the line in the array
        gameArr[input]=10+*playerIndex; gameArr[*s1]++;//the value stored in the gameArr[input] becomes 11 or 12 depending on the player input
        if(gameArr[*s1]==4) {++*flag;gameArr[*s1]=(int)pow(-1,*playerIndex)*4;} /*i a square is completed then i increase the flag which i use to indicate
         a square closing in the game mode function to increase his score, and setting the square index to -4 or 4 depending on the player index*/
    }else if((input)%(ndl)==2*nd-2)//this condition means that the line will be on the right side of the game array
    {
        *s1=input-1;
        gameArr[input]=10+*playerIndex; gameArr[*s1]++;
        if(gameArr[*s1]==4){++*flag;gameArr[*s1]=(int)pow(-1,*playerIndex)*4;}
    }else if((input)/(ndl)==0)//this condition means that the line is on the upper side of the game array
    {
        *s1=input+(ndl);
        gameArr[input]=10+*playerIndex; gameArr[*s1]++;
        if(gameArr[*s1]==4){++*flag;gameArr[*s1]=(int)pow(-1,*playerIndex)*4;}
    }else if((input)/(ndl)==2*nd-2)//this condition means that the line is on the lower side of the game array
    {
        *s1=input-(ndl);
        gameArr[input]=10+*playerIndex; gameArr[*s1]++;
        if(gameArr[*s1]==4){++*flag;gameArr[*s1]=(int)pow(-1,*playerIndex)*4;}
    }else if(((input)/(ndl))%2==0)//this condition means that the line is a horizontal one inside the game array so this time i need to change tow square indices instead of one
    {
        *s1=input-(ndl); *s2=input+(ndl);
        gameArr[input]=10+*playerIndex; gameArr[*s1]++; gameArr[*s2]++;
        if(gameArr[*s1]==4){++*flag;gameArr[*s1]=(int)pow(-1,*playerIndex)*4;}
        if(gameArr[*s2]==4){++*flag;gameArr[*s2]=(int)pow(-1,*playerIndex)*4;}
    }else if(((input)/(ndl))%2!=0)//this condition means that the line is a vertical one inside the game array so this time i need to change tow square indices instead of one
    {
        *s1=input-1; *s2=input+1;
        gameArr[input]=10+*playerIndex; gameArr[*s1]++; gameArr[*s2]++;
        if(gameArr[*s1]==4){++*flag;gameArr[*s1]=(int)pow(-1,*playerIndex)*4;}
        if(gameArr[*s2]==4){++*flag;gameArr[*s2]=(int)pow(-1,*playerIndex)*4;}
    }

}
