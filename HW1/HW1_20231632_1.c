#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>

#define KEY_SPACE ' '
#define KEY_Enter 10
#define START_ROW 5
#define START_COL 5

int h, w;
  

void saveGame(int **taqta, int n, int row, int col, int turn, char *fname){

	FILE *ifp;
	int i,j;
	ifp = fopen(fname,"w");
	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			fprintf(ifp,"%c", taqta[i][j]);
		}
	}
	fprintf(ifp, " ");
	fprintf(ifp,"%d %d %d %d %d %d",row,col,turn, n, h,w);
	fclose(ifp);
	
}

int** readSavedGame(int **taqta, int *n, int *col, int *row,  int *turn, char *fname){
	
	FILE *ofp=NULL;
	int i,j;
	int res=0;
	int dx=0;
	char tmp[400];
	
	ofp = fopen(fname,"r");
	res = fscanf(ofp,"%s %d %d %d %d %d %d",tmp, row, col, turn, n, &h, &w);
	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			if(tmp[dx]=='O'||tmp[dx]=='X'||tmp[dx]=='Y')
				taqta[i][j]=tmp[dx];
			dx++;
		}
	}
	fclose(ofp);

	return taqta;
	
}

void readSavedHW(char *fname){
	FILE *ofp;
	char tmp[400];
	int tmp1,tmp2,tmp3,tmp4;
	int res;
	ofp = fopen(fname,"r");
	res = fscanf(ofp,"%s %d %d %d %d %d %d",tmp,&tmp1,&tmp2,&tmp3,&tmp4,&h,&w);
	fclose(ofp);
}

int** initBoard(int **taqta, int *row, int *col, int *turn, int load){
	taqta = (int**)malloc(sizeof(int*)*h);
	for(int i=0; i<h; i++){
		taqta[i] = (int*)malloc(sizeof(int)*w);
	}

	printf("%d %d\n", h, w);
        taqta[0][0] = ACS_ULCORNER;
	for (int i=1; i < w-1; i++)
		taqta[0][i] = ACS_TTEE;
	taqta[0][w-1] = ACS_URCORNER; 

	for (int i=1; i<h-1; i++){
		taqta[i][0] = ACS_LTEE; 
		for (int j=1; j < w-1; j++)
			taqta[i][j] = ACS_PLUS; 
		taqta[i][w-1] = ACS_RTEE; 
	}
    taqta[h-1][0] = ACS_LLCORNER; 
	for (int i=1; i < w-1; i++)
		taqta[h-1][i] = ACS_BTEE; 
	taqta[h-1][w-1] = ACS_LRCORNER; 

	return taqta;
}

void paintBoard(int **taqta, WINDOW *win, int row, int col){

	wmove(win,0,0);
	int i,j;
	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			waddch(win,taqta[i][j]);
		}
	}
	wmove(win,row,col);

}

void paintMenu(int n, int turn, char cturn, WINDOW *win, int row, int col){

	if(n==2){
	if(turn==1){
		cturn='O';
		turn=2;
	}
	else if(turn==2){
		cturn='X';
		turn=1;
	}
	}
	else if(n==3){
	if(turn==1){
		cturn='O';
		turn=2;
	}
	else if(turn==2){
		cturn='X';
		turn=3;
	}
	else if(turn==3){
		cturn='Y';
		turn=1;
	}
	}
	mvprintw(h+10,6,"Current Turn : %c",cturn);
	mvprintw(h+11,6,"1. Press 1 to save");
	mvprintw(h+12,6,"2. Exit without save");
	wmove(win,row,col);
}

int checkWin(int **taqta, int turn,int n){ 

	int i,j;
	if(n==2){
	for(i=0; i<h; i++){
		for(j=2; j<w-2; j++){
			if(
			taqta[i][j-2]=='O'&&
			taqta[i][j-1]=='O'&&
			taqta[i][j]=='O'&&
			taqta[i][j+1]=='O'&&
			taqta[i][j+2]=='O'
			){
				mvprintw(h+9,6,"PLAYER%d WIN !! Press any button two times to terminate the program",turn-1);
				return 1;
			}

			else if(
			taqta[i][j-2]=='X'&&
			taqta[i][j-1]=='X'&&
			taqta[i][j]=='X'&&
			taqta[i][j+1]=='X'&&
			taqta[i][j+2]=='X'
			){
				mvprintw(h+9,6,"PLAYER%d WIN !! Press any button two times to terminate the program",turn+1);
				return 1;
			}
		}
	}

	for(j=0;j<w;j++){
		for(i=2;i<h-2;i++){
			if(
			taqta[i-2][j]=='O'&&
			taqta[i-1][j]=='O'&&
			taqta[i][j]=='O'&&
			taqta[i+1][j]=='O'&&
			taqta[i+2][j]=='O'
			){
				mvprintw(h+9,6,"PLAYER%d WIN !! Press any button two times to terminate the program",turn-1);
				return 1;
			}

			else if(
			taqta[i-2][j]=='X'&&
			taqta[i-1][j]=='X'&&
			taqta[i][j]=='X'&&
			taqta[i+1][j]=='X'&&
			taqta[i+2][j]=='X'
			){
				mvprintw(h+9,6,"PLAYER%d WIN !! Press any button two times to terminate the program",turn+1);
				return 1;
			}
		}
	}

	for(i=2;i<h-2;i++){
		for(j=2;j<w-2;j++){
			if(
			taqta[i-2][j-2]=='O'&&
			taqta[i-1][j-1]=='O'&&
			taqta[i][j]=='O'&&
			taqta[i+1][j+1]=='O'&&
			taqta[i+2][j+2]=='O'
			){
				mvprintw(h+9,6,"PLAYER%d WIN !! Press any button two times to terminate the program",turn-1);
				return 1;
			}

			else if(
			taqta[i-2][j-2]=='X'&&
			taqta[i-1][j-1]=='X'&&
			taqta[i][j]=='X'&&
			taqta[i+1][j+1]=='X'&&
			taqta[i+2][j+2]=='X'
			){
				mvprintw(h+9,6,"PLAYER%d WIN !! Press any button two times to terminate the program",turn+1);
				return 1;
			}
			else if(
			taqta[i-2][j+2]=='O'&&
			taqta[i-1][j+1]=='O'&&
			taqta[i][j]=='O'&&
			taqta[i+1][j-1]=='O'&&
			taqta[i+2][j-2]=='O'
			){
				mvprintw(h+9,6,"PLAYER%d WIN !! Press any button two times to terminate the program",turn-1);
				return 1;
			}

			else if(
			taqta[i-2][j+2]=='X'&&
			taqta[i-1][j+1]=='X'&&
			taqta[i][j]=='X'&&
			taqta[i+1][j-1]=='X'&&
			taqta[i+2][j-2]=='X'
			){
				mvprintw(h+9,6,"PLAYER%d WIN !! Press any button two times to terminate the program",turn+1);
				return 1;
			}

		}
	}
	}

	if(n==3){
	for(i=0;i<h;i++){
		for(j=0;j<w-3;j++){
			if(
			taqta[i][j]=='O'&&
			taqta[i][j+1]=='O'&&
			taqta[i][j+2]=='O'&&
			taqta[i][j+3]=='O'
			){
				mvprintw(h+9,6,"PLAYER%d WIN !! Press any button two times to terminate the program",turn-1);
				return 1;
			}

			else if(
			taqta[i][j]=='X'&&
			taqta[i][j+1]=='X'&&
			taqta[i][j+2]=='X'&&
			taqta[i][j+3]=='X'
			){
				mvprintw(h+9,6,"PLAYER%d WIN !! Press any button two times to terminate the program",turn-1);
				return 1;
			}
			else if(
			taqta[i][j]=='Y'&&
			taqta[i][j+1]=='Y'&&
			taqta[i][j+2]=='Y'&&
			taqta[i][j+3]=='Y'
			){
				mvprintw(h+9,6,"PLAYER%d WIN !! Press any button two times to terminate the program",turn+2);
				return 1;
			}

		}
	}
	
	for(j=0;j<w;j++){
		for(i=0;i<h-3;i++){
			if(
			taqta[i][j]=='O'&&
			taqta[i+1][j]=='O'&&
			taqta[i+2][j]=='O'&&
			taqta[i+3][j]=='O'
			){
				mvprintw(h+9,6,"PLAYER%d WIN !! Press any button two times to terminate the program",turn-1);
				return 1;
			}

			else if(
			taqta[i][j]=='X'&&
			taqta[i+1][j]=='X'&&
			taqta[i+2][j]=='X'&&
			taqta[i+3][j]=='X'
			){
				mvprintw(h+9,6,"PLAYER%d WIN !! Press any button two times to terminate the program",turn-1);
				return 1;
			}
			else if(
			taqta[i][j]=='Y'&&
			taqta[i+1][j]=='Y'&&
			taqta[i+2][j]=='Y'&&
			taqta[i+3][j]=='Y'
			){
				mvprintw(h+9,6,"PLAYER%d WIN !! Press any button two times to terminate the program",turn+2);
				return 1;
			}

		}
	}
	
	for(j=0;j<w-3;j++){
		for(i=0;i<h-3;i++){
			if(
			taqta[i][j]=='O'&&
			taqta[i+1][j+1]=='O'&&
			taqta[i+2][j+2]=='O'&&
			taqta[i+3][j+3]=='O'
			){
				mvprintw(h+9,6,"PLAYER%d WIN !! Press any button two times to terminate the program",turn-1);
				return 1;
			}

			else if(
			taqta[i][j]=='X'&&
			taqta[i+1][j+1]=='X'&&
			taqta[i+2][j+2]=='X'&&
			taqta[i+3][j+3]=='X'
			){
				mvprintw(h+9,6,"PLAYER%d WIN !! Press any button two times to terminate the program",turn-1);
				return 1;
			}
			else if(
			taqta[i][j]=='Y'&&
			taqta[i+1][j+1]=='Y'&&
			taqta[i+2][j+2]=='Y'&&
			taqta[i+3][j+3]=='Y'
			){
				mvprintw(h+9,6,"PLAYER%d WIN !! Press any button two times to terminate the program",turn+2);
				return 1;
			}
		}
	}

	for(i=2;i<h;i++){
		for(j=1;j<w;j++){
			if(
			taqta[i-2][j+2]=='O'&&
			taqta[i-1][j+1]=='O'&&
			taqta[i][j]=='O'&&
			taqta[i+1][j-1]=='O'
			){
				mvprintw(h+9,6,"PLAYER%d WIN !! Press any button two times to terminate the program",turn-1);
				return 1;
			}

			else if(
			taqta[i-2][j+2]=='X'&&
			taqta[i-1][j+1]=='X'&&
			taqta[i][j]=='X'&&
			taqta[i+1][j-1]=='X'
			){
				mvprintw(h+9,6,"PLAYER%d WIN !! Press any button two times to terminate the program",turn-1);
				return 1;
			}
			else if(
			taqta[i-2][j+2]=='Y'&&
			taqta[i-1][j+1]=='Y'&&
			taqta[i][j]=='Y'&&
			taqta[i+1][j-1]=='Y'
			){
				mvprintw(h+9,6,"PLAYER%d WIN !! Press any button two times to terminate the program",turn+2);
				return 1;
			}
		}
	}

	}
	return 0;
	
}

int Action(WINDOW *win, int **taqta, int keyin, int *row, int *col, int *turn, int n){

	char *fname;
	char file[100];
    fname = file;
	
	int check,tmp;
	
	switch(keyin){

		case KEY_RIGHT : *col += 1;
				wmove(win,*row,*col);
				if(*col>=w) *col-=1; 
				break;

		case KEY_LEFT : *col -= 1;
				wmove(win,*row,*col);
				if(*col<0) *col+=1;
				break;
				
		case KEY_UP : *row -= 1;
			    wmove(win,*row,*col);
			    if(*row<0) *row+=1;
			    break;

		case KEY_DOWN: *row += 1;
			    wmove(win,*row,*col);
			    if(*row>=h) *row-=1;
			    break;
		case KEY_SPACE:
		case KEY_Enter:
			       wmove(win,*row,*col);
			       if(n==2){
			       if(taqta[*row][*col]!='O' && taqta[*row][*col]!='X'){
			       		if (*turn==1){
				       		taqta[*row][*col]='O';
				       		*turn=2;
			       		}
			       		else if(*turn==2){
					        taqta[*row][*col]='X';
				       		*turn=1;
			       		}
					else *turn=*turn;
					mvprintw(h+7,6,"\t\t\t\t\t\t\t");
			       }
			       else if(taqta[*row][*col]=='O'||taqta[*row][*col]=='X'){
				      		mvprintw(h+7,6,"STONE ALREADY THERE");
						break;
			       }
			       }

			       else if(n==3){
			       if(taqta[*row][*col]!='O' && taqta[*row][*col]!='X' && taqta[*row][*col]!='Y'){
			       		if (*turn==1){
				       		taqta[*row][*col]='O';
				       		*turn=2;
			       		}
			       		else if(*turn==2){
					        taqta[*row][*col]='X';
				       		*turn=3;
			       		}
					else if(*turn==3){
						taqta[*row][*col]='Y';
						*turn=1;
					}
					else *turn=*turn;
					mvprintw(h+7,6,"\t\t\t\t\t\t\t");
			       }
			       else if(taqta[*row][*col]=='O'||taqta[*row][*col]=='X'||taqta[*row][*col]=='Y'){
				      		mvprintw(h+7,6,"STONE ALREADY THERE");
				       		wmove(win,*row,*col);
				       		break;
			       }
			       }
			       
			       refresh();
			       wrefresh(win);
			       
			       check = checkWin(taqta,*turn, n);
			       refresh();
			       wrefresh(win);
			       break;
		case '1':
			       mvprintw(h+7,6,"ENTER FILE NAME : ");
			       echo();
			       scanw("%s",fname);
			       noecho();
			       mvprintw(h+7,6,"\t\t\t\t\t\t\t");
			       saveGame(taqta, n, *row, *col, *turn, fname);
			       break;
			       
			       
		case '2': 
			       return 2;
			       break;

	}
	return check;
 
}


void gameStart(WINDOW *win, int load, int n){
	int **taqta;
	int row = 0;
	int col = 0;
	int keyin;
	wmove(win, row, col); 
	int turn=1;
	char cturn;
	int tmp,check;

	taqta = initBoard(taqta, &row, &col, &turn, load); 
	if(load =='n'){
		row=0;
		col=0;
		turn = 1;
		taqta = initBoard(taqta, &row, &col, &turn, load);
	}

	while(1){
		
		paintBoard(taqta,win,row,col); 
		refresh();
		wrefresh(win);
		paintMenu(n,turn,cturn,win,row,col);
		refresh();
		wrefresh(win);
		keyin = getch();
		
		checkWin(taqta, turn, n);
		refresh();
		wrefresh(win);
		
		if(check == 1){
			tmp=getch();
			break;
		}
		
		
		check = Action(win, taqta, keyin, &row, &col, &turn, n);
		
		if(check == 2){
			mvprintw(h+8,6,"Are you sure to exit the game?[y/n]");
			tmp=getch();
			if(tmp=='y') break;
			else if(tmp=='n'){
				mvprintw(h+8,6,"\t\t\t\t\t\t");
			}
		}

		refresh();
		wrefresh(win);
		  
	}

	return;
}

int main(){
	char load;
	int n;
	
	char *fname;
	char file[100];

	int **taqta, row, col, turn;

	fname = file;
	
	printf("Want to load the game?[y/n]: ");
	scanf("%c",&load);

	if(load=='n'){
		printf("Enter the HEIGHT of the board : ");
		scanf("%d",&h);
		printf("Enter the WIDTH of the board : ");
		scanf("%d",&w);
		printf("Enter the number of players[2/3] : ");
		scanf("%d",&n);
	}
	initscr();
	noecho();
	if(load=='y'){
		printw("PLEASE ENTER THE FILE NAME : ");
		echo();
		getstr(file);
		noecho();
		mvprintw(0,0oplkllklllllklllkllllklll,"\t\t\t\t\t\t");
		refresh();
		readSavedHW(fname);
		taqta = initBoard(taqta, &row, &col, &turn, load);	
		taqta = readSavedGame(taqta, &n, &row, &col, &turn, fname);
	}
	
	WINDOW *win=newwin(h,w,5,5);

	keypad(stdscr,TRUE);
	keypad(win,TRUE); 


	gameStart(win,load,n);

	endwin();
	return 0;
}
