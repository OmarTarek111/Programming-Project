#ifndef INGAMEACTIONS_H_INCLUDED
#define INGAMEACTIONS_H_INCLUDED

void save(char player1Name[],int player1Score,char player2Name[],int player2Score,int currentIndex,int turn,int nd,int cells,int remainingSquares,int gameArr[],int gameMode);

void load(char player1Name[],int *player1Score,char player2Name[],int *player2Score,int *computerScore,int *currentIndex,int *turn,int *nd,int *ndl, int *cells, int *remainingSquares,int gameArr[],int *gameMode);

void playerTurn(char playerName[], int *playerScore, int *playerIndex, int nd, int ndl, int cells, int gameArr[], int *flag, int input, int *s1, int *s2);

void computerGuide(int *computerScore, int nd, int ndl, int *input, int gameArr[], int *flag, int *s1, int *s2);

#endif // INGAMEACTIONS_H_INCLUDED
