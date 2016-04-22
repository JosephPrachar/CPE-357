#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINE_LENGTH 50
#define WIDTH 20
#define HEIGHT 20
#define MINE 255
#define UNEXPLORED 254

void clearBoard(void);
void inputMines(void);

char board[HEIGHT][WIDTH];

int main (void) {
  printf("Hello, World\n");
  exit(EXIT_SUCCESS);
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
  
  char i, row, col = 0;
  char buf[MAX_LINE_LENGTH];
  char *nums;
  for (; i < mineCount && fgets(buf, MAX_LINE_LENGTH, stdin) != NULL; i++) {
    nums = strtok(buf, ",");
    if (nums == NULL) {
      printf("Problem parsing line");
      exit(EXIT_FAILURE);
    }
    row = atoi(nums);
    nums = strtok(NULL, ",");
    if (nums == NULL) {
      printf("Problem parsing line");
      exit(EXIT_FAILURE);
    }
    col = atoi(nums);
    if (row < 0 || row >= HEIGHT || col < 0 || col >= WIDTH) {
      printf("Invalid col or row");
      exit(EXIT_FAILURE);
    }
    board[row][col] = MINE;
  }

  if (i != mineCount) {
    printf("Not enough mines placed on the board.");
    exit(EXIT_FAILURE);
  }
}
