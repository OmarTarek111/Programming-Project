#ifndef AICOMP_H_INCLUDED
#define AICOMP_H_INCLUDED

void computerGuide(int *computerScore, int nd, int ndl, int *input, int gameArr[], int *flag, int s1, int s2);
void generateGrid(int nd, int ndl, int cells, int gameArr[]);
void printGrid(int ndl, int cells, int gameArr[]);
void PlayerVsPlayer(char player1Name[],int player1Score,int player1Index,char player2Name[],int player2Score,int player2Index,int *currentIndex,int remainingSquares ,int gameArr[] ,int turn,int nd,int ndl,int cells);
void PlayerVsComputer(char playerName[],int playerScore,int playerIndex,int computerScore,int remainingSquares ,int gameArr[] ,int turn,int nd,int ndl, int cells);
#endif // AICOMP_H_INCLUDED
