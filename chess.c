#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

_Bool enableColor = 0;

int playSide = 1;
char chessBoard[8][8] = { 0 };
_Bool ooPossible[2];
_Bool oooPossible[2];

char buffer[80];

int i = 0, j = 0;
char chessBoard2[8][8];

int main(int argc, char *argv[]) {
  if (argc > 2) {
    printf("usage: chess [c]");
  }

  if (argc == 2) {
    if (strchr(argv[1], 'c')) {
      enableColor = 1;
    }
  }

init_cless:
  memset(chessBoard, '.', 64);
  memcpy(chessBoard[0], "rnbqkbnr", 8);
  memcpy(chessBoard[1], "pppppppp", 8);
  memcpy(chessBoard[6], "PPPPPPPP", 8);
  memcpy(chessBoard[7], "RNBQKBNR", 8);

  playSide = 1;
  memset(ooPossible, 1, 2);
  memset(oooPossible, 1, 2);

print_chess_board:
  printf("  ABCDEFGH\n +--------+\n");
  for (i = 0; i < 8; i++) {
    printf("%d|", 8 - i);
    for (j = 0; j < 8; j++) {
      if (enableColor) {
        if (i % 2 == j % 2) {
          if (isupper(chessBoard[i][j])) {
            printf("\033[97;101m%c\033[0m", chessBoard[i][j]);
          } else if (islower(chessBoard[i][j])) {
            printf("\033[97;104m%c\033[0m", chessBoard[i][j]);
          } else {
            printf("\033[47m \033[0m");
          }
        } else {
          if (isupper(chessBoard[i][j])) {
            printf("\033[97;41m%c\033[0m", chessBoard[i][j]);
          } else if (islower(chessBoard[i][j])) {
            printf("\033[97;44m%c\033[0m", chessBoard[i][j]);
          } else {
            printf("\033[40m \033[0m");
          }
        }
      } else {
        putchar(chessBoard[i][j]);
      }
    }
    printf("|%d\n", 8 - i);
  }

  printf(" +--------+\n  ABCDEFGH\n\n");

user_input:
  printf("%s> ", playSide ? "white" : "black");
  fflush(stdout);

  fgets(buffer, 79, stdin);

parse_user_input:
  if (strlen(buffer) != 6) {
    goto invalid_user_input;
  }

  return 0;

invalid_user_input:
  printf("  - invalid input.\n");
  printf("    expected: [source],[dest]\n");
  printf("    example:  e2,e4\n");
  return 0;  
}

