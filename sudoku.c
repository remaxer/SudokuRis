#include <stdio.h>
#include <strings.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define SUDOKU_SIZE 9


int sudoku_arr[SUDOKU_SIZE][SUDOKU_SIZE];
int slc_solved9=0;

int checkNumber(int,int,int);
void makeSudoku(void);
void printSudoku(void);
int checkSquare(int);
int checkLine(int);
int checkColumn(int);
int *getLine(int);
int *getColumn(int);
int putNumber(int,int,int);
int checkNumberExistsInSquare(int,int *);
int resolveSudoku(void);
int checkSudokuInteg(void);
int *getSquare(int);
int getColRowSquare(int,char);
int checkSudokuInt(void);
int checkIfSquareIsComplete(int);
int startSudoku(void);
int saveSudokuInFile(int);
int getSudokuFromFile(int);
int checkNumberExistsInColLine(int,int *);


int saveSudokuInFile(int fd){
	int n;
	if(fd<0)
		return(0);
	n=write(fd,sudoku_arr,sizeof(sudoku_arr));
	printf("\n %d Bytes Written \n",n);
	return(1);	
}

int getSudokuFromFile(int fd){
	int n;
	if(fd<0)
		return(0);
	n=read(fd,sudoku_arr,sizeof(sudoku_arr));
	printf("\n %d Bytes Read \n",n);
	return(1);
	

}

int checkIfSquareIsComplete(int square){
	int j,i;
        int column=getColRowSquare(square,'c');
	int row=getColRowSquare(square,'r');
	for(i=0;i<3;i++){
		for(j=0;j<3;j++)
			if(sudoku_arr[row+i][column+j])
				return(0);
	}
	return(1);
}


int checkSudokuInt(void){
	int i,j;
	for(i=1;i<SUDOKU_SIZE+1;i++){
		int *square_p;
		square_p=getSquare(i);
		int checkArr[SUDOKU_SIZE+1];
		for(j=0;j<SUDOKU_SIZE+1;j++)checkArr[j]=0;
		for(j=0;j<SUDOKU_SIZE;j++){
			if(*square_p!=0)
				checkArr[*square_p]+=1;
			square_p++;
		}
		for(j=0;j<SUDOKU_SIZE+1;j++)
			if(checkArr[j]!=0||checkArr[j]>1)
				 return(0);	
	}
	return(1);


}

int startSudoku(void){
int success=1;
while(success){
	success=resolveSudoku();
	}
	return(0);

}

int resolveSudoku(void){
int i;
int slc_solved=0;
for(i=1;i<=SUDOKU_SIZE;i++){
	if(!checkIfSquareIsComplete(i)){
		slc_solved+=checkSquare(i);
	}
		slc_solved+=checkLine(i-1);
		slc_solved+=checkColumn(i-1);

}
if(slc_solved>0){
	printSudoku();
	slc_solved9++;
	return(1);
	}
else
	return(0);

}


int putNumber(int row,int column,int n){
	if(sudoku_arr[row][column]==0){
		sudoku_arr[row][column]=n;
		return(1);
	}
	else
		return(0);

}




int checkNumberExistsInSquare(int n,int *sudoku_square){
	int exists=0,i;
	for(i=0;i<SUDOKU_SIZE;i++){
		if(*sudoku_square==n){
			exists=1;
			break;
		}
		sudoku_square++;
	}
	return(exists);
}


int checkNumberExistsInColLine(int n,int *colline){
	int exists=0,i;
	for(i=0;i<SUDOKU_SIZE;i++){
		if(*colline==n){
			exists=1;
			break;
		}
		colline++;	
	}
	return(exists);

}

int getColRowSquare(int square,char ch){
	int column,row;	
	switch(square){
		case 1:
			column=0;
			row=0;
			break;
		case 2:
			column=3;
			row=0;
			break;
		case 3:
			column=6;
			row=0;
			break;
		case 4: 
			column=0;
			row=3;
			break;
		case 5:
			column=3;
			row=3;
			break;
		case 6:
			column=6;
			row=3;
			break;
		case 7:
			column=0;
			row=6;
			break;
		case 8:
			column=3;
			row=6;
			break;
		case 9:
			column=6;
			row=6;
			break;
	}
	switch(ch){
		case 'r':
			return(row);
			break;
		case 'c':
			return(column);
			break;
		default:
			return(row);
			break;
	}

}


int *getSquare(int square){
        int column=getColRowSquare(square,'c');
	int row=getColRowSquare(square,'r');
	int i,j;
	int sudoku_square[3][3];
	int k=0,p=0;
	for(i=row,p=0;i<row+3;i++,p++){
		for(j=column,k=0;j<column+3;j++,k++){
			sudoku_square[p][k]=sudoku_arr[i][j];	
		}
		
	}
	return(sudoku_square);
}

int *getLine(int line){
	int line_arr[SUDOKU_SIZE],i;
	for(i=0;i<SUDOKU_SIZE;i++){
		line_arr[i]=sudoku_arr[line][i];
	}
	return(line_arr);
}

int *getColumn(int col){
	int col_arr[SUDOKU_SIZE],i;
	for(i=0;i<SUDOKU_SIZE;i++){
		col_arr[i]=sudoku_arr[i][col];
	}
	return(col_arr);
}

int checkColumn(int col){
	int *col_p,i,k;
	col_p=getColumn(col);
	int numbersIns=0;
	for(k=1;k<=SUDOKU_SIZE;k++){
		int poss=0;
		int possrow;
		if(checkNumberExistsInColLine(k,col_p++))
			continue;
		for(i=0;i<SUDOKU_SIZE;i++){
			if(sudoku_arr[i][col]==0){
				if(checkNumber(i,col,k)){
					poss++;
					possrow=i;
				}

			}
		}
		if(poss==1){
			putNumber(possrow,col,k);
			numbersIns++;

		}
	}
	if(numbersIns>0)
		return(1);
	else
		return(0);
}


int checkLine(int line){
	int *line_p,i,k;
	line_p=getLine(line);
	int numbersIns=0;
	for(k=1;k<=SUDOKU_SIZE;k++){
		int poss=0;
		int posscol;
		if(checkNumberExistsInColLine(k,line_p++))
			continue;
		for(i=0;i<SUDOKU_SIZE;i++){
			if(sudoku_arr[line][i]==0){
				if(checkNumber(line,i,k)){
					poss++;
					posscol=i;
				}

			}
		}
		if(poss==1){
			putNumber(line,posscol,k);
			numbersIns++;

		}
	}
	if(numbersIns>0)
		return(1);
	else
		return(0);
}



int checkSquare(int square){
	int sudoku_square[3][3];
	int *sudoku_square_p;
	sudoku_square_p=getSquare(square);
	int numbersIns=0;
	int k,i,j;
	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
			sudoku_square[i][j]=*sudoku_square_p++;
	
	int column=getColRowSquare(square,'c');
	int row=getColRowSquare(square,'r');
	for(k=1;k<10;k++){
		int poss=0;
		int possrow;
		int posscol;
		if(checkNumberExistsInSquare(k,sudoku_square))
			continue;
		for(i=0;i<3;i++){
			for(j=0;j<3;j++){
				if(sudoku_square[i][j]==0){
						if(checkNumber(row+i,column+j,k)){
							possrow=row+i;
							posscol=column+j;
							poss++;
						}		
				}
			}
		}
		if(poss==1){
			putNumber(possrow,posscol,k);
			numbersIns++;
			}			
		}
	if(numbersIns>0)
		return(1);
	else
		return(0);
	
}

void makeSudoku(void){
	int ind,success,fd;
	fd=open("oldsudoku.dat",O_RDWR|O_CREAT,0600);
	ind=lseek(fd,0L,2);
	char ch='n';
	if(ind>0){
		printf("Do you wanna remake old sudoku?(S/n)");
		scanf("%c",&ch);
	}
		lseek(fd,0L,0);
	if(ch=='s'||ch=='S'){
		success=getSudokuFromFile(fd);
	}
	else{
	int i,j;
	for(i=0;i<SUDOKU_SIZE;i++){
		for(j=0;j<SUDOKU_SIZE;j++){
		int it;
		do{
	printf("\nType number (0 for empty) : row %d column %d  :",i+1,j+1);
        scanf("%d",&it);
		}while(it<0||it>9);
		sudoku_arr[i][j]=it;
	}
	}
		success=saveSudokuInFile(fd);
	}
	if(!success)
		printf("There was an error reading or writing file\n");
	close(fd);
}




int checkNumber(int row,int column,int n){
	//If there's already a number it'll return  0
   	if(sudoku_arr[row][column]!=0)	
		return(0);
	int success,i,j;
	success=1;
	//Columns
	for(i=0;i<SUDOKU_SIZE;i++){
		if(sudoku_arr[row][i]==n&&i!=column){
			success=0;
			break;
		}
	}
	//Rows
	if(success){
		for(i=0;i<SUDOKU_SIZE;i++){
			if(sudoku_arr[i][column]==n&&i!=row){
				success=0;
				break;
			}
		}
	}
	return(success);

}



void printSudoku(void){
	int i,j;
	for(i=0;i<SUDOKU_SIZE;i++){
		for(j=0;j<SUDOKU_SIZE;j++){
				if(sudoku_arr[i][j]==0)
					printf("  ");
				else
					printf(" %d",sudoku_arr[i][j]);
				if(j==2||j==5||j==8){
					printf("|");
					if(j==8)
					if(i==2||i==5||i==8)
						printf("\n");
				}
		}
		printf("\n");
	}
	printf("\n\n\n");
}



int main(){
	makeSudoku();
	printSudoku();
	char ent;
	scanf("%c",&ent);	
	printf("Click to resolve sudoku ");
	scanf("%c",&ent);
	startSudoku();
	if(slc_solved9>0){
		printf("\n--- Sudoku solved ---\n");
	}
	else{
		printf("\n--- Sudoku is impossible --- \n");
	}
	printSudoku();
}









