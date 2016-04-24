#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define FALSE 0
#define TRUE 1

#define INPLAY 0
#define EXPLODED 1
#define END_OF_INPUT 2

#define MAX_LINE_LENGTH 50
#define WIDTH 20
#define HEIGHT 20
#define MINE 9
#define HITMINE 10
#define UNEXPLORED 11

void playGame(void);
void printBoard(int);
char countSur(char row, char col);
void floodFill(char row, char col);
int  validPos(char row, char col);
int  inputPos(char *str);
void clearBoard(void);
void inputMines(void);
int  hasWon(void);

char buf[MAX_LINE_LENGTH];
char board[HEIGHT][WIDTH];
char lastPos[2];

int main (void) {
  clearBoard();
  inputMines();
  playGame();
  exit(EXIT_SUCCESS);
}

void playGame(void) {
  char end = INPLAY;
  char buf[MAX_LINE_LENGTH];
  int count = 0;
  printf("Initial Board: \n");
  printBoard(FALSE);
  
  while (end == INPLAY && !hasWon()) {    
    if (fgets(buf, MAX_LINE_LENGTH, stdin) == NULL) {
      end = END_OF_INPUT;
      continue;
    } else if (!inputPos(buf)) {
      printf("(%d, %d) is not a valid position.\n", lastPos[0], lastPos[1]);
    } else if (board[lastPos[0]][lastPos[1]] == MINE) {
      end = EXPLODED;
      board[lastPos[0]][lastPos[1]] = HITMINE;
    } else if (board[lastPos[0]][lastPos[1]] == UNEXPLORED) {
      char numSur = countSur(lastPos[0], lastPos[1]);
      board[lastPos[0]][lastPos[1]] = numSur;
      if (numSur == 0)
	floodFill(lastPos[0], lastPos[1]);
    } else {
      printf("Spot already revealed, pick another spot\n");
    }

    printf("Round: %d\n", count);
    printBoard(FALSE);
    count++;
  }

  if (end == EXPLODED) {
    printf("You Lose...\n");
  } else if (end == INPLAY) {
    printf("You Win!!!!\n");
  } else if (end == END_OF_INPUT) {
    printf("No more input. Game Over.\n");
  }
  printBoard(TRUE);
}

void printBoard(int displayMines) {
  for (char row = 0; row < HEIGHT; row++) {
    for (char col = 0; col < WIDTH; col++) {
      if (board[row][col] == MINE) {
	if (displayMines)
	  printf("X ");
	else
	  printf("* ");
      } else if (board[row][col] == HITMINE) {
	printf("X ");
      } else if (board[row][col] == UNEXPLORED) {
	if (displayMines)
	  printf("%d ", countSur(row, col));
	else
	  printf("* ");
      } else {
	printf("%d ", board[row][col]);
      }

      if (col == WIDTH - 1)
	printf("\n");
    }
  }
}

void floodFill(char row, char col) {
  board[row][col] = countSur(row, col);

  if (board[row][col] != 0)
    return;

  for (short i = -1; i <= 1; i++)
    for (short j = -1; j <= 1; j++)
      if (validPos(i + row, j + col) && !(i == 0 && j == 0) &&
	  board[i + row][j + col] == UNEXPLORED)
	floodFill(i + row, j + col);
}
char countSur(char row, char col) {
  char count = 0;

  for (short i = -1; i <= 1; i++)
    for (short j = -1; j <= 1; j++)
      if (validPos(i + row, j + col))
	if (board[i + row][j + col] == MINE || board[i + row][j + col] == HITMINE)
	  count++;

  return count;
}

int validPos(char row, char col) {
  if (row >= 0 && row < HEIGHT && col >= 0 && col < WIDTH)
    return TRUE;
  else
    return FALSE;
}

int inputPos(char *str) {
    char *nums = strtok(str, " ");
    if (nums == NULL) {
      return FALSE;
    }
    lastPos[0] = atoi(nums);
    nums = strtok(NULL, " ");
    if (nums == NULL) {
      return FALSE;
    }
    lastPos[1] = atoi(nums);
    if (lastPos[0] < 0 || lastPos[0] >= HEIGHT ||
	lastPos[1] < 0 || lastPos[1] >= WIDTH) {
      return FALSE;
    }
    return TRUE;
}

void clearBoard(void) {
  for (char row = 0; row < HEIGHT; row++) {
    for (char col = 0; col < WIDTH; col++) {
      board[row][col] = UNEXPLORED;
    }
  }
}

void inputMines(void) {
  int mineCount = 0;
  fgets(buf, MAX_LINE_LENGTH, stdin);
  mineCount = atoi(buf);
  if (mineCount < 0) {
    printf("Number of mines cannot be negative.\n");
    exit(EXIT_FAILURE);
  }
  
  char i = 0;
  for (; i < mineCount && fgets(buf, MAX_LINE_LENGTH, stdin) != NULL; i++) {
    if (inputPos(buf) && board[lastPos[0]][lastPos[1]] != MINE) {
      board[lastPos[0]][lastPos[1]] = MINE;
    } else {
      printf("(%d, %d) is not a valid position for a mine.\n",
	     lastPos[0], lastPos[1]);
      i--;
    }
  }

  if (i != mineCount) {
    printf("Not enough mines placed on the board.\n");
    exit(EXIT_FAILURE);
  }
}

int hasWon(void) {
  for (char row = 0; row < HEIGHT; row++)
    for (char col = 0; col < WIDTH; col++)
      if (board[row][col] == UNEXPLORED)
	return FALSE;

  return TRUE;
}
