#ifndef AICOMP_H_INCLUDED
#define AICOMP_H_INCLUDED

void PlayerVsPlayer(char player1Name[],int player1Score,int player1Index,char player2Name[],int player2Score,int player2Index,int *currentIndex,int remainingSquares ,int gameArr[] ,int turn,int nd,int ndl,int cells);

void PlayerVsComputer(char playerName[],int playerScore,int playerIndex,int computerScore,int remainingSquares ,int gameArr[] ,int turn,int nd,int ndl, int cells);
#endif // AICOMP_H_INCLUDED
