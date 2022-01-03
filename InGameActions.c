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
void load(char player1Name[],int *player1Score,char player2Name[],int *player2Score,int *currentIndex,int *turn,int *nd,int *ndl, int *cells, int *remainingSquares,int gameArr[],int *gameMode)
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
    fscanf(fsave,"%d",&*player2Score);
    fscanf(fsave,"%d",&*currentIndex);
    printf("%d", *currentIndex);
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
