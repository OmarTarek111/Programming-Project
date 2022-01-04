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

void generateGrid(int nd, int ndl, int cells, int gameArr[])
{
    int counter = 21;
    for(int i=0; i<=cells-1; i++)
    {
        if(i%2==1)
            gameArr[i]=counter++;
        else
            gameArr[i]=10;
    }
    for(int i=2*nd+1; i<=cells-2*nd; i+=2)
    {
        gameArr[i-1]=0;
        if((i+1)%(ndl)==0)
            i+=2*nd;
    }
}
void printGrid(int ndl, int cells, int gameArr[])
{
    system("cls");
    int counter=4; char x[]="    "; printf("\n");
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
            case 11:
                if(i/(ndl)%2==0)
                   printf(RED"%s%s"RESET,x,x);
                else
                    printf(RED"%s"RESET,x);
                if((i+1)%(ndl)==0){printf("\n");counter--;}
                break;
            case 12:
                if(i/(ndl)%2==0)
                   printf(BLUE"%s%s"RESET,x,x);
                else
                    printf(BLUE"%s"RESET,x);
                if((i+1)%(ndl)==0){printf("\n");counter--;}
                break;
            case -4:printf(BoldRed"%s%s"RESET,x,x); break;
            case 4:printf(BoldBlue"%s%s"RESET,x,x); break;
            case 3:printf("%s%s",x,x); break;
            case 2:printf("%s%s",x,x); break; case 1:printf("%s%s",x,x); break; case 0:printf("%s%s",x,x); break;
            default:
                if(i/(ndl)%2==0&&counter==2)
                {
                    if(gameArr[i]-20<10) printf("   %d    ",gameArr[i]-20);
                    else if(gameArr[i]-20>=100) printf("   %d  ",gameArr[i]-20);
                    else printf("   %d   ",gameArr[i]-20);
                }
                else if(i/(ndl)%2==0&&counter!=2)printf("%s%s",x,x);
                else if(i/(ndl)%2!=0&&counter==2)
                {
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
    printf("\n\n");
}
