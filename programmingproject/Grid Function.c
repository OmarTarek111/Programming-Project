#include <stdio.h>
#include <stdlib.h>
#define BoldRed "\033[0;101m"
#define RED     "\033[0;41m"
#define GREEN   "\033[0;42m"
#define YELLOW  "\033[0;43m"
#define BoldBlue "\033[0;104m"
#define BLUE    "\033[0;44m"
#define MAGENTA "\033[0;45m"
#define CYAN    "\033[0;46m"
#define WHT    "\033[0;47m"
#define RESET   "\x1b[0m"
#define RRED     "\e[0;31m"
#define BBLUE    "\x1b[34m"
#define GGREEN   "\x1b[32m"
#define YYELLOW  "\x1b[33m"
#include "Grid.h"

float player1Moves, player2Moves; int remainingLines;

void generateGrid(int nd, int ndl, int cells, int gameArr[])
{
    //the grid is a 1-d array generated with values referring to different elements in the game
    int counter = 21;
    for(int i=0; i<=cells-1; i++)
    {
        if(i%2==1)
            gameArr[i]=counter++;//starting from 21 is the lines the player could connect and his input would be counter-20
        else
            gameArr[i]=10;//10 is a dot
    }
    for(int i=2*nd+1; i<=cells-2*nd; i+=2)
    {
        gameArr[i-1]=0;//the indices surrounded by lines which i call s1 and s2 in the code are assigned zero and their value is incremented by 1 when a line is conned beside them
        if((i+1)%(ndl)==0) //whenever the next index is divisible by the number of lines and dots then that means i have reached the end of the row
            i+=2*nd; //i add 2*the number of dots because it gets me to the next square index
    }
}
void printGrid(int ndl, int cells, int gameArr[], char player1Name[], char player2Name[])
{
    system("cls");
    player1Moves=0; player2Moves=0; remainingLines=0;
    int counter=4; //I implemented counter in such a way that helps me print the grid with my preferred thickness of lines and dots
    char x[]="    "; printf("\n");
    for(int i=0; i<cells; i++)
    {
    /*if((i+1)%(ndl)==0)
        printf("     %d\n\n\n\n", gameArr[i]);
    else
        printf("    %d    ", gameArr[i]);*/
       switch(gameArr[i])
        {
            case 10:
                printf(WHT"%s"RESET,x);
                if((i+1)%(ndl)==0)
                {
                    printf("\n");counter-=2;
                }
                break;
            case 11://11 is another index indicating a line made by player 1
                if(i/(ndl)%2==0)//this condition indicates a horizontal line
                {
                    player1Moves+=0.5;//adding 0.5 since this line is going to be repeated twice
                    printf(RED"%s%s"RESET,x,x);
                }
                else
                {
                    player1Moves+=0.25;//adding 0.25 since this line is going to be repeated four times
                    printf(RED"%s"RESET,x);
                }
                if((i+1)%(ndl)==0){printf("\n");counter--;}
                break;
            case 12://12 indicates a line made by player 2 or the computer
                if(i/(ndl)%2==0)//everything is the same as in previous case except the color of the line is blue is blue
                {
                    player2Moves+=0.5;
                    printf(BLUE"%s%s"RESET,x,x);
                }
                else
                {
                    player2Moves+=0.25;
                    printf(BLUE"%s"RESET,x);
                }
                if((i+1)%(ndl)==0){printf("\n");counter--;}
                break;
            case -4:printf(BoldRed"%s%s"RESET,x,x); break;//-4 indicates that this square was completed by player 1
            case 4:printf(BoldBlue"%s%s"RESET,x,x); break;//4 indicates that this square was completed by player 2
            case 3:printf("%s%s",x,x); break;
            case 2:printf("%s%s",x,x); break;
            case 1:printf("%s%s",x,x); break;
            case 0:printf("%s%s",x,x); break;
            //cases 3,2,1,0 indicate incomplete squares and they're the number of lines surrounding the square
            default://default is an empty line in which i print a number on it, if the player chooses this number then it becomes his input
                if(i/(ndl)%2==0&&counter==2)
                {
                    remainingLines++;
                    if(gameArr[i]-20<10) printf("   %d    ",gameArr[i]-20);
                    else if(gameArr[i]-20>=100) printf("   %d  ",gameArr[i]-20);
                    else printf("   %d   ",gameArr[i]-20);
                }
                else if(i/(ndl)%2==0&&counter!=2)printf("%s%s",x,x);
                else if(i/(ndl)%2!=0&&counter==2)
                {
                    remainingLines++;
                    if(gameArr[i]-20<10) printf(" %d  ",gameArr[i]-20);
                    else if(gameArr[i]-20>=100) printf("%d ",gameArr[i]-20);
                    else printf(" %d ",gameArr[i]-20);
                }

                else if(i/(ndl)%2!=0&&counter!=2)printf("%s",x);
                if((i+1)%(ndl)==0){printf("\n");counter--;}
                break;
        }
        if(counter!=0&&(i+1)%(ndl)==0) i-=ndl;
        else if((i+1)%(ndl)==0) counter=4;
    }
    printf("\n\n\t\t"RRED"%s"YYELLOW"'s total moves: "GGREEN"%.0f\t\t"BBLUE"%s"YYELLOW"'s total moves: "GGREEN"%.0f"YYELLOW"\t\tRemaining Lines: "GGREEN"%d\n\t\t"RESET,player1Name,player1Moves,player2Name,player2Moves,remainingLines);
}
