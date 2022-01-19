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
}s[10] = {};

void leaderboard(int h){
    char w[100];
    int i,a,check = 1;
    //load the info from the file
    FILE*rank = fopen("Rank.txt","r");
    for(int i = 0;i<h;i++){
    fscanf(rank,"%s",s[i].name);
    fscanf(rank,"%d",&s[i].score);
    }
    fclose(rank);
    system("cls");
    //print the leaderboard
    printf("\n\n\n\n\n");
    for(int i=0;i<h;i++){
    printf(YELLOW"\t\t\t\t\t\t%s:"RED"  %d\n\n",s[i].name,s[i].score);
    }
}


void Rank(int score,char name[])
{
    char w[100];
    int h = 10,a = 0,check = 0;
    FILE*rank = fopen("Rank.txt","r");
    //load the info from the file
    for(int i = 0;i<h;i++){
    fscanf(rank,"%s",s[i].name);
    fscanf(rank,"%d",&s[i].score);
    }
    fclose(rank);
    //Checks if the name is already there
    for(int i= 0;i<h;i++){
        if(!strcasecmp(s[i].name,name) && score <= s[i].score){
            check = 2;
            break;
        }
    }
    for(int i= 0;i<h;i++){
        if(!strcasecmp(s[i].name,name) && score > s[i].score){
            check = 1;
            break;
        }
        a++;
    }
    //if the name alraedy exists and the new score is higher remove it
    if(check){
        for(a;a<9;a++){
        s[a].score = s[a+1].score;
        strcpy(s[a].name,s[a+1].name);
        }
        s[9].score = 0;
        strcpy(s[9].name,"NULL");
    }
    if(check == 2){
        strcpy(name,"NULL");
        score = 0;
    }


    //add the new score if possible
     for(int i = 0;i<h;i++){
        if(score > s[i].score){
            for(int j = 0;j<h-1-i;j++){
                s[h-1-j] = s[h-2-j];
            }
            s[i].score = score;
            strcpy(s[i].name,name);
            break;
            }
        }


    //save the changes to the file
    FILE*rnk = fopen("Rank.txt","w");
    for(int i = 0;i<h;i++){
    fprintf(rnk,"%s\n",s[i].name);
    fprintf(rank,"%d\n",s[i].score);
    }
    fclose(rnk);

}

void logGrid(int gameArr[], FILE *log, int cells, int ndl)
{
    for(int i=0; i<cells; i++)
    {
        switch(gameArr[i])
        {
        case 3:fprintf(log,"  "); break;
        case 2:fprintf(log,"  "); break;
        case 1:fprintf(log,"  "); break;
        case 0:fprintf(log,"  "); break;
        case -4:
            fprintf(log,"1 ");
            break;
        case 4:
            fprintf(log,"2 ");
            break;
        case 10:
            fprintf(log,".");
            if((i+1)%(ndl)==0)
                fprintf(log,"\n");
            break;
        case 11:
            if(i/(ndl)%2==0)
                fprintf(log,"__");
            else
                fprintf(log,"|");
            if((i+1)%(ndl)==0) {
                fprintf(log,"\n");
            }
            break;
        case 12:
            if(i/(ndl)%2==0)
                fprintf(log,"__");
            else
                fprintf(log,"|");
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
    //this function compares the scores of the two players and declare who won
    if (player1Score > player2Score)
        printf(RED "\n%s"YELLOW " WON!!!\n"RESET, player1);
    else if (player1Score < player2Score)
        printf(BLUE "\n%s"YELLOW " WON!!!\n"RESET, player2);
    else
        printf(BLUE "\nDraw.\n"RESET);
}

void PlayerVsPlayer(char player1Name[],int player1Score,int player1Index,char player2Name[],int player2Score,int player2Index,int currentIndex,int remainingSquares ,int gameArr[] ,int turn,int nd,int ndl,int cells, int *navigate)
{
    /*PlayerVsPlayer is the function of the player vs player game mode*/
    int  s1, s2, input, redoCounter=0, undoCounter=0,flag=0, *currentScore;
    /*s1 and s2 are the indexes of the boxes in the game array we use them to determine if the box is closed or not if they equal 4 or -4
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
    //current player is the name of the player playing in the current turn
    char currentPlayer[20];
    /*The next section is used to determine who is the current player because
     after saving the if this section wasn't here then the player would always be the first one*/
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
        int minuteNow =((now-start)/CLOCKS_PER_SEC)/60;
        int secondNow = ((now-start)/CLOCKS_PER_SEC)%60;
        printGrid(ndl,cells,gameArr,player1Name,player2Name);
        printf(RED"%s"YELLOW"'s Score: "GREEN"%d\t\t\t"BLUE"%s"YELLOW"'s Score: "GREEN"%d"YELLOW"\n\t\tTurn:"GREEN"%d\n"RESET,player1Name, player1Score, player2Name, player2Score,turn);
        printf(YELLOW"\n\nEnter "GREEN"0"YELLOW" to save.\tEnter "GREEN"-1"YELLOW" to undo.\tEnter "GREEN"-2"YELLOW" to redo.\tEnter "GREEN"-3"YELLOW" to go back to main menu.\n"MAGENTA"Time "GREEN"= %d"YELLOW" Minutes "GREEN"%d"YELLOW" Seconds""\n"RESET,minuteNow,secondNow);
        printf("\n%s's turn\t\n"YELLOW"Enter your move:  \n"RESET, currentPlayer);
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
        case 0://0 is used to save
            save(player1Name,player1Score,player2Name,player2Score,currentIndex,turn,nd,cells,remainingSquares,gameArr,1);
            continue;

        case -1: //-1 is used to undo
            if(undoCounter==0)
                continue;
            turn--;
            undoCounter--; //decrementing the values of the turn and undoCounter after undoing
            /*the value stored in this index is, as i stated in the default case,
            the input of the player * 10 + the index of the player. so if this
            value is divisible by two then the last player was the second one otherwise its the first*/
            if(undo[5*undoCounter]%2)
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
            //the score of the player is set to equal his score minus the number of boxes closed in the turn we're going back to*/
            *currentScore=undo[5*undoCounter+1]-undo[5*undoCounter+4];
            //changing the values of the box indices to their previous value
            s1=undo[5*undoCounter+2];
            s2=undo[5*undoCounter+3];
            gameArr[s1]=abs(gameArr[s1])-1;
            if(s2) //doing the same if the line was put in between two boxes
                gameArr[s2]=abs(gameArr[s2])-1;
            input=undo[5*undoCounter]/10;//input is the input of the player in the previous turn used to remove the line in the array
            gameArr[2*input-1]=20+input; //adding 20 to make the line empty again
            remainingSquares+=undo[5*undoCounter+4];//adding the number of boxes closed in the previous turn to remainingSquares
            for(int i=0; i<5; i++)
            {
                redo[5*redoCounter+i]=undo[5*undoCounter+i];
                undo[5*undoCounter+i]=0;
            }
            redoCounter++;
            /*putting all the values used to undo this turn in the redo counter in the same order to use them in redoing then zeroing them out
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
            //if any of the boxs' indices become 4 that means that after redoing a box was closed so we need to make it either 4 or -4 depending on who closed them
            if(gameArr[s1]==4&&gameArr[s2]==4)
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
            } /*continues the loop if there were squares completed otherwise change to the next player
                if not change the player then continue*/
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
        case -3://-3 is used to go back to main menu then ending the playervsplayer function
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
            /*putting all the required values in the undo array and then incrementing the turn and undoCounter
            each turn the undo array gets 5 values the first one is input * 10 and the index of the player int ones position
            the second one is the score of the player that just played
            the third is the index of the first square that the line was connected around
            the fourth is the index of the second square and it equals zero if the line was in the outer border
            the fifth is the number of boxes closed*/
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
                /*changing the second value of the last undo to the new score if a box was closed
                and changing the value of the fifth position to the flag*/
                undo[5*(undoCounter-1)+1]=*currentScore;
                undo[5*(undoCounter-1)+4]=flag;
                break;/*break here is used to end the turn without changing the player
                      because a box was closed and the player was granted another move*/
            }
            /*if the program gets here then the player hasn't changed because he didn't complete a box
            so then the current player's turn end and the next one's begin*/
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
    } /*when the number of remaining squares gets to zero the game loop ends so we need to print the array
        and end the game by comparing the squares to see who won, then saving the ranks of the player that
        if his score was in the top ten, then setting the navigate to -3 so that after the function ends we return to the main menu*/
    printGrid(ndl,cells,gameArr,player1Name,player2Name);
    endGame(player1Name,player1Score,player2Name,player2Score);
    if(player1Score>player2Score)
        Rank(player1Score,player1Name);
    else if(player2Score>player1Score)
        Rank(player2Score,player2Name);
    *navigate=-3;
    return;
}

void PlayerVsComputer(char playerName[],int playerScore,int playerIndex,int computerScore,int remainingSquares ,int gameArr[] ,int turn,int nd,int ndl, int cells, int *navigate)
{
    /*Most variables and functions are the same as in the player vs player function since there's only one human player
     then playerScore is used to represent the score of the player while computerScore is used for the computer*/
    int s1, s2, input, redoCounter=0, undoCounter=0, flag, *currentScore=&playerScore, currentIndex=1;;
    int wrong; char term;
    clock_t start = clock();
    int minuteNow;
    int secondNow;
    int undo[10*nd*(nd-1)], redo[10*nd*(nd-1)];
    for(int i=0; i<(10*nd*(nd-1)); i++)
    {
        undo[i]=0;
        redo[i]=0;
    }
    FILE *log = fopen("log.txt","w");
    fprintf(log,"Game Mode: Player VS Computer\n");
    fclose(log);
    while(remainingSquares)
    {   //this if condition is used to determine if this turn is the player's
        if(currentIndex==1) //starting from here everything is mostly the same as in the other game mode and i added comments to explain the changes
        {
            clock_t now = clock();
            minuteNow =((now-start)/CLOCKS_PER_SEC)/60;
            secondNow = ((now-start)/CLOCKS_PER_SEC)%60;
            flag=0;
            printGrid(ndl,cells,gameArr,playerName,"The Computer");
            printf(RED"%s"YELLOW"'s Score: "GREEN"%d\t\t\t"BLUE"Computer"YELLOW"'s Score: "GREEN"%d\n\t\t"YELLOW"Turn:"GREEN"%d\n",playerName, playerScore, computerScore,turn);
            printf(YELLOW"\n\nEnter "GREEN"0"YELLOW" to save.\tEnter "GREEN"-1"YELLOW" to undo.\tEnter "GREEN"-2"YELLOW" to redo.\tEnter "GREEN"-3"YELLOW" to go back to main menu.\n"MAGENTA"Time "GREEN"= %d"YELLOW" Minutes "GREEN"%d"YELLOW" Seconds""\n"RESET,minuteNow,secondNow);
            printf(RED"%s"RESET"'s turn\t\t"RESET"\n"YELLOW"Enter your move:  \n"RESET, playerName);
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
                /*then saving in this game mode the player2Name argument of save() is set to "computer" and that of
                  player2Score is set to computerScore*/
                save(playerName,playerScore,"Computer",computerScore,1,turn,nd,cells,remainingSquares,gameArr,2);
                continue;
            case -1:
                /*the undo in this mode is exactly the same as that in the other game mode except in here a do while
                 loop is used that ends only when the index of the player is 1 indicating that there is no more undos
                 that the computer can do if it had played multiple moves*/
                do
                {
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

            case -2:
                /*the changes made to the redo are the same changes done to the undo*/
                do
                {   if(redoCounter==0)
                        continue;
                    redoCounter--;
                    input=redo[5*redoCounter]/10;
                    if(redo[5*redoCounter]%2)
                    {
                        currentScore=&playerScore;
                        currentIndex=1;
                    }
                    else
                    {
                        currentScore=&computerScore;
                        currentIndex=2;
                    }
                    *currentScore=redo[5*redoCounter+1];
                    s1=redo[5*redoCounter+2];
                    s2=redo[5*redoCounter+3];
                    gameArr[s1]=abs(gameArr[s1])+1;

                    if(s2)
                        gameArr[s2]=abs(gameArr[s2])+1;
                    gameArr[2*input-1]=10+currentIndex;
                    remainingSquares-=redo[5*redoCounter+4];
                    for(int i=0; i<5; i++)
                    {
                        undo[5*undoCounter+i]=redo[5*redoCounter+i];
                        redo[5*redoCounter+i]=0;
                    }
                    undoCounter++; turn++;
                    if(gameArr[s1]==4&&gameArr[s2]==4)
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
                    }
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
                undo[5*undoCounter]=input*10+1;
                undo[5*undoCounter+1]=playerScore;
                undo[5*undoCounter+2]=s1;
                undo[5*undoCounter+3]=s2;
                undoCounter++; turn++;
                if(flag)
                {
                    remainingSquares-=flag;
                    *currentScore+=flag;
                    undo[5*(undoCounter-1)+1]=*currentScore;
                    undo[5*(undoCounter-1)+4]=flag;
                    break;
                }
                currentIndex=2;
            }
        }
        else {
            //this is the computer's turn and most of the stuff is the same except for the computerGuide function which is basically its turn
            clock_t now = clock();
            minuteNow =((now-start)/CLOCKS_PER_SEC)/60;
            secondNow = ((now-start)/CLOCKS_PER_SEC)%60;
            flag=0;
            input=0;
            printGrid(ndl,cells,gameArr,playerName,"The Computer");
            printf(RED"%s"RESET"'s Score: %d\t\t\t"BLUE"Computer"RESET"'s Score: %d\nTurn:%d\n",playerName, playerScore, computerScore,turn);
            printf(YELLOW"\n\nEnter "GREEN"0"YELLOW" to save.\tEnter "GREEN"-1"YELLOW" to undo.\tEnter "GREEN"-2"YELLOW" to redo.\tEnter "GREEN"-3"YELLOW" to go back to main menu.\n"MAGENTA"Time "GREEN"= %d"YELLOW" Minutes "GREEN"%d"YELLOW" Seconds""\n"RESET,minuteNow,secondNow);
            printf(BLUE"The Computer"RESET"'s turn: \n"YELLOW"Enter your move:  \n"RESET);
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
    printGrid(ndl,cells,gameArr,playerName,"The Computer");
    endGame(playerName,playerScore,"The Computer",computerScore);
    if(playerScore>computerScore)
        Rank(playerScore,playerName);
    *navigate=-3;
    return;
}
