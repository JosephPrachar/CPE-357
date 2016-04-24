#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define FALSE 1
#define TRUE 0

#define MAX_LINE_LENGTH 50
#define WIDTH 20
#define HEIGHT 20
#define MINE 255
#define UNEXPLORED 254

void playGame(void);
void printBoard(int);
char countSur(char row, char col);
void floodFill(char row, char col);
int  validPos(char row, char col);
int  inputPos(char *str);
void clearBoard(void);
void inputMines(void);
int  hasWon(void);

char board[HEIGHT][WIDTH];
char lastPos[2];

int main (void) {
  clearBoard();
  inputMines();
  playGame();
  exit(EXIT_SUCCESS);
}

void playGame(void) {
  char hitMine = FALSE;
  char buf[MAX_LINE_LENGTH];
  while (!hitMine && !hasWon()) {
    printBoard(FALSE);
    fgets(buf, MAX_LINE_LENGTH, stdin);
    if (buf == NULL || !inputPos(buf)) {
      printf("(%d, %d) is not a valid position.", lastPos[0], lastPos[1]);
      continue;
    }

    if (board[lastPos[0]][lastPos[1]] == MINE) {
      hitMine = TRUE;
    } else if (board[lastPos[0]][lastPos[1]] == UNEXPLORED) {
      char numSur = countSur(lastPos[0], lastPos[1]);
      board[lastPos[0]][lastPos[1]] = numSur;
      if (numSur == 0)
	floodFill(lastPos[0], lastPos[1]);
    } else {
      printf("(%d, %d) has already been revealed", lastPos[0], lastPos[1]);
    }
  }

  printBoard(TRUE);

  if (hitMine) {
    printf("You Lost!");
  } else {
    printf("You Won!");
  }
}

void printBoard(int displayMines) {
  for (char row = 0; row < HEIGHT; row++) {
    for (char col = 0; col < WIDTH; col++) {
      if (board[row][col] == MINE) {
	if (displayMines)
	  printf("X");
	else
	  printf("*");
      } else if (board[row][col] == UNEXPLORED) {
	printf("*");
      } else {
	printf("%d", board[row][col]);
      }

      if (col == WIDTH - 1)
	printf("\n");
      else
	printf(" ");
    }
  }
}

void floodFill(char row, char col) {
  board[row][col] = countSur(row, col);

  if (board[row][col] != 0)
    return;

  for (short i = -1; i <= 1; i++)
    for (short j = -1; j <= 1; j++)
      if (validPos(i + row, j + col) && i != 0 && j != 0)
	floodFill(i + row, j + col);
}
char countSur(char row, char col) {
  char count = 0;

  for (short i = -1; i <= 1; i++)
    for (short j = -1; j <= 1; j++)
      if (validPos(i + row, j + col) && board[i + row][j + col] == MINE)
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
    char *nums = strtok(str, ",");
    if (nums == NULL)
      return FALSE;

    lastPos[0] = atoi(nums);
    nums = strtok(NULL, ",");
    if (nums == NULL)
      return FALSE;

    lastPos[1] = atoi(nums);
    if (lastPos[0] < 0 || lastPos[0] >= HEIGHT ||
	lastPos[1] < 0 || lastPos[1] >= WIDTH)
      return FALSE;

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
  scanf("%d", &mineCount);

  if (mineCount < 0) {
    printf("Number of mines cannot be negative.");
    exit(EXIT_FAILURE);
  }
  
  char i = 0;
  char buf[MAX_LINE_LENGTH];
  for (; i < mineCount && fgets(buf, MAX_LINE_LENGTH, stdin) != NULL; i++) {
    if (!inputPos(buf) || board[lastPos[0]][lastPos[1]] == MINE) {
      printf("(%d, %d) is not a valid position for a mine.",
	     lastPos[0], lastPos[1]);
    } else {
      board[lastPos[0]][lastPos[1]] = MINE;
    }
  }

  if (i != mineCount) {
    printf("Not enough mines placed on the board.");
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
