#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *whitePieces = "RNBQKP";
const char *blackPieces = "rnbqkp";
const char *horizontal = "ABCDEFGH";

_Bool enableColor = 0;

int playSide = 1;
char chessBoard[8][8] = { 0 };
_Bool ooPossible[2];
_Bool oooPossible[2];

char buffer[80];

int i = 0, j = 0;
char chessBoard2[8][8];
int srcX, srcY, dstX, dstY;
int srcY1, dstY1;
int dx, dy;
char srcPiece, dstPiece;

char *potentialKingRookSwitch;

int tx, ty;
int check_return_position;
_Bool hasAttack;
int atkX, atkY;

#define RET_CHECK_RETURN_POSITION_O3_SWITCH_1 0

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
  printf("\033[H\033[J");
  fflush(stdout);

  printf("  %s\n +--------+\n", horizontal);
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

  printf(" +--------+\n  %s\n\n", horizontal);

user_input:
  printf("%s> ", playSide ? "white" : "black");
  fflush(stdout);

  fgets(buffer, 79, stdin);

parse_user_input:
  if (strlen(buffer) != 6) {
    goto invalid_user_input;
  }
  
  if (buffer[2] != ',' && buffer[2] != '-') {
    goto invalid_user_input;
  }
  
  srcX = toupper(buffer[0]) - 'A';
  srcY = buffer[1] - '0';
  dstX = toupper(buffer[3]) - 'A';
  dstY = buffer[4] - '0';
  
  if (srcX < 0 || srcX >= 8 || dstX < 0 || dstX >= 8
      || srcY < 0 || srcY >= 8 || dstY < 0 || dstY >= 8) {
    goto invalid_user_input;
  }

  srcY1 = 7 - srcY;
  dstY1 = 7 - dstY;

verify_user_input:  
  memcpy(chessBoard2, chessBoard, 64);
  srcPiece = chessBoard2[srcY1][srcX];
  dstPiece = chessBoard2[dstY1][dstX];
  
  if ((playSide && !strchr(whitePieces, srcPiece))
      || (!playSide && !strchr(blackPieces, srcPiece))) {
    printf("cannot move from %c%d, because there's not your piece.",
           horizontal[srcX], srcY);
    goto pause_reprint;
  }

  potentialKingRookSwitch = NULL;
  if (playSide == 1 && srcPiece == 'K' && dstPiece == 'R'
      && srcY == 7 && dstY == 7) {
    potentialKingRookSwitch = chessBoard[srcY1];
  } else if (playSide == 0 && srcPiece == 'k' && dstPiece == 'r'
             && srcY == 0 && dstY == 0) {
    poteltialKingRookSwitch = chessBoard[srcY1];
  }

    if (srcY == 7 && dstY == 7 && srcX == 4 && dstX == 0) {
      if (!oooPossible[playSide]) {
        printf("cannot perform king-rook switch: king or rook already moved");
        goto pause_reprint;
      }

      for (i = 1; i < 4; i++) {
        if (chessBoard2[7][i] != '.') {
          printf("cannot perform king-rook switch: blocked by %c%d",
                 horizontal[i], srcY);
          goto pause_reprint;
        }

        ty = srcY1;
        tx = i;
        check_return_position = RET_CHECK_RETURN_POSITION_O3_SWITCH_1;
        hasAttack = 0;
        goto check_position_attacked;
ret_check_position_attacked_o3_switch1:
        if (hasAttack) {
          printf("cannot perform king-rook switch: position %c%d attacked by %c%d",
                 horizontal[srcX], srcY, horizontal[atkX], 7 - atkY);
          goto pause_reprint;
        }
      }
      goto check_if_mate;
    }
  }

  goto program_end;

check_if_mate:
  goto finish_move_piece;

finish_move_piece:
  memcpy(chessBoard, chessBoard2, 64);
  goto print_chess_board;

invalid_user_input:
  printf("  - invalid input.\n");
  printf("    expected: [source],[dest]\n");
  printf("          or: [source]-[dest]\n");
  printf("     example: e2,e4\n");
  printf("          or: e2-e4\n");

pause_reprint:
  getchar();
  goto print_chess_board;

program_end:
  printf("program ended.\n");
  return 0;

check_position_attacked:
  goto ret_check_position_attacked_o3_switch1;
}
