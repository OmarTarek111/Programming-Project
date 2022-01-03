#ifndef INGAMEACTIONS_H_INCLUDED
#define INGAMEACTIONS_H_INCLUDED
void save(char player1Name[],int player1Score,char player2Name[],int player2Score,int currentIndex,int turn,int nd,int cells,int remainingSquares,int gameArr[],int gameMode);

void load(char player1Name[],int *player1Score,char player2Name[],int *player2Score,int *currentIndex,int *turn,int *nd,int *ndl, int *cells, int *remainingSquares,int gameArr[],int *gameMode);

#endif // INGAMEACTIONS_H_INCLUDED
