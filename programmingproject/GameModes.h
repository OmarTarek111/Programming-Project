#ifndef GAMEMODES_H_INCLUDED
#define GAMEMODES_H_INCLUDED

void PlayerVsPlayer(char player1Name[],int player1Score,int player1Index,char player2Name[],int player2Score,int player2Index,int currentIndex,int remainingSquares ,int gameArr[] ,int turn,int nd,int ndl,int cells, int *navigate);

void PlayerVsComputer(char playerName[],int playerScore,int playerIndex,int computerScore,int remainingSquares ,int gameArr[] ,int turn,int nd,int ndl, int cells, int *navigate);

#endif // GAMEMODES_H_INCLUDED
