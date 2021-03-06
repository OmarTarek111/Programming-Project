#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "inGameActions.h"
#define BoldRed "\e[0;91m"
#define RED     "\e[0;31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BoldYellow "\e[0;93m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"


struct
{
    int score;
    char *name[100];
}s[10] = {0};

void Rank(int score,char name[])
{
    int check = 1;
    FILE * rank = fopen("rank.txt","r");
    for(int i=0;i<10;i++)
    {
        fscanf(rank,"%d",&s[i].score);
        fgets(s[i].name,100,rank);
        if(score > s[i].score && check)
        {
            s[i+1].score = s[i].score;
            s[i].score = score;
            strcpy(s[i+1].name,s[i].name);
            strcpy(s[i].name,name);
            i++;
            check = 0;
        }
    }
    fclose(rank);
    rank = fopen("rank.txt","w");
    for(int i=0;i<10;i++)
    {
        fprintf(rank,"%d  %s\n",s[i].score,s[i].name);
    }
    fclose(rank);
}








void PlayerVsComputer(char playerName[],int playerScore,int playerIndex,int computerScore,int remainingSquares ,int gameArr[] ,int turn,int nd,int ndl, int cells)
{
    int s1, s2;
    int wrong, input=0, redoCounter=0; char term;
    int undo[10*nd*(nd-1)], redo[10*nd*(nd-1)];
    for(int i=0; i<(10*nd*(nd-1)); i++)
    {
        undo[i]=0; redo[i]=0;
    }
    clock_t start = clock();
    FILE * log = fopen("log.txt","w");
    fprintf(log,"Game Mode: Player VS Computer\n");
    fclose(log);
    char currentPlayer[10];
    int *currentScore=&playerScore, currentIndexC=1;
        computerScore=0; int noTurn=0; int flag=0;
        char isComp='f';
        while(remainingSquares)
        {
            for(int i=0; i<10*nd*(nd-1);i++)
            {
                printf("%d ",undo[i]);
            }
            printf("\n");
            for(int i=0; i<10*nd*(nd-1);i++)
            {
                printf("%d ",redo[i]);
            }
            if(isComp=='f')
            {
            clock_t now = clock();
            flag=0; printGrid(ndl,cells,gameArr); printf("Player Score:%d\t\t\tComputerScore:%d\n", playerScore, computerScore);
            printf(YELLOW"\n\n\tEnter "GREEN"0"YELLOW" to save.\tEnter "GREEN"-1"YELLOW" to undo.\tEnter "GREEN"-2"YELLOW" to redo.\t"MAGENTA"Time "GREEN"= %d\n"RESET,(now-start)/CLOCKS_PER_SEC);
                printf(CYAN"%s"RESET"'s turn\t\t"CYAN"score"RESET": %d\n"YELLOW"Enter your move:  \n"RESET, playerName, playerScore);
            do{
                fflush(stdin);
                if(scanf("%d%c",&input,&term) != 2 || term != '\n' || input < -2 || input > 2*nd*(nd-1)||(input!=0&&input!=-1&&input!=-2&&gameArr[2*input-1]<=20)){
                printf("Enter a valid move: ");
                wrong = 1;
                }else
                    wrong = 0;
            }while(wrong);
            switch(input)
            {
            case 0:
                save(playerName,playerScore,"The Computer",computerScore,1,turn,nd,cells,remainingSquares,gameArr,2);
                return;
            case -1:
                do
                {//Undo Function
                if(turn==0)
                    continue;
                turn--;
                input=undo[5*turn]/10;
                if(undo[5*turn]%2)
                {
                    isComp='f'; currentScore=&playerScore; currentIndexC=1; noTurn=0;
                }

                else
                {
                    isComp='t'; currentScore=&computerScore; currentIndexC=2; noTurn=-1; //printf("\n\nNOTURN:%d\n\n",noTurn);
                }
                *currentScore=undo[5*turn+1]-undo[5*turn+4];
                s1=undo[5*turn+2];
                s2=undo[5*turn+3];
                gameArr[s1]=abs(gameArr[s1])-1;
                if(s2)
                    gameArr[s2]=abs(gameArr[s2])-1;
                gameArr[2*input-1]=20+input;
                remainingSquares+=undo[5*turn+4];
                for(int i=0; i<5; i++)
                {
                    redo[5*redoCounter+i]=undo[5*turn+i];
                    undo[5*turn+i]=0;
                }
                redoCounter++;
                }while(noTurn);
                continue;

                case -2: //Redo Function
                do
                {if(redoCounter==0) //if redoCounter is zero then no redos are valid
                    continue;
                redoCounter--; //decrementing the counter to the correct index in the redo array from which the necessary values are collected
                input=redo[5*redoCounter]/10; // the value in the tenth position is the input of the player
                if(redo[5*redoCounter]%2) // if the value is odd then the last player was the first otherwise it was the second
                {                         //assigning the values of the score, player index and player name to the correct player
                    isComp='f'; currentScore=&playerScore; currentIndexC=1; noTurn=0;
                }

                else
                {
                    isComp='t'; currentScore=&computerScore; currentIndexC=2; noTurn=-2;
                }
                *currentScore=redo[5*redoCounter+1]; // the score becomes the score stored in the redo array
                s1=redo[5*redoCounter+2]; // getting the index of the two squares that the line was connected between
                s2=redo[5*redoCounter+3];
                gameArr[s1]=abs(gameArr[s1])+1;

                if(s2) // if s2 equals zero then the line was in the outer edge of the array
                    gameArr[s2]=abs(gameArr[s2])+1;
                gameArr[2*input-1]=10+currentIndexC;
                remainingSquares-=redo[5*redoCounter+4]; // decreasing the amount of the remaining squares by the number of squares closed before undoing
                for(int i=0; i<5; i++) //transferring the values after redoing to the undo array and the zeroing them in the redo one
                {
                    undo[5*turn+i]=redo[5*redoCounter+i];
                    redo[5*redoCounter+i]=0;
                }
                turn++;
                if(gameArr[s1]==4&&gameArr[s2]==4)
                {
                    gameArr[s1]=4*(int)pow(-1,currentIndexC);
                    gameArr[s2]=4*(int)pow(-1,currentIndexC);
                    continue;
                }
                else if(gameArr[s1]==4) //checks if the number of squares closed then assigning the right value of 4 to its index
                {
                    gameArr[s1]=4*(int)pow(-1,currentIndexC);
                    continue;
                }
                else if(gameArr[s2]==4)
                {
                    gameArr[s2]=4*(int)pow(-1,currentIndexC);
                    continue;
                } //continues the loop if there were squares completed otherwise change to the next player
                if(currentIndexC%2)
                {
                    isComp='t'; currentScore=&computerScore; currentIndexC=2; noTurn=-2;
                }
                else
                {
                    isComp='f'; currentScore=&playerScore; currentIndexC=1; noTurn=0;
                }
                }while(noTurn);
                continue;
            default:
                noTurn=0;
                for(int i=0; i<10*nd*(nd-1);i++)
                {
                    redo[i]=0;
                }
                redoCounter=0; flag=0;
                playerTurn(playerName,&playerScore,&playerIndex, nd,ndl,cells,gameArr,&flag,input,&s1,&s2);
                undo[5*turn]=input*10+1;
                undo[5*turn+1]=playerScore;
                undo[5*turn+2]=s1;
                undo[5*turn+3]=s2;
                turn++;
                if(flag)
                {
                    remainingSquares-=flag; *currentScore+=flag;
                    undo[5*(turn-1)+1]=*currentScore;
                    undo[5*(turn-1)+4]=flag;
                    break;
                }
                isComp='t';
            }
            }else{
                flag=0; input=0;
                computerGuide(&computerScore, nd, ndl, &input, gameArr, &flag, &s1, &s2);
                undo[5*turn]=input*10+2;
                undo[5*turn+1]=computerScore;
                undo[5*turn+2]=s1;
                undo[5*turn+3]=s2;
                turn++;
                if(flag)
                {
                    printf("\nThe Computer gets %d points!!!\n", flag);
                    remainingSquares-=flag;
                    undo[5*(turn-1)+1]=computerScore;
                    undo[5*(turn-1)+4]=flag;
                }else{
                isComp='f';}
            }
           FILE * log = fopen("log.txt","a");
            fprintf(log ,"Turn = %d\n%s's Score = %d\tComputer's Score = %d\n",turn,playerName,playerScore,computerScore);
            for(int i=0; i<cells; i++)
            {
                switch(gameArr[i])
                    {
                    case 10:
                        fprintf(log,".");
                        if((i+1)%(ndl)==0)
                            fprintf(log,"\n");
                    break;
                    case 11:
                        if(i/(ndl)%2==0)
                            fprintf(log,"--");
                        else{
                            if(i%ndl){
                                    fprintf(log,"  ");}
                            fprintf(log,"|");}
                        if((i+1)%(ndl)==0){fprintf(log,"\n");}
                        break;
                    case 12:
                        if(i/(ndl)%2==0)
                        fprintf(log,"--");
                        else{
                            if(i%ndl){fprintf(log,"  ");}
                            fprintf(log,"|");}
                        if((i+1)%(ndl)==0){fprintf(log,"\n");}
                    break;
                    default:
                        if(i/(ndl)%2==0)
                            if(i/(ndl)%2==0)
                            fprintf(log,"  ");
                        else
                            fprintf(log," ");
                        if((i+1)%(ndl)==0){fprintf(log,"\n");}
                        break;
                    }
            }
            fprintf(log,"\n\n");
            fprintf(log,"\n\n----------------------------------------\n");
            fclose(log);
        }
        printGrid(ndl,cells,gameArr);
        endGame(playerName,playerScore,"The Computer",computerScore);
        Rank(playerScore,playerName);
        Rank(computerScore,"Computer");
}




void PlayerVsPlayer(char player1Name[],int player1Score,int player1Index,char player2Name[],int player2Score,int player2Index,int *currentIndex,int remainingSquares ,int gameArr[] ,int turn,int nd,int ndl,int cells)
{
    int wrong, s1, s2, input=0, redoCounter=0, flag=0; char term; int *currentScore;
    turn = 0;
    clock_t start = clock();
    int undo[10*nd*(nd-1)], redo[10*nd*(nd-1)];
    for(int i=0; i<(10*nd*(nd-1)); i++)
    {
        undo[i]=0; redo[i]=0;
    }
    FILE * log = fopen("log.txt","w");
    fprintf(log,"Game Mode: Player VS Player\n");
    fclose(log);
    char currentPlayer[10];
    if(*currentIndex==1)
    {
        strcpy(currentPlayer,player1Name);
        currentScore=&player1Score;
    }else
    {
        strcpy(currentPlayer,player2Name);
        currentScore=&player2Score;
    }
    printf("sadas0");
    while(remainingSquares)
    {
        clock_t now = clock();
        printGrid(ndl,cells,gameArr);
        printf(YELLOW"\n\n\tEnter "GREEN"0"YELLOW" to save.\tEnter "GREEN"-1"YELLOW" to undo.\tEnter "GREEN"-2"YELLOW" to redo.\t\t"MAGENTA"Timer: "GREEN"%d Seconds\n"RESET,(now-start)/CLOCKS_PER_SEC);
        printf(YELLOW"Turn"RESET": %d\n%s's turn\t\tScore: %d\n"YELLOW"Enter your move:  \n"RESET,turn, currentPlayer, *currentScore);
        do{
            fflush(stdin);
            if(scanf("%d%c",&input,&term) != 2 || term != '\n' || input < -2 || input > 2*nd*(nd-1)||(input!=0&&input!=-1&&input!=-2&&gameArr[2*input-1]<=20)){
            printf("Enter a valid move: ");
            wrong = 1;
        }else
        wrong = 0;
        }while(wrong);
        switch(input)
        {
            case -1: //Undo Function
            if(turn==0)
                continue;
            turn--;
            input=undo[5*turn]/10;
            if(undo[5*turn]%2)
            {
                strcpy(currentPlayer,player1Name); currentScore=&player1Score; currentIndex=&player1Index;
            }
            else
            {
                strcpy(currentPlayer,player2Name); currentScore=&player2Score; currentIndex=&player2Index;
            }
            *currentScore=undo[5*turn+1]-undo[5*turn+4];
            s1=undo[5*turn+2];
            s2=undo[5*turn+3];
            gameArr[s1]=abs(gameArr[s1])-1;
            if(s2)
                gameArr[s2]=abs(gameArr[s2])-1;
            gameArr[2*input-1]=20+input;
            remainingSquares+=undo[5*turn+4];
            for(int i=0; i<5; i++)
            {
                redo[5*redoCounter+i]=undo[5*turn+i];
                undo[5*turn+i]=0;
            }
            redoCounter++;
            continue;
            case 0:
                save(player1Name,player1Score,player2Name,player2Score,*currentIndex,turn,nd,cells,remainingSquares,gameArr,1);
                return;
                case -2: //Redo Function
                if(redoCounter==0) //if redoCounter is zero then no redos are valid
                    continue;
                redoCounter--; //decrementing the counter to the correct Index in the redo array from which the necessary values are collected
                input=redo[5*redoCounter]/10; // the value in the tenth position is the input of the player
                if(redo[5*redoCounter]%2) // if the value is odd then the last player was the first otherwise it was the second
                {                         //assigning the values of the Score, player Index and player Name to the correct player
                    strcpy(currentPlayer,player1Name); currentScore=&player1Score; currentIndex=&player1Index;
                }

                else
                {
                    strcpy(currentPlayer,player2Name); currentScore=&player2Score; currentIndex=&player2Index;
                }
                *currentScore=redo[5*redoCounter+1]; // the Score becomes the Score stored in the redo array
                s1=redo[5*redoCounter+2]; // getting the Index of the two squares that the line was connected between
                s2=redo[5*redoCounter+3];
                gameArr[s1]=abs(gameArr[s1])+1;
                if(s2) // if s2 equals zero then the line was in the outer edge of the array
                    gameArr[s2]=abs(gameArr[s2])+1;
                gameArr[2*input-1]=10+*currentIndex;
                remainingSquares-=redo[5*redoCounter+4]; // decreasing the amount of the remaining squares by the number of squares closed before undoing
                for(int i=0; i<5; i++) //transferring the values after redoing to the undo array and the zeroing them in the redo one
                {
                    undo[5*turn+i]=redo[5*redoCounter+i];
                    redo[5*redoCounter+i]=0;
                }
                turn++;
                if(gameArr[s1]==4&&gameArr[s2]==4)
                {
                    gameArr[s1]=4*(int)pow(-1,*currentIndex);
                    gameArr[s2]=4*(int)pow(-1,*currentIndex);
                    continue;
                }
                else if(gameArr[s1]==4) //checks if the number of squares closed then assigning the right value of 4 to its Index
                {
                    gameArr[s1]=4*(int)pow(-1,*currentIndex);
                    continue;
                }
                else if(gameArr[s2]==4)
                {
                    gameArr[s2]=4*(int)pow(-1,*currentIndex);
                    continue;
                } //continues the loop if there were squares completed otherwise change to the next player
                if(*currentIndex%2)
                {
                    strcpy(currentPlayer,player2Name); currentScore=&player2Score;  currentIndex=&player2Index;
                }
                else
                {
                    strcpy(currentPlayer,player1Name); currentScore=&player1Score;  currentIndex=&player1Index;
                }
                continue;
            default:
                for(int i=0; i<10*nd*(nd-1);i++)
                {
                    redo[i]=0;
                }
                redoCounter=0; flag=0;
                playerTurn(currentPlayer,&*currentScore,&*currentIndex, nd,ndl,cells,gameArr,&flag,input,&s1,&s2);
                undo[5*turn]=input*10+*currentIndex;
                undo[5*turn+1]=*currentScore;
                undo[5*turn+2]=s1;
                undo[5*turn+3]=s2;
                turn++;

                if(flag)
                {
                    remainingSquares-=flag; *currentScore+=flag;
                    undo[5*(turn-1)+1]=*currentScore;
                    undo[5*(turn-1)+4]=flag;
                    break;
                }
                if(*currentIndex==1){
                      printf("Index:%d",*currentIndex);
                    strcpy(currentPlayer,player2Name);currentScore=&player2Score; currentIndex=&player2Index;
                    printf("Index:%d",*currentIndex);
                }else{
                    printf("Index:%d",*currentIndex);
                    strcpy(currentPlayer,player1Name);;currentScore=&player1Score; currentIndex=&player1Index;
                    printf("Index:%d",*currentIndex);
                }
                break;
            }
            FILE * log = fopen("log.txt","a");
            fprintf(log ,"Turn = %d\n%s's Score = %d\t%s's Score = %d\n",turn,player1Name,player1Score,player2Name,player2Score);
            for(int i=0; i<cells; i++)
            {
                switch(gameArr[i])
                    {
                    case 10:
                        fprintf(log,".");
                        if((i+1)%(ndl)==0)
                            fprintf(log,"\n");
                    break;
                    case 11:
                        if(i/(ndl)%2==0)
                            fprintf(log,"--");
                        else{
                            if(i%ndl){fprintf(log,"  ");}
                            fprintf(log,"|");}
                        if((i+1)%(ndl)==0){fprintf(log,"\n");}
                        break;
                    case 12:
                        if(i/(ndl)%2==0)
                        fprintf(log,"--");
                        else{
                            if(i%ndl){fprintf(log,"  ");}
                            fprintf(log,"|");}
                        if((i+1)%(ndl)==0){fprintf(log,"\n");}
                    break;
                    default:
                        if(i/(ndl)%2==0)
                            if(i/(ndl)%2==0)
                            fprintf(log,"  ");
                        else
                            fprintf(log," ");
                        if((i+1)%(ndl)==0){fprintf(log,"\n");}
                        break;
                    }
            }
            fprintf(log,"\n\n");
            fprintf(log,"\n\n----------------------------------------\n");
            fclose(log);
        }
        printGrid(ndl,cells,gameArr);
        endGame(player1Name,player1Score,player2Name,player2Score);
        Rank(player1Score,player1Name);
        Rank(player2Score,player2Name);
}
