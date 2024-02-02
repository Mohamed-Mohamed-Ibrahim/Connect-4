#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <ctype.h>

typedef struct{
    int height, width,Highscores;                               //struct for game parameters
}dim;

struct{
    char namePlayer[101];int score;                             //struct for winner player
}playerHigh;



void swap(int* xp, int* yp){
    int temp = *xp;         //function for swap which used in sorting high scores
    *xp = *yp;
    *yp = temp;
}

typedef struct {
    char sign, color; int HighestScore,scoreNow,moveno; char name[101];         //struct for players and computer
}player;

typedef struct {
    int hour,min,sec;                       //struct for time
}gameTime;

int EndGame(int row, int col, char grid[][col]){
    int endgame=0,i,j,k;int endgameodd=0;                          //EndGame fn determines the end of the game when the grid is full or full except one solt
    for(j=0;j<col;j++){
        if(grid[0][j]!=32){
            endgame++;
        }
    }
    for(j=0;j<col;j++){
        if(grid[1][j]!=32){
            endgameodd++;
        }
    }
    if(endgame==col){
        return 0;
    }
    else if(endgame==col-1&&row*col%2==1&&endgameodd==col){  //if number of slots are odd there will be one remaining slot
        return 0;
    }
    else return 1;
}

void printGrid(char *arr, int row, int col, char sign1, char sign2, int color1, int color2){
     int i,j,k;
     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);           //fn for printing grid with colors
     for(k=0;k<2*col;k++)printf("%c",95);printf("\n");
        for(i=0;i<row;i++){
            for(j=0;j<col;j++){
                if(*((arr+i*col)+j)==sign1){
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),color1);
                    printf("%c",*((arr+i*col)+j));
                }
                else if(*((arr+i*col)+j)==sign2){

                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),color2);
                    printf("%c",*((arr+i*col)+j));
                }
                else{
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
                    printf("%c",*((arr+i*col)+j));
                }
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
                printf("|");
            }
            printf("\n");
            for(k=0;k<2*col;k++)printf("%c",196);printf("\n");
            for(k=0;k<2*col;k++)printf("%c",95);printf("\n");
        }
}

int validMove(char a[], int move, int col){
    while(*(a+move)!=32){                                   //fn checks if the player move is valid
        printf("The move is not valid. Enter another:");
        scanf("%d", &move);
    }
    while(move<0||move>col-1){
        printf("The move is not valid. Enter another:");
        scanf("%d", &move);
    }
    return move;
}

int validMoveComp(int row, int col, char grid[][col], int move){
    int movenew=move,i,j;
    while(grid[0][movenew]!=32){ //check if the computer random move is valid
        movenew=rand()%col;
    }
    return movenew;
}

void spaceSpread(int row, int col, char arr[][col]){
    int i,j;                        //fn fills the grid with empty slots
    for(i=0;i<row;i++){
        for(j=0;j<col;j++){
            arr[i][j]=32;
        }
    }
}

void zeroSpread(int un[],int n){
    memset(un,0,sizeof(un[0])*n);           //fn initializes any 1D array with zero used in undo and redo arrays
}

int scoreNow(int row, int col, char arr[][col], char sign){
    int diag=0, count=0,scoreno=0,score[col*row],scoreindex,i,j;  //fn calculates the score of players and computer

    for(i=0;i<row;i++){                     //row count
        count=0;
        for(j=0;j<col;j++){
            if(arr[i][j]==sign){
                count++;
            }
            else count=0;
            if(count==4)scoreno++,count=0;
        }
    }
    for(i=0;i<row;i++){                     //col count
        count=0;
        for(j=0;j<col;j++){
            if(arr[j][i]==sign){
                count++;
            }
            else count=0;
            if(count==4)scoreno++,count=0;
        }
    }
    for(diag=col-1;diag>=0;diag--){         //diag left to right
        count=0;
        for(i=0,j=diag;j<col;i++,j++){
            if(arr[i][j]==sign){
                count++;
            }
            else count=0;
            if(count==4)scoreno++,count=0;
        }
    }
    for(diag=1;diag<row;diag++){            //diag left to right
        count=0;
        for(i=diag,j=0;i<row;i++,j++){
            if(arr[i][j]==sign){
                count++;
            }
            else count=0;
            if(count==4)scoreno++,count=0;
        }
    }
    for(diag=0;diag<col;diag++){            //diag right to left
        count=0;
        for(i=0,j=diag;j>=0;i++,j--){
            if(arr[i][j]==sign){
                count++;
            }
            else count=0;
            if(count==4)scoreno++,count=0;
        }
    }
    for(diag=1;diag<row;diag++){            //diag right to left
        count=0;
        for(i=diag,j=col-1;i<row;i++,j--){
            if(arr[i][j]==sign){
                count++;
            }
            else count=0;
            if(count==4)scoreno++,count=0;
        }
    }

    return scoreno;
}

gameTime timeGame(time_t start, time_t end){
    int timediff=difftime(end,start);int h,m,s;gameTime finalTime;          //fn calculates game time
    if(timediff>=3600){
        h=timediff/3600;
        m=(timediff/60)-(h*60);
        s=timediff-(h*3600)-(m*60);
        finalTime.hour=h,finalTime.min=m,finalTime.sec=s;
        return finalTime;
    }
    else if(timediff>=60){
        h=00;
        m=(timediff/60);
        s=timediff-(m*60);
        finalTime.hour=h,finalTime.min=m,finalTime.sec=s;
        return finalTime;
    }
    else{
        h=00;m=00;
        finalTime.hour=h,finalTime.min=m,finalTime.sec=timediff;
        return finalTime;
    }

}

int Redo(char sign1, char sign2, int redo1[],int redo2[], int *redocount1,int *redocount2, int row, int col, char arr[][col], int color1, int color2){
    int i,j,endgame,move1,move2,end=1;                              //redo fn
    while(end==1&&*redocount1>=1&&*redocount2>=1){
        move1=redo1[*redocount1],move2=redo2[*redocount2];
        (*redocount1)--;(*redocount2)--;
        for(i=0;(i<row)&&(arr[i][move1]==32);i++);i--;
        arr[i][move1]=sign1;
        for(i=0;(i<row)&&(arr[i][move2]==32);i++);i--;
        arr[i][move2]=sign2;
        printGrid(arr, row, col,sign1,sign2,color1,color2);
        printf("\n1:Redo\nany no:Exit Redo Option\nSelection: ");
        scanf("%d",&endgame);
        switch(endgame){
            case 1:end=1;break;
            default:end=0;break;
        }
    }
    if(*redocount1<=1&&*redocount2<=1){
        printf("\nNot a valid option\n");
    }
    return 0;
}

void Undo(dim a,int undo1[], int undo2[],char sign1, char sign2, int *undocount1, int *undocount2, int *redocount1,int *redocount2, int row, int col, char arr[][col], int redo1[], int redo2[], int color1, int color2, int *mode){
    int j,endgame,move1,i,move2,end=1;                                      //undo fn

    while(end==1&&*undocount1>0&&*undocount2>0){
        (*redocount1)++;(*redocount2)++;(*undocount1)--;(*undocount2)--;
        move1=undo1[*undocount1],move2=undo2[*undocount2];
        for(i=0;i<row&&(arr[i][move1]!=sign1);i++);
        arr[i][move1]=32;
        redo1[*redocount1]=move1;
        for(i=0;i<row&&(arr[i][move2]!=sign2);i++);
        arr[i][move2]=32;
        printGrid(arr, row, col,sign1,sign2,color1,color2);
        redo2[*redocount2]=move2;
        printf("\n1:Undo\n2:Redo\n3:Save\nany no:Exit Undo Option\nSelection: ");
        scanf("%d",&endgame);
        switch(endgame){
            case 1:end=1;break;
            case 2:end=Redo(sign1,sign2,redo1,redo2,redocount1,redocount2,row,col,arr,color1,color2);break;
            case 3:(*undocount1)++;(*undocount2)++;saveAuto(undo1,undo2,undocount1,undocount2,a,mode);break;
            default:end=0,*redocount1=0,*redocount2=0;break;
        }
    }
    if(*undocount1<=0&&*undocount2<=0&&*redocount1>1){
        printf("\nNot a valid option\n");
    }
}

int randomColor(int precolor){
    srand(time(NULL));                          //fn for giving each player a random color
    int randcolor=rand()%15;
    while(randcolor==precolor||randcolor==0)randcolor=rand()%15;
    return randcolor;
}

void allStringLower(char name[], int size){
    int i;                  //fn selets all string charaters for better storing
    for(i=0;i<=size;i++)name[i]=tolower(name[i]);
}

void updatePlayerScore(FILE *fp_out, char playerName[], int *playerScore){
    int offest;                                         //fn for updating teh player newscore
    for(int j=0;j<10;j++){
        offest=sizeof(playerHigh)*j;
        fseek(fp_out,offest,SEEK_SET);
        fread(&playerHigh,sizeof(playerHigh),1,fp_out);
        if(!strcmp(playerName,playerHigh.namePlayer)){
            if((*playerScore)>playerHigh.score){
                playerHigh.score=(*playerScore);
                fseek(fp_out,0,SEEK_END);
                fwrite(&playerHigh,sizeof(playerHigh),1,fp_out);
            }
            printf("Score:%d",playerHigh.score);
            break;
        }
    }
}

void HumVsCom(dim a, char grid[][a.width]){
    //human vs computer mode
    gameTime finalTime;int redocount1=0,redocount2=0,undocount1=0,undocount2=0,end=1,i,j,move,endgame;
    int undo1[(a.width*a.height)/2],redo1[(a.width*a.height)/2],undo2[(a.width*a.height)/2],redo2[(a.width*a.height)/2], mode=0;

    zeroSpread(undo1,(a.width*a.height)/2);zeroSpread(undo2,(a.width*a.height)/2);
    zeroSpread(redo1,(a.width*a.height)/2);zeroSpread(redo2,(a.width*a.height)/2);
    time_t beginGame=time(NULL); //begin time game
    srand(time(NULL));
    int color1=randomColor(0);
    int color2=randomColor(color1);
    player player1,computer;
    player1.sign=88,player1.color=color1,player1.scoreNow=0,player1.moveno=0;
    computer.sign=79,computer.color=color2,computer.scoreNow=0,computer.moveno=0;
    do{
        system("cls");
        printGrid(grid, a.height, a.width,player1.sign,computer.sign,player1.color,computer.color);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),player1.color);
        printf("Enter your move:");                                                                  //read move
        if(!scanf("%d",&move)){
            move=rand()%a.width;
            move=validMoveComp(a.height,a.width,grid,move);
        }                                                                                           //another check for valid move

        move=validMove(grid,move,a.width);
        for(i=0;(i<a.height)&&(grid[i][move]==32);i++);i--;                                         //do move
        grid[i][move]=player1.sign;

        undo1[undocount1]=move;undocount1++;
        printGrid(grid, a.height, a.width,player1.sign,computer.sign,player1.color,computer.color); //printing grid and turn and score and time and no of moves
        player1.moveno++;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),player1.color);
        printf("Number of moves:\nplayer:%d\ncomputer:%d\n",player1.moveno,computer.moveno);
        player1.scoreNow=scoreNow(a.height,a.width,grid,player1.sign);
        printf("Your score: %d\n",player1.scoreNow);
        time_t timeNow=time(NULL);
        finalTime=timeGame(beginGame, timeNow);
        printf("Game time: %d:%d:%d\n", finalTime.hour,finalTime.min,finalTime.sec);
        //time
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),computer.color);
        move=rand()%a.width;                                                                        //random move for computer
        move=validMoveComp(a.height,a.width,grid,move);

        for(i=0;(i<a.height)&&grid[i][move]==32;i++);i--;
        grid[i][move]=computer.sign;
        undo2[undocount2]=move;undocount2++;
        printGrid(grid, a.height, a.width,player1.sign,computer.sign,player1.color,computer.color); //printnig same staff
        computer.moveno++;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),computer.color);
        printf("Number of moves:\nplayer:%d\ncomputer:%d\n",player1.moveno,computer.moveno);
        computer.scoreNow=scoreNow(a.height,a.width,grid,computer.sign);
        printf("Your score: %d\n",computer.scoreNow);
        timeNow=time(NULL);
        finalTime=timeGame(beginGame, timeNow);
        printf("Game time: %d:%d:%d\n", finalTime.hour,finalTime.min,finalTime.sec);
        //time
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
        printf("1:Contine\n2:Undo\n3:Save\nany no:Exit\nSelection: ");
        scanf("%d",&endgame);

        switch(endgame){
            case 1:break;
            case 2:Undo(a,undo1,undo2,player1.sign,computer.sign,&undocount1,&undocount2,&redocount1,&redocount2,a.height,a.width,grid,redo1,redo2,player1.color,computer.color,&mode);break;
            case 3:saveAuto(undo1,undo2,&undocount1,&undocount2,a,&mode);return;break;
            default:exit(1);break;
        }
    }while((EndGame(a.height,a.width,grid))&&(end==1));
    system("cls");
    printf("Game has ended\n");
    int found=0,len,k=1,offest=0,rank=1;char name[101];
    FILE *fp_out=NULL,*fp_in=NULL,*Sorted=NULL;                         //MOdifying files part
    fp_out=fopen("scores list.txt","ab");
    fp_in=fopen("scores list.txt","rb");
    int scoreplayer[a.Highscores],scoresCount=0;
    printGrid(grid, a.height, a.width,player1.sign,computer.sign,player1.color,computer.color);
    if(player1.scoreNow>computer.scoreNow){
        fflush(stdin);
        printf("\nWell Done... You have won the game\nPlease... Enter Your Name:\n");
        fgets(player1.name,101,stdin);
        player1.name[strlen(player1.name)-1]='\0';
        allStringLower(player1.name,30);
        while(!feof(fp_in)&&found==0){                                  //check if player is in scores file
            if(fread(name,sizeof(name),1,fp_in)==0)break;
            len=strlen(name);
            if(!memcmp(name,player1.name,len))found=1;
        }
        fclose(fp_in);
        if(found==1){                                                   //if found, player's score will be updated if it is larger than the last one
            fp_out=NULL;
            fp_out=fopen("High.txt","a+b");
            updatePlayerScore(fp_out,player1.name,&player1.scoreNow);
            rewind(fp_out);
            while(!feof(fp_out)){                                       //rest of if is printing rank and score of player plus sorting high scores in a file
                if(fread(&playerHigh,sizeof(playerHigh),1,fp_out)==0)break;
                scoreplayer[scoresCount]=playerHigh.score;
                scoresCount++;
            }
            for(i=0;i<scoresCount;i++){
                for(j=0;j<(scoresCount-1-i);j++){
                    if(scoreplayer[j]<scoreplayer[j+1]){
                        swap(&scoreplayer[j],&scoreplayer[j+1]);
                    }
                }
            }
            for(i=0;i<scoresCount;i++){
                if(scoreplayer[i]==scoreplayer[i-1])continue;
                if(player1.scoreNow==scoreplayer[i]){
                    printf("\nRank:%d\n",rank);
                    break;
                }
                else rank++;
            }
            FILE *Sorted=NULL;rank=1;
            remove("Highscoressorted.txt");
            Sorted=fopen("Highscoressorted.txt","wb");
            printf("Rank\tScore\n");
            for(i=0;i<10;i++){
                fseek(fp_out,0,SEEK_SET);
                for(int j=0;j<10;j++){
                    offest=sizeof(playerHigh)*j;
                    fseek(fp_out,offest,0);
                    fread(&playerHigh,sizeof(playerHigh),1,fp_out);
                    found=0;
                    if(playerHigh.score==scoreplayer[i]){
                        printf("%d\t%d\n",rank,playerHigh.score);rank++;
                        fwrite(&playerHigh.score,sizeof(playerHigh.score),1,Sorted);
                        break;
                    }
                }
            }
            fclose(Sorted);
            fclose(fp_out);
        }
        else{                                                           //if not found, player will be added to scores file
            fflush(stdin);
            fwrite(player1.name,sizeof(player1.name),1,fp_out);
            fclose(fp_out);
            fp_out=fopen("High.txt","a+b");
            strcpy(playerHigh.namePlayer,player1.name);playerHigh.score=player1.scoreNow;
            printf("Score:%d",playerHigh.score);
            fwrite(&playerHigh,sizeof(playerHigh),1,fp_out);
            rewind(fp_out);
            while(!feof(fp_out)){                                       //rest of else is printing rank and score of player plus sorting high scores in a file
                if(fread(&playerHigh,sizeof(playerHigh),1,fp_out)==0)break;
                scoreplayer[scoresCount]=playerHigh.score;
                scoresCount++;
            }
            for(i=0;i<scoresCount;i++){
                for(j=0;j<(scoresCount-1-i);j++){
                    if(scoreplayer[j]<scoreplayer[j+1]){
                        swap(&scoreplayer[j],&scoreplayer[j+1]);
                    }
                }
            }
            for(i=0;i<scoresCount;i++){
                if(scoreplayer[i]==scoreplayer[i-1])continue;
                if(player1.scoreNow==scoreplayer[i]){
                    printf("\nRank:%d\n",rank);
                    break;
                }
                else rank++;
            }
            FILE *Sorted=NULL;rank=1;
            remove("Highscoressorted.txt");
            Sorted=fopen("Highscoressorted.txt","wb");
            printf("Rank\tScore\n");
            for(i=0;i<10;i++){
                fseek(fp_out,0,SEEK_SET);
                for(int j=0;j<10;j++){
                    offest=sizeof(playerHigh)*j;
                    fseek(fp_out,offest,0);
                    fread(&playerHigh,sizeof(playerHigh),1,fp_out);
                    found=0;
                    if(playerHigh.score==scoreplayer[i]){
                        printf("%d\t%d\n",rank,playerHigh.score);rank++;
                        fwrite(&playerHigh.score,sizeof(playerHigh.score),1,Sorted);
                        break;
                    }
                }
            }
            fclose(Sorted);
            fclose(fp_out);
        }
    }
    else if(player1.scoreNow<computer.scoreNow){                        //if it is a win by computer. Nothing happens
        printf("\nHard Luck... You have lost the game\n");
    }
    else{
        printf("Good Luck Next Time... A Draw\n");                      //if it is a draw. Nothing happens
    }


    printf("1:Main Menu\n2:Save\nany no:Exit\nSelection: ");
    scanf("%d",&endgame);
    switch(endgame){
        case 1:return;break;
        case 2:saveAuto(undo1,undo2,&undocount1,&undocount2,a,&mode);break;
        default:exit(1);break;
    }
}

void HumVsHum(dim a,char grid[][a.width]){
    gameTime finalTime;int redocount1=0,redocount2=0,undocount1=0,undocount2=0,end=1,i,j,move,endgame;
    int undo1[(a.width*a.height)/2],redo1[(a.width*a.height)/2],undo2[(a.width*a.height)/2],redo2[(a.width*a.height)/2], mode=1; //redo and undo arrays
    zeroSpread(undo1,(a.width*a.height)/2);zeroSpread(undo2,(a.width*a.height)/2);
    zeroSpread(redo1,(a.width*a.height)/2);zeroSpread(redo2,(a.width*a.height)/2);
    time_t beginGame=time(NULL);                                                    //begin time game
    srand(time(NULL));
    int color1=randomColor(0);                                                      //random colors for players
    int color2=randomColor(color1);
    player player1,player2;                                                         //players information
    player1.sign=88,player1.color=color1,player1.scoreNow=0,player1.moveno=0;
    player2.sign=79,player2.color=color2,player2.scoreNow=0,player2.moveno=0;
    do{
        system("cls");
        printGrid(grid, a.height, a.width,player1.sign,player2.sign,player1.color,player2.color);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),player1.color);
        printf("Player 1 Turn\nEnter your move:");                                  //read move
        if(!scanf("%d",&move)){
            move=rand()%a.width;
            move=validMoveComp(a.height,a.width,grid,move);                         //another check for valid move
        }
        move=validMove(grid,move,a.width);
        undo1[undocount1]=move;undocount1++;

        for(i=0;(i<a.height)&&(grid[i][move]==32);i++);i--;                         //do move
        grid[i][move]=player1.sign;

        printGrid(grid, a.height, a.width,player1.sign,player2.sign,player1.color,player2.color);
        player1.moveno++;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),player1.color);     //printing grid and turn and score and time and no of moves
        player1.moveno++;
        printf("Number of moves:\nPlayer1:%d\nPlayer2:%d\n",player1.moveno,player2.moveno);

        player1.scoreNow=scoreNow(a.height,a.width,grid,player1.sign);              //score function
        printf("Your score: %d\n",player1.scoreNow);
        time_t timeNow=time(NULL);
        finalTime=timeGame(beginGame, timeNow);                                     //calculating time
        printf("Game time: %d:%d:%d\n", finalTime.hour,finalTime.min,finalTime.sec);

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
        printf("1:Contine\n2:Undo\n3:Save\nany no:Exit\nSelection: ");
        scanf("%d",&endgame);
        switch(endgame){
            case 1:break;
            case 2:Undo(a,undo1,undo2,player1.sign,player2.sign,&undocount1,&undocount2,&redocount1,&redocount2,a.height,a.width,grid,redo1,redo2,player1.color,player2.color,&mode);break;
            case 3:saveAuto(undo1,undo2,&undocount1,&undocount2,a,&mode);return;break;
            default: exit(1);break;
        }

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),player2.color);
        printf("Player 2 Turn\nEnter your move:");
        if(!scanf("%d",&move)){
            move=rand()%a.width;
            move=validMoveComp(a.height,a.width,grid,move);
        }
        move=validMove(grid,move,a.width);
        undo2[undocount2]=move;undocount2++;

        for(i=0;(i<a.height)&&grid[i][move]==32;i++);i--;
        grid[i][move]=player2.sign;

        printGrid(grid, a.height, a.width,player1.sign,player2.sign,player1.color,player2.color);
        player2.moveno++;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),player2.color);
        printf("Number of moves:\nPlayer1:%d\nPlayer2:%d\n",player1.moveno,player2.moveno);
        player2.scoreNow=scoreNow(a.height,a.width,grid,player2.sign);
        printf("Your score: %d\n",player2.scoreNow);
        timeNow=time(NULL);
        finalTime=timeGame(beginGame, timeNow);
        printf("Game time: %d:%d:%d\n", finalTime.hour,finalTime.min,finalTime.sec);
        //time
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
        printf("1:Contine\n2:Undo\n3:Save\nany no:Exit\nSelection: ");
        scanf("%d",&endgame);
        switch(endgame){
            case 1:break;
            case 2:Undo(a,undo1,undo2,player1.sign,player2.sign,&undocount1,&undocount2,&redocount1,&redocount2,a.height,a.width,grid,redo1,redo2,player1.color,player2.color,&mode);break;
            case 3:saveAuto(undo1,undo2,&undocount1,&undocount2,a,&mode);return;break;
            default: exit(1);break;
        }
    }while((EndGame(a.height,a.width,grid))&&(end==1));
    system("cls");
    printf("Game has ended\n");
    int found=0,len,k=1,offest=0,rank=1;char name[101];
    FILE *fp_out=NULL,*fp_in=NULL,*Sorted=NULL;;
    fp_out=fopen("scores list.txt","ab");
    fp_in=fopen("scores list.txt","rb");
    int scoreplayer[a.Highscores],scoresCount=0;
    printGrid(grid, a.height, a.width,player1.sign,player2.sign,player1.color,player2.color);

    if(player1.scoreNow>player2.scoreNow){
        fflush(stdin);
        printf("\nWell Done Player1... You have won the game\nPlease... Enter Your Name:\n");
        fgets(player1.name,101,stdin);
        allStringLower(player1.name,1000);
        while(!feof(fp_in)&&found==0){
            if(fread(name,sizeof(name),1,fp_in)==0)break;
            len=strlen(name);
            if(!memcmp(name,player1.name,len))found=1;
        }
        fclose(fp_in);
        if(found==1){
            fp_out=NULL;
            fp_out=fopen("High.txt","a+b");
            updatePlayerScore(fp_out,player1.name,&player1.scoreNow);
            rewind(fp_out);
            while(!feof(fp_out)){
                if(fread(&playerHigh,sizeof(playerHigh),1,fp_out)==0)break;
                scoreplayer[scoresCount]=playerHigh.score;
                scoresCount++;
            }
            for(i=0;i<scoresCount;i++){
                for(j=0;j<(scoresCount-1-i);j++){
                    if(scoreplayer[j]<scoreplayer[j+1]){
                        swap(&scoreplayer[j],&scoreplayer[j+1]);
                    }
                }
            }
            for(i=0;i<scoresCount;i++){
                if(scoreplayer[i]==scoreplayer[i-1])continue;
                if(player1.scoreNow==scoreplayer[i]){
                    printf("\nRank:%d\n",rank);
                    break;
                }
                else rank++;
            }
            FILE *Sorted=NULL;rank=1;
            remove("Highscoressorted.txt");
            Sorted=fopen("Highscoressorted.txt","wb");
            printf("Rank\tScore\n");
            for(i=0;i<10;i++){
                fseek(fp_out,0,SEEK_SET);
                for(int j=0;j<10;j++){
                    offest=sizeof(playerHigh)*j;
                    fseek(fp_out,offest,0);
                    fread(&playerHigh,sizeof(playerHigh),1,fp_out);
                    found=0;
                    if(playerHigh.score==scoreplayer[i]){
                        printf("%d\t%d\n",rank,playerHigh.score);rank++;
                        fwrite(&playerHigh.score,sizeof(playerHigh.score),1,Sorted);
                        break;
                    }
                }
            }
            fclose(Sorted);
            fclose(fp_out);
        }
        else{
            fflush(stdin);
            fwrite(player1.name,sizeof(player1.name),1,fp_out);
            fclose(fp_out);
            fp_out=fopen("High.txt","a+b");
            strcpy(playerHigh.namePlayer,player1.name);playerHigh.score=player1.scoreNow;
            printf("Score:%d",playerHigh.score);
            fwrite(&playerHigh,sizeof(playerHigh),1,fp_out);
            rewind(fp_out);
            while(!feof(fp_out)){
                if(fread(&playerHigh,sizeof(playerHigh),1,fp_out)==0)break;
                scoreplayer[scoresCount]=playerHigh.score;
                scoresCount++;
            }
            for(i=0;i<scoresCount;i++){
                for(j=0;j<(scoresCount-1-i);j++){
                    if(scoreplayer[j]<scoreplayer[j+1]){
                        swap(&scoreplayer[j],&scoreplayer[j+1]);
                    }
                }
            }
            for(i=0;i<scoresCount;i++){
                if(scoreplayer[i]==scoreplayer[i-1])continue;
                if(player1.scoreNow==scoreplayer[i]){
                    printf("\nRank:%d\n",rank);
                    break;
                }
                else rank++;
            }
            FILE *Sorted=NULL;rank=1;
            remove("Highscoressorted.txt");
            Sorted=fopen("Highscoressorted.txt","wb");
            printf("Rank\tScore\n");
            for(i=0;i<10;i++){
                fseek(fp_out,0,SEEK_SET);
                for(int j=0;j<10;j++){
                    offest=sizeof(playerHigh)*j;
                    fseek(fp_out,offest,0);
                    fread(&playerHigh,sizeof(playerHigh),1,fp_out);
                    found=0;
                    if(playerHigh.score==scoreplayer[i]){
                        printf("%d\t%d\n",rank,playerHigh.score);rank++;
                        fwrite(&playerHigh.score,sizeof(playerHigh.score),1,Sorted);
                        break;
                    }
                }
            }
            fclose(Sorted);
            fclose(fp_out);
        }
    }
    else if(player1.scoreNow<player2.scoreNow){
        printf("\nWell Done Player2... You have won the game\nPlease... Enter Your Name:\n");
        fflush(stdin);
        fgets(player2.name,101,stdin);
        allStringLower(player2.name,100);
        while(!feof(fp_in)&&found==0){
            if(fread(name,sizeof(name),1,fp_in)==0)break;
            len=strlen(name);
            if(!memcmp(name,player2.name,len))found=1;
        }
        fclose(fp_in);
        if(found==1){
            fp_out=NULL;
            fp_out=fopen("High.txt","a+b");
            updatePlayerScore(fp_out,player2.name,&player2.scoreNow);
            rewind(fp_out);
            while(!feof(fp_out)){
                if(fread(&playerHigh,sizeof(playerHigh),1,fp_out)==0)break;
                scoreplayer[scoresCount]=playerHigh.score;
                scoresCount++;
            }
            for(i=0;i<scoresCount;i++){
                for(j=0;j<(scoresCount-1-i);j++){
                    if(scoreplayer[j]<scoreplayer[j+1]){
                        swap(&scoreplayer[j],&scoreplayer[j+1]);
                    }
                }
            }
            for(i=0;i<scoresCount;i++){
                if(scoreplayer[i]==scoreplayer[i-1])continue;
                if(player2.scoreNow==scoreplayer[i]){
                    printf("\nRank:%d\n",rank);
                    break;
                }
                else rank++;
            }
            FILE *Sorted=NULL;rank=1;
            remove("Highscoressorted.txt");
            Sorted=fopen("Highscoressorted.txt","wb");
            printf("Rank\tScore\n");
            for(i=0;i<10;i++){
                fseek(fp_out,0,SEEK_SET);
                for(int j=0;j<10;j++){
                    offest=sizeof(playerHigh)*j;
                    fseek(fp_out,offest,0);
                    fread(&playerHigh,sizeof(playerHigh),1,fp_out);
                    found=0;
                    if(playerHigh.score==scoreplayer[i]){
                        printf("%d\t%d\n",rank,playerHigh.score);rank++;
                        fwrite(&playerHigh.score,sizeof(playerHigh.score),1,Sorted);
                        break;
                    }
                }
            }
            fclose(Sorted);
            fclose(fp_out);
        }
        else{
            fflush(stdin);
            fwrite(player2.name,sizeof(player2.name),1,fp_out);
            fclose(fp_out);
            fp_out=fopen("High.txt","a+b");
            strcpy(playerHigh.namePlayer,player2.name);playerHigh.score=player2.scoreNow;
            printf("Score:%d",playerHigh.score);
            fwrite(&playerHigh,sizeof(playerHigh),1,fp_out);
            rewind(fp_out);
            while(!feof(fp_out)){
                if(fread(&playerHigh,sizeof(playerHigh),1,fp_out)==0)break;
                scoreplayer[scoresCount]=playerHigh.score;
                scoresCount++;
            }
            for(i=0;i<scoresCount;i++){
                for(j=0;j<(scoresCount-1-i);j++){
                    if(scoreplayer[j]<scoreplayer[j+1]){
                        swap(&scoreplayer[j],&scoreplayer[j+1]);
                    }
                }
            }
            for(i=0;i<scoresCount;i++){
                if(scoreplayer[i]==scoreplayer[i-1])continue;
                if(player2.scoreNow==scoreplayer[i]){
                    printf("\nRank:%d\n",rank);
                    break;
                }
                else rank++;
            }
            FILE *Sorted=NULL;rank=1;
            remove("Highscoressorted.txt");
            Sorted=fopen("Highscoressorted.txt","wb");
            printf("Rank\tScore\n");
            for(i=0;i<10;i++){
                fseek(fp_out,0,SEEK_SET);
                for(int j=0;j<10;j++){
                    offest=sizeof(playerHigh)*j;
                    fseek(fp_out,offest,0);
                    fread(&playerHigh,sizeof(playerHigh),1,fp_out);
                    found=0;
                    if(playerHigh.score==scoreplayer[i]){
                        printf("%d\t%d\n",rank,playerHigh.score);rank++;
                        fwrite(&playerHigh.score,sizeof(playerHigh.score),1,Sorted);
                        break;
                    }
                }
            }
            fclose(Sorted);
            fclose(fp_out);
        }
    }
    else{
        printf("Hard Luck for both of You... Train Using Computer Mode... It's A Draw\n");
    }
    fclose(fp_out);
    printf("1:Main Menu\nany no:Exit\nSelection: ");
    scanf("%d",&endgame);
    switch(endgame){
        case 1:break;
        default:exit(1);break;
    }
}

int readXml(char xmlFile[],char open[], char close[], char realValue[]){
    char xmlCheckOpen[]="<Configurations>", xmlCheckClose[]="</Configurations>";    //fn for reading xml file
    int finalValue=0,lenopen=strlen(open),lenclose=strlen(close),lenValue,lenfile=strlen(xmlFile),beginread=0,endread=0,i;
    int lenCheckOpen=strlen(xmlCheckOpen),lenCheckClose=strlen(xmlCheckClose),lenXmlFile=strlen(xmlFile);
    if(!(memcmp(xmlCheckOpen,xmlFile,lenCheckOpen))&&!(memcmp(xmlCheckClose,xmlFile+lenXmlFile-lenCheckClose,lenCheckClose))){
        for(i=0;i<lenfile;i++){                                                     //check for configration tab
            if(!memcmp(open,(xmlFile+i),lenopen)){                                  //check for specific tab if configration tab is found
                beginread=i;
                break;
            }
        }
        for(i=0;i<lenfile;i++){
            if(!memcmp(close,(xmlFile+i),lenclose)){
                endread=i;
                break;
            }
        }
    lenValue=endread-beginread-lenopen;

    if(lenValue>0){
        for(i=0;i<lenValue;i++){
            realValue[i]=*(xmlFile+beginread+lenopen+i);
        }                                                                           //reading what is in the tag
        for(i=0;i<lenValue;i++){
            if(realValue[i]>47&&realValue[i]<58){
                finalValue=finalValue*10+(realValue[i]-'0');
        }
        }
        return finalValue;
    }
    else{
        return -1;                                                                  //if some error happen, fn will return -1
    }
    }
    else{
        return -1;
    }
}

void saveAuto(int undo1[], int undo2[],int *undocount1,int *undocount2, dim a,int *mode){
    FILE *newSave=NULL;                                                         //fn for saving the game in a file
    newSave=fopen("Save1.txt","r");
    if(newSave==NULL){
        remove("Save1.txt");
        newSave=fopen("Save1.txt","wb");
        fwrite(mode,sizeof(*mode),1,newSave);
        fwrite(&a,sizeof(a),1,newSave);
        fwrite(undocount1,sizeof(*undocount1),1,newSave);
        fwrite(undocount2,sizeof(*undocount2),1,newSave);
        fwrite(undo1,sizeof(undo1[0])*(*undocount1),1,newSave);
        fwrite(undo2,sizeof(undo2[0])*(*undocount2),1,newSave);

        FILE *saveScores=NULL;
        saveScores=fopen("High.txt","rb");
        if(saveScores==NULL)printf("Scores File is missing\n cannot autoSave");
        while(!feof(saveScores)){
            if(fread(&playerHigh,sizeof(playerHigh),1,saveScores)==0)break;
            fwrite(&playerHigh,sizeof(playerHigh),1,newSave);
        }

        fclose(newSave);
    }
    else{
        newSave=fopen("Save2.txt","r");
        if(newSave==NULL){
            remove("Save2.txt");
            newSave=fopen("Save2.txt","wb");
            fwrite(mode,sizeof(*mode),1,newSave);
            fwrite(&a,sizeof(a),1,newSave);
            fwrite(undocount1,sizeof(*undocount1),1,newSave);
            fwrite(undocount2,sizeof(*undocount2),1,newSave);
            fwrite(undo1,sizeof(undo1[0])*(*undocount1),1,newSave);
            fwrite(undo2,sizeof(undo2[0])*(*undocount2),1,newSave);

            FILE *saveScores=NULL;
            saveScores=fopen("High.txt","rb");
            if(saveScores==NULL)printf("Scores File is missing\n cannot autoSave"),exit(1);
            while(!feof(saveScores)){
                if(fread(&playerHigh,sizeof(playerHigh),1,saveScores)==0)break;
                fwrite(&playerHigh,sizeof(playerHigh),1,newSave);
            }

            fclose(newSave);
        }
        else{
            newSave=fopen("Save3.txt","r");
            if(newSave==NULL){
                remove("Save3.txt");
                newSave=fopen("Save3.txt","wb");
                fwrite(mode,sizeof(*mode),1,newSave);
                fwrite(&a,sizeof(a),1,newSave);
                fwrite(undocount1,sizeof(*undocount1),1,newSave);
                fwrite(undocount2,sizeof(*undocount2),1,newSave);
                fwrite(undo1,sizeof(undo1[0])*(*undocount1),1,newSave);
                fwrite(undo2,sizeof(undo2[0])*(*undocount2),1,newSave);

                FILE *saveScores=NULL;
                saveScores=fopen("High.txt","rb");
                if(saveScores==NULL)printf("Scores File is missing\n cannot autoSave");
                while(!feof(saveScores)){
                if(fread(&playerHigh,sizeof(playerHigh),1,saveScores)==0)break;
                fwrite(&playerHigh,sizeof(playerHigh),1,newSave);
                }

                fclose(newSave);
            }
            else{
                printf("Number of saves is reached");
            }
        }
    }
}

void loadGame(FILE *load){
    int mode,undocount1,undocount2,i;dim a;                                 //fn for loading the game form a file
    fread(&mode,sizeof(mode),1,load);
    fread(&a,sizeof(a),1,load);
    fread(&undocount1,sizeof(undocount1),1,load);
    fread(&undocount2,sizeof(undocount2),1,load);

    int undo1[undocount1+1],undo2[undocount2];
    for(i=0;i<undocount1;i++)fread(undo1+i,sizeof(*undo1),1,load);
    for(i=0;i<undocount2;i++)fread(undo2+i,sizeof(*undo2),1,load);

    FILE *loadScores;
    loadScores=fopen("High.txt","wb");
    while(!feof(load)){
        if(fread(&playerHigh,sizeof(playerHigh),1,load)==0)break;
        fwrite(&playerHigh,sizeof(playerHigh),1,loadScores);
    }
    fclose(load);
    loading(undo1,undo2,&undocount1,&undocount2,a,&mode);
}

void loading(int undo1[], int undo2[],int *undocount1,int *undocount2, dim a,int *mode){
    (*undocount1)--;(*undocount2)--;int move,i;                                 //filling the grid from moves of the saved file
    char grid[a.height][a.width];
    spaceSpread(a.height,a.width,grid);
    if((*mode)==0){
        while((*undocount1)>=0&&(*undocount2)>=0){
            move=undo1[*undocount1];(*undocount1)--;
            for(i=0;(i<a.height)&&(grid[i][move]==32);i++)printf("Working");;i--;
            grid[i][move]=88;
            move=undo2[*undocount2];(*undocount2)--;
            for(i=0;(i<a.height)&&grid[i][move]==32;i++);i--;
            grid[i][move]=79;
        }
        printGrid(grid,a.height,a.width,88,79,4,13);
        HumVsCom(a,grid);
    }
    else if((*mode)==1){
        while((*undocount1)>=0&&(*undocount2)>=0){              //
            move=undo1[*undocount1];(*undocount1)--;
            for(i=0;(i<a.height)&&(grid[i][move]==32);i++)printf("Working");;i--;
            grid[i][move]=88;
            move=undo2[*undocount2];(*undocount2)--;
            for(i=0;(i<a.height)&&grid[i][move]==32;i++);i--;
            grid[i][move]=79;
        }
        printGrid(grid,a.height,a.width,88,79,4,13);
        HumVsHum(a,grid);
    }
}

void assignPara(int *row , int *col, int *hiScores){
    *row=4;*col=4;*hiScores=10;
}

int main(){
    dim para;int len,sel1,sel2,mode=2,i,endgame;
    FILE* fp=NULL;
    char s[3001],par[3001],finalHeight[1001],finlaWidth[1001]={0},finlaHighscores[1001]={0};
    fp =fopen("Configurations.xml","r");
    if(fp!=NULL){ //configration
        while(!feof(fp)){
            fscanf(fp,"%s\n",s);                                    //reading the whole xml file into a string
            len=strlen(s);
            strncat(par,s,len);
        }
        char tagopen1[]="<Height>", tagclose1[]="</Height>";        //specific tags for game parameters
        char tagopen2[]= "<Width>", tagclose2[]="</Width>";
        char tagopen3[]="<Highscores>", tagclose3[]="</Highscores>";
        para.height=readXml(par,tagopen1,tagclose1,finalHeight);
        para.width=readXml(par,tagopen2,tagclose2,finlaWidth);
        para.Highscores=readXml(par,tagopen3,tagclose3,finlaHighscores);
    }
   if(fp==NULL||para.height<=3||para.width<=3||para.Highscores<=3){
        printf("Error... Enter the Path if possible:\n");           //same staff but the file is not beside main.c... then ask user for path 1st time
        fgets(s,3001,stdin);
        s[strlen(s)-1]='\0';
        fp=fopen(s, "r");
        if(fp!=NULL){ //configration
            while(!feof(fp)){
                fscanf(fp,"%s\n",s);
                len=strlen(s);
                strncat(par,s,len);
            }

        char tagopen1[]="<Height>", tagclose1[]="</Height>";
        char tagopen2[]= "<Width>", tagclose2[]="</Width>";
        char tagopen3[]="<Highscores>", tagclose3[]="</Highscores>";
        para.height=readXml(par,tagopen1,tagclose1,finalHeight);
        para.width=readXml(par,tagopen2,tagclose2,finlaWidth);
        para.Highscores=readXml(par,tagopen3,tagclose3,finlaHighscores);
        }
   }
    if(fp==NULL||para.height<=3||para.width<=3|para.Highscores<=3){ //same staff but the file is not beside main.c... then ask user for path 2nd time
        printf("Error... Enter the Path if possible:\n");
        fgets(s,3001,stdin);
        s[strlen(s)-1]='\0';
        fp=fopen(s, "r");
        if(fp!=NULL){ //configration
            while(!feof(fp)){
                fscanf(fp,"%s\n",s);
                len=strlen(s);
                strncat(par,s,len);
            }
        char tagopen1[]="<Height>", tagclose1[]="</Height>";
        char tagopen2[]= "<Width>", tagclose2[]="</Width>";
        char tagopen3[]="<Highscores>", tagclose3[]="</Highscores>";
        para.height=readXml(par,tagopen1,tagclose1,finalHeight);
        para.width=readXml(par,tagopen2,tagclose2,finlaWidth);
        para.Highscores=readXml(par,tagopen3,tagclose3,finlaHighscores);
        printf("%d\n%d\n%d", para.height,para.width,para.Highscores);
        }
    }
    if(fp==NULL||para.height<=3|para.width<=3||para.Highscores<=3){ //same staff but the file is not beside main.c... then ask user for path 3rd time
        printf("Error... Enter the Path if possible:\n");
        fgets(s,3001,stdin);
        s[strlen(s)-1]='\0';
        fp=fopen(s, "r");
        if(fp!=NULL){ //configration
            while(!feof(fp)){
                fscanf(fp,"%s\n",s);
                len=strlen(s);
                strncat(par,s,len);
            }

        char tagopen1[]="<Height>", tagclose1[]="</Height>";
        char tagopen2[]= "<Width>", tagclose2[]="</Width>";
        char tagopen3[]="<Highscores>", tagclose3[]="</Highscores>";
        para.height=readXml(par,tagopen1,tagclose1,finalHeight);
        para.width=readXml(par,tagopen2,tagclose2,finlaWidth);
        para.Highscores=readXml(par,tagopen3,tagclose3,finlaHighscores);
        }
    }

    if(fp==NULL||para.height<=3||para.width<=3|para.Highscores<=3){ //Using default parameters as the file fail to load 3 times in a row
        assignPara(&para.height,&para.width,&para.Highscores);
        printf("Error... Normal parameters will take place\n");
    }
    fclose(fp);

    while(1){
        system("cls");                                              //Main Menu features
        printf("1:Start a new game\n2:Load a game\n3:Top Players\n4:Delete Saves\nany no:Quit\nSelection: ");
        scanf("%d", &sel1);
    if(sel1==1){
        printf("1:Human vs. Human\n2:Human vs. Computer\nSelection: ");
        scanf("%d", &sel2);
        if(sel2==1){
            char grid[para.height][para.width];
            spaceSpread(para.height,para.width,grid);
            HumVsHum(para,grid);
        }
        else if(sel2==2){
            char grid[para.height][para.width];
            spaceSpread(para.height,para.width,grid);
            HumVsCom(para,grid);
        }
        else{

        }
    }

    else if(sel1==3){                                               //print highscores sorted
        FILE *Sorted;
        Sorted=fopen("Highscoressorted.txt","rb");
        printf("Rank\tScore\n");
        for(i=0;i<para.Highscores&&!feof(Sorted);i++){
            if(fread(&endgame,sizeof(endgame),1,Sorted)==0)break;
            printf("%d:\t%d\n",i+1,endgame);
        }
        scanf("%d",&endgame);

        fclose(Sorted);
    }
    else if(sel1==2){
        printf("1:Which Save would you like to load(1 or 2 or 3)\nany no:Exit\nSelection:");
        scanf("%d",&sel1);                                          //loading saved files
        FILE *load;
        switch(sel1){
            case 1:{
                load=fopen("Save1.txt","rb");
                if(load==NULL)printf("Save 1 is missing?!?!"),exit(1);
                loadGame(load);
                break;
            }
            case 2:{
                load=fopen("Save2.txt","rb");
                if(load==NULL)printf("Save 2 is missing?!?!"),exit(1);
                loadGame(load);
                break;
            }
            case 3:{
                load=fopen("Save3.txt","rb");
                if(load==NULL)printf("Save 3 is missing?!?!"),exit(1);
                loadGame(load);
                break;
            }
            default:break;
        }
    }
    else if(sel1==4){                                           //deleting saved files
         printf("1:Which Save would you like to delete(1 or 2 or 3)\nany no:Main Menu\nSelection:");
         scanf("%d",&sel1);
         switch(sel1){
            case 1:remove("Save1.txt");break;
            case 2:remove("Save2.txt");break;
            case 3:remove("Save3.txt");break;
            default:break;
         }
    }
    else exit(1);
    }

    return 0;
}

