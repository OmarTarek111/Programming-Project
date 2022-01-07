#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "inGameActions.h"

#include "Grid.h"

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

void logGrid(int gameArr[], FILE *log, int cells, int ndl)
{
    for(int i=0; i<cells; i++)
    {
        switch(gameArr[i])
        {
        case -4:
            fprintf(log,"1");
            break;
        case 4:
            fprintf(log,"2");
            break;
        case 10:
            fprintf(log,".");
            if((i+1)%(ndl)==0)
                fprintf(log,"\n");
            break;
        case 11:
            if(i/(ndl)%2==0)
                fprintf(log,"__");
            else {
                if(i%ndl) {
                    fprintf(log,"  ");
                }
                fprintf(log,"|");
            }
            if((i+1)%(ndl)==0) {
                fprintf(log,"\n");
            }
            break;
        case 12:
            if(i/(ndl)%2==0)
                fprintf(log,"__");
            else {
                if(i%ndl) {
                    fprintf(log,"  ");
                }
                fprintf(log,"|");
            }
            if((i+1)%(ndl)==0) {
                fprintf(log,"\n");
            }
            break;
        default:
            if(i/(ndl)%2==0)
                fprintf(log,"  ");
            else
                fprintf(log," ");
            if((i+1)%(ndl)==0)
                fprintf(log,"\n");
            break;
        }
    }
}

void endGame(char player1[], int player1Score, char player2[], int player2Score) {
    if (player1Score > player2Score)
        printf(RED "\n%s"
            YELLOW " WON!!!\n"
            RESET, player1);
    else if (player1Score < player2Score)
        printf(BLUE "\n%s"
            YELLOW " WON!!!\n"
            RESET, player2);
    else
        printf(BLUE "\nDraw.\n"
            RESET);
}

void PlayerVsPlayer(char player1Name[],int player1Score,int player1Index,char player2Name[],int player2Score,int player2Index,int currentIndex,int remainingSquares ,int gameArr[] ,int turn,int nd,int ndl,int cells, int *navigate)
{
    /*PlayerVsPlayer is the function of the player vs player game mode*/
    int  s1, s2, input, redoCounter=0, undoCounter=0,flag=0, *currentScore;
    /*s1 and s2 are the indexes of the boxes in the game array we use them to determine if the box is closed or not
    input is the index of the line in the game array of the input of the player, redo counter is a counter for how many times we can redo,
    flag is used to determine how many boxes closed in the turn, current score is a pointer of the score of the current player.
    */
    int wrong;
    char term;
    clock_t start = clock();
    int undo[10*nd*(nd-1)], redo[10*nd*(nd-1)];
    //the arrays for undo and redo
    for(int i=0; i<(10*nd*(nd-1)); i++)
    {
        undo[i]=0;
        redo[i]=0;
    }
    FILE * log = fopen("log.txt","w");
    fprintf(log,"Game Mode: Player VS Player\n");
    fclose(log);
    char currentPlayer[20];
    //current player is the name of the player playing in the current turn
    if(currentIndex==1)
    {
        strcpy(currentPlayer,player1Name);
        currentScore=&player1Score;
    } else
    {
        strcpy(currentPlayer,player2Name);
        currentScore=&player2Score;
    }

    while(remainingSquares)
    {
        clock_t now = clock();
        printGrid(ndl,cells,gameArr);
        printf(YELLOW"\n\nEnter "GREEN"0"YELLOW" to save.\tEnter "GREEN"-1"YELLOW" to undo.\tEnter "GREEN"-2"YELLOW" to redo.\tEnter "GREEN"-3"YELLOW" to go back to main menu.\n"MAGENTA"Time "GREEN"= %d\n"RESET,(now-start)/CLOCKS_PER_SEC);
        printf(YELLOW"Turn"RESET": %d\n%s's turn\t\tScore: %d\n"YELLOW"Enter your move:  \n"RESET,turn, currentPlayer, *currentScore);
        do {
            fflush(stdin);
            if(scanf("%d%c",&input,&term) != 2 || term != '\n' || input < -3 || input > 2*nd*(nd-1)||(input!=0&&input!=-1&&input!=-2&&input!=-3&&gameArr[2*input-1]<=20)) {
                printf("Enter a valid move: ");
                wrong = 1;
            } else
                wrong = 0;
        } while(wrong);
        switch(input)
        {
        case 0:
            save(player1Name,player1Score,player2Name,player2Score,currentIndex,turn,nd,cells,remainingSquares,gameArr,1);
            continue;

        case -1: //-1 is used to undo
            if(undoCounter==0)
                continue;
            turn--;
            undoCounter--; //decrementing the values of the turn and undoCounter after undoing
            if(undo[5*undoCounter]%2) //changing the player to the previous player
            {
                strcpy(currentPlayer,player1Name);
                currentScore=&player1Score;
                currentIndex=1;
            }
            else
            {
                strcpy(currentPlayer,player2Name);
                currentScore=&player2Score;
                currentIndex=2;
            }
            *currentScore=undo[5*undoCounter+1]-undo[5*undoCounter+4];
            s1=undo[5*undoCounter+2];
            s2=undo[5*undoCounter+3];
            gameArr[s1]=abs(gameArr[s1])-1; //returning the square of the square around which the line was put to its previous value
            if(s2) //doing the same if the line was put in between two boxes
                gameArr[s2]=abs(gameArr[s2])-1;
            input=undo[5*undoCounter]/10;//input is the input of the player in the previous turn used to remove the line in the array
            gameArr[2*input-1]=20+input; //adding 20 to make the line empty again
            remainingSquares+=undo[5*undoCounter+4];
            for(int i=0; i<5; i++)
            {
                redo[5*redoCounter+i]=undo[5*undoCounter+i];
                undo[5*undoCounter+i]=0;
            }
            redoCounter++;
            /*putting all the values used to undo the turn in the redo counter then zeroing them out
            incrementing the redo counter*/

            continue;


        case -2: //Redo Function
            if(redoCounter==0) //if redoCounter is zero then no redos are valid
                continue;
            redoCounter--; //decrementing the counter to the correct Index in the redo array from which the necessary values are collected
            input=redo[5*redoCounter]/10; // the value in the tenth position is the input of the player
            if(redo[5*redoCounter]%2) // if the value is odd then the last player was the first otherwise it was the second
            {   //assigning the values of the Score, player Index and player Name to the correct player
                strcpy(currentPlayer,player1Name);
                currentScore=&player1Score;
                currentIndex=1;
            }
            else
            {
                strcpy(currentPlayer,player2Name);
                currentScore=&player2Score;
                currentIndex=2;
            }
            *currentScore=redo[5*redoCounter+1]; // the Score becomes the Score stored in the redo array
            s1=redo[5*redoCounter+2]; // getting the Index of the two squares that the line was connected between
            s2=redo[5*redoCounter+3];
            gameArr[s1]=abs(gameArr[s1])+1;
            if(s2) // if s2 equals zero then the line was in the outer edge of the array
                gameArr[s2]=abs(gameArr[s2])+1;
            gameArr[2*input-1]=10+currentIndex;
            remainingSquares-=redo[5*redoCounter+4]; // decreasing the amount of the remaining squares by the number of squares closed before undoing
            for(int i=0; i<5; i++) //transferring the values after redoing to the undo array and the zeroing them in the redo one
            {
                undo[5*undoCounter+i]=redo[5*redoCounter+i];
                redo[5*redoCounter+i]=0;
            }
            undoCounter++;
            turn++;
            if(gameArr[s1]==4&&gameArr[s2]==4)
            {
                gameArr[s1]=4*(int)pow(-1,currentIndex);
                gameArr[s2]=4*(int)pow(-1,currentIndex);
                continue;
            }
            else if(gameArr[s1]==4) //checks if the number of squares closed then assigning the right value of 4 to its Index
            {
                gameArr[s1]=4*(int)pow(-1,currentIndex);
                continue;
            }
            else if(gameArr[s2]==4)
            {
                gameArr[s2]=4*(int)pow(-1,currentIndex);
                continue;
            } //continues the loop if there were squares completed otherwise change to the next player
            if(currentIndex%2)
            {
                strcpy(currentPlayer,player2Name);
                currentScore=&player2Score;
                currentIndex=2;
            }
            else
            {
                strcpy(currentPlayer,player1Name);
                currentScore=&player1Score;
                currentIndex=1;
            }
            continue;
        case -3:
            *navigate = -3;
            return;
        default: //at the start of the turn the redo counter, redo array and flag are zeroed out
            redoCounter=0;
            flag=0;
            for(int i=0; i<10*nd*(nd-1); i++)
            {
                redo[i]=0;
            }
            playerTurn(currentPlayer,&*currentScore,&currentIndex, nd,ndl,cells,gameArr,&flag,input,&s1,&s2);
            //putting all the required values in the undo array and then incrementing the turn and undoCounter
            undo[5*undoCounter]=input*10+currentIndex;
            undo[5*undoCounter+1]=*currentScore;
            undo[5*undoCounter+2]=s1;
            undo[5*undoCounter+3]=s2;
            undoCounter++;
            turn++;

            if(flag) // if a box was closed then the score and remainingSquares values are changed accordingly
            {
                remainingSquares-=flag;
                *currentScore+=flag;
                undo[5*(turn-1)+1]=*currentScore;//storing the score and flag in the undo array
                undo[5*(turn-1)+4]=flag;
                break;/*break here is used to end the turn without changing the player
                      because a box was closed and the player was granted another move*/
            }
            if(currentIndex==1) {
                strcpy(currentPlayer,player2Name);
                currentScore=&player2Score;
                currentIndex=2;
            } else {
                strcpy(currentPlayer,player1Name);;
                currentScore=&player1Score;
                currentIndex=1;
            }
            break;
        }
        FILE * log = fopen("log.txt","a");
        logGrid(gameArr,log,cells,ndl);
        fprintf(log ,"Turn = %d\n%s's Score = %d\t%s's Score = %d\n",turn,player1Name,player1Score,player2Name,player2Score);
        fprintf(log,"\n\n");
        fprintf(log,"\n\n----------------------------------------\n");
        fclose(log);
    }
    printGrid(ndl,cells,gameArr);
    endGame(player1Name,player1Score,player2Name,player2Score);
    Rank(player1Score,player1Name);
    Rank(player2Score,player2Name);
    *navigate=-3;
    return;
}

void PlayerVsComputer(char playerName[],int playerScore,int playerIndex,int computerScore,int remainingSquares ,int gameArr[] ,int turn,int nd,int ndl, int cells, int *navigate)
{
    //Most variables are the same as in the player vs player function
    int s1, s2, input, redoCounter=0, undoCounter=0, flag, *currentScore=&playerScore, currentIndex=1;;
    int wrong; char term;
    clock_t start = clock();
    int undo[10*nd*(nd-1)], redo[10*nd*(nd-1)];
    for(int i=0; i<(10*nd*(nd-1)); i++)
    {
        undo[i]=0;
        redo[i]=0;
    }
    FILE *log = fopen("log.txt","w");
    fprintf(log,"Game Mode: Player VS Computer\n");
    fclose(log);
    char currentPlayer[20];
    while(remainingSquares)
    {
        if(currentIndex==1)
        {
            clock_t now = clock();
            flag=0;
            printGrid(ndl,cells,gameArr);
            printf("Player Score:%d\t\t\tComputerScore:%d\nTurn:%d\n", playerScore, computerScore,turn);
            printf(YELLOW"\n\nEnter "GREEN"0"YELLOW" to save.\tEnter "GREEN"-1"YELLOW" to undo.\tEnter "GREEN"-2"YELLOW" to redo.\tEnter "GREEN"-3"YELLOW" to go back to main menu.\n"MAGENTA"Time "GREEN"= %d\n"RESET,(now-start)/CLOCKS_PER_SEC);
            printf(RED"%s"RESET"'s turn\t\t"RED"score"RESET": %d\n"YELLOW"Enter your move:  \n"RESET, playerName, playerScore);
            do {
                fflush(stdin);
                if(scanf("%d%c",&input,&term) != 2 || term != '\n' || input < -3 || input > 2*nd*(nd-1)||(input!=0&&input!=-1&&input!=-2&&input!=-3&&gameArr[2*input-1]<=20)) {
                    printf("Enter a valid move: ");
                    wrong = 1;
                } else
                    wrong = 0;
            } while(wrong);
            switch(input)
            {
            case 0:
                save(playerName,playerScore,"Computer",computerScore,1,turn,nd,cells,remainingSquares,gameArr,2);
                continue;
            case -1:
                do
                {   //The Undo Function here is mostly the same as in the pvp function only with a loop that determines when it's the turn of the player again
                    if(undoCounter==0)
                        continue;
                    turn--; undoCounter--;
                    if(undo[5*undoCounter]%2)
                    {
                        currentScore=&playerScore;
                        currentIndex=1;
                    }

                    else
                    {
                        currentScore=&computerScore;
                        currentIndex=2;
                    }
                    input=undo[5*undoCounter]/10;
                    *currentScore=undo[5*undoCounter+1]-undo[5*undoCounter+4];
                    s1=undo[5*undoCounter+2];
                    s2=undo[5*undoCounter+3];
                    gameArr[s1]=abs(gameArr[s1])-1;
                    if(s2)
                        gameArr[s2]=abs(gameArr[s2])-1;
                    gameArr[2*input-1]=20+input;
                    remainingSquares+=undo[5*undoCounter+4];
                    for(int i=0; i<5; i++)
                    {
                        redo[5*redoCounter+i]=undo[5*undoCounter+i];
                        undo[5*undoCounter+i]=0;
                    }
                    redoCounter++;
                } while(currentIndex==2);
                continue;

            case -2: //The Redo Function here is mostly the same as in the pvp function only with a loop that determines when it's the turn of the player again
                do
                {   if(redoCounter==0) //if redoCounter is zero then no redos are valid
                        continue;
                    redoCounter--; //decrementing the counter to the correct index in the redo array from which the necessary values are collected
                    input=redo[5*redoCounter]/10; // the value in the tenth position is the input of the player
                    if(redo[5*redoCounter]%2) // if the value is odd then the last player was the first otherwise it was the second
                    {   //assigning the values of the score and player index to the correct player
                        currentScore=&playerScore;
                        currentIndex=1;
                    }
                    else
                    {
                        currentScore=&computerScore;
                        currentIndex=2;
                    }
                    *currentScore=redo[5*redoCounter+1]; // the score becomes the score stored in the redo array
                    s1=redo[5*redoCounter+2]; // getting the index of the two squares that the line was connected between
                    s2=redo[5*redoCounter+3];
                    gameArr[s1]=abs(gameArr[s1])+1;

                    if(s2) // if s2 equals zero then the line was in the outer edge of the array
                        gameArr[s2]=abs(gameArr[s2])+1;
                    gameArr[2*input-1]=10+currentIndex;
                    remainingSquares-=redo[5*redoCounter+4]; // decreasing the amount of the remaining squares by the number of squares closed before undoing
                    for(int i=0; i<5; i++) //transferring the values after redoing to the undo array and the zeroing them in the redo one
                    {
                        undo[5*undoCounter+i]=redo[5*redoCounter+i];
                        redo[5*redoCounter+i]=0;
                    }
                    undoCounter++; turn++; //incrementing the values in undoCounter and turn
                    if(gameArr[s1]==4&&gameArr[s2]==4) //checks if the number of squares closed then assigning the right value of 4 to its index
                    {
                        gameArr[s1]=4*(int)pow(-1,currentIndex);
                        gameArr[s2]=4*(int)pow(-1,currentIndex);
                        continue;
                    }
                    else if(gameArr[s1]==4)
                    {
                        gameArr[s1]=4*(int)pow(-1,currentIndex);
                        continue;
                    }
                    else if(gameArr[s2]==4)
                    {
                        gameArr[s2]=4*(int)pow(-1,currentIndex);
                        continue;
                    } //continues the loop if there were squares completed otherwise change to the next player
                    if(currentIndex%2)
                    {
                        currentScore=&computerScore;
                        currentIndex=2;
                    }
                    else
                    {
                        currentScore=&playerScore;
                        currentIndex=1;
                    }
                } while(currentIndex==2);
                continue;

            case -3:
                *navigate = -3;
                return;

            default: //at the start of the turn the redo counter, redo array and flag are zeroed out
                redoCounter=0; flag=0;
                for(int i=0; i<10*nd*(nd-1); i++)
                {
                    redo[i]=0;
                }
                playerTurn(playerName,&playerScore,&playerIndex, nd,ndl,cells,gameArr,&flag,input,&s1,&s2);
                //putting all the required values in the undo array and then incrementing the turn and undoCounter
                undo[5*undoCounter]=input*10+1;
                undo[5*undoCounter+1]=playerScore;
                undo[5*undoCounter+2]=s1;
                undo[5*undoCounter+3]=s2;
                undoCounter++; turn++;
                if(flag) // if a box was closed then the score and remainingSquares values are changed accordingly
                {
                    remainingSquares-=flag;
                    *currentScore+=flag;
                    undo[5*(undoCounter-1)+1]=*currentScore;
                    undo[5*(undoCounter-1)+4]=flag;
                    break;
                }
                currentIndex=2;
            }
        } else {
            clock_t now = clock();
            flag=0;
            input=0;
            printGrid(ndl,cells,gameArr);
            printf("Player Score:%d\t\t\tComputerScore:%d\nTurn:%d\n", playerScore, computerScore,turn);
            printf(YELLOW"\n\nEnter "GREEN"0"YELLOW" to save.\tEnter "GREEN"-1"YELLOW" to undo.\tEnter "GREEN"-2"YELLOW" to redo.\tEnter "GREEN"-3"YELLOW" to go back to main menu.\n"MAGENTA"Time "GREEN"= %d\n"RESET,(now-start)/CLOCKS_PER_SEC);
            printf(BLUE"The Computer"RESET"'s turn\t\t"BLUE"score"RESET": %d\n"YELLOW"Enter your move:  \n"RESET, computerScore);
            computerGuide(&computerScore, nd, ndl, &input, gameArr, &flag, &s1, &s2);
            undo[5*undoCounter]=input*10+2;
            undo[5*undoCounter+1]=computerScore;
            undo[5*undoCounter+2]=s1;
            undo[5*undoCounter+3]=s2;
            undoCounter++; turn++;
            if(flag)
            {
                printf("\nThe Computer gets %d points!!!\n", flag);
                remainingSquares-=flag;
                undo[5*(undoCounter-1)+1]=computerScore;
                undo[5*(undoCounter-1)+4]=flag;
            } else {
                currentIndex=1;
            }
        }
        FILE * log = fopen("log.txt","a");
        fprintf(log ,"Turn = %d\n%s's Score = %d\tComputer's Score = %d\n",turn,playerName,playerScore,computerScore);
        logGrid(gameArr,log,cells,ndl);
        fprintf(log,"\n\n");
        fprintf(log,"\n\n----------------------------------------\n");
        fclose(log);
    }
    printGrid(ndl,cells,gameArr);
    endGame(playerName,playerScore,"The Computer",computerScore);
    Rank(playerScore,playerName);
    Rank(computerScore,"Computer");
    *navigate=-3;
    return;
}
