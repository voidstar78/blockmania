#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <termios.h>
#include <string.h>

#define DEFAULT_HEIGHT  22
#define DEFAULT_WIDTH	16

#define TRUE 1
#define FALSE 0

/* GLOBALS */
int iBoardHeight = -1;
int iBoardWidth = -1;

int iCurrentX = -1;  /* X index of current block (width) */
int iCurrentY = -1;  /* Y index of current block (height) */

/* Index (into iaBlockData) of the current block that is dropping. */
int iCurrentBlockIndex = -1;
/* Additional offset index (into iaBlockData) that represents
     the current rotation of the block that is dropping.  */
int iCurrentBlockRotation = 0;

/* An integer array that represents the current Tetris board.
   Index 0 is the top of the board, iaBoard[iBoardHeight-1] is the bottom. */
int* iaBoard;
int iaBoard_Length;
int iaBlockMask[5];

int iEmptyRowMask = -1;
int iFullRowMask = -1;

int iRowsRemoved = 0;

#define NUM_BLOCK_TYPES 7

int iaBlockData[28];

void initBlockData();
void initialize();
void doInitialize(int iDefaultHeight, int iDefaultWidth);
void initializeBoard(int iHeight, int iWidth);
int* iaOverlayBlock();
void updateBlockMask();
short bBlockCollision();
short bMoveDownOne();
short bMoveLeft();
short bMoveRight();
short bRotate(int iRotateAmount);
void startNewBlock();
void clearFullRows();
short bRemoveLowestFullRow();
void startNewBlock(int iBlockIndex);
int iGetRowsRemoved();
void showBoard();

void initBlockData() {
    
    /* BLOCK 0
      // -------
      // 00000       00000       00100       00000
      // 00100       00000       00100       00000
      // 00100       11110       00100       01111
      // 00100       00000       00100       00000
      // 00100       00000       00000       00000
    */          
      iaBlockData[0] = 0x0021084;
      iaBlockData[1] = 0x0007800;
      iaBlockData[2] = 0x0421080;
      iaBlockData[3] = 0x0003C00;

      /* BLOCK 1
      // -------
      // 00000       00000       00000       00000
      // 00100       00100       00000       00100
      // 01110       00110       01110       01100
      // 00000       00100       00100       00100
      // 00000       00000       00000       00000
      */          
      iaBlockData[4] = 0x0023800;
      iaBlockData[5] = 0x0021880;
      iaBlockData[6] = 0x0003880;
      iaBlockData[7] = 0x0023080;

      /* BLOCK 2
      // -------
      // 00000       00000       00000       00000
      // 00110       00110       00110       00110
      // 00110       00110       00110       00110
      // 00000       00000       00000       00000
      // 00000       00000       00000       00000
      */          
      iaBlockData[8] = 0x0031800;
      iaBlockData[9] = 0x0031800;
      iaBlockData[10] = 0x0031800;
      iaBlockData[11] = 0x0031800;

      /* BLOCK 3
      // -------
      // 00000       00000       00000       00000
      // 00100       00000       01000       00110
      // 00110       00110       01100       01100
      // 00010       01100       00100       00000
      // 00000       00000       00000       00000
      */          
      iaBlockData[12] = 0x0021840;
      iaBlockData[13] = 0x0001980;
      iaBlockData[14] = 0x0043080;
      iaBlockData[15] = 0x0033000;

      // BLOCK 4
      // -------
      // 00000       00000       00000       00000
      // 00100       01100       00010       00000
      // 01100       00110       00110       01100
      // 01000       00000       00100       00110
      // 00000       00000       00000       00000
      //        
      iaBlockData[16] = 0x0023100;
      iaBlockData[17] = 0x0061800;
      iaBlockData[18] = 0x0011880;
      iaBlockData[19] = 0x00030C0;

      // BLOCK 5
      // -------
      // 00000       00000       00000       00000
      // 00110       00000       00100       01000
      // 00100       01110       00100       01110
      // 00100       00010       01100       00000
      // 00000       00000       00000       00000
      iaBlockData[20] = 0x0031080;
      iaBlockData[21] = 0x0003840;
      iaBlockData[22] = 0x0021180;
      iaBlockData[23] = 0x0043800;

      // BLOCK 6
      // -------
      // 00000       00000       00000       00000
      // 01100       00010       00100       00000
      // 00100       01110       00100       01110
      // 00100       00000       00110       01000
      // 00000       00000       00000       00000
      iaBlockData[24] = 0x0061080;
      iaBlockData[25] = 0x0013800;
      iaBlockData[26] = 0x00210C0;
      iaBlockData[27] = 0x0003900;

    };

static struct termios stored_settings;

void set_keypress(void) {
    struct termios new_settings;
    tcgetattr(0, &stored_settings);
    new_settings = stored_settings;
    new_settings.c_lflag &= (~ICANON);
    new_settings.c_cc[VTIME] = 0;
    new_settings.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &new_settings);
}

void reset_keyboard(void) {
    tcsetattr(0, TCSANOW, &stored_settings);
}
    
void initialize() {
  doInitialize(DEFAULT_HEIGHT, DEFAULT_WIDTH);
}

void doInitialize(int iDefaultHeight, int iDefaultWidth) {
    initBlockData();
    initializeBoard(iDefaultHeight, iDefaultWidth);
    startNewBlock();
}

void initializeBoard(int iHeight, int iWidth) {
    iBoardHeight = iHeight;
    iBoardWidth = iWidth;

    iaBoard = (int *)calloc( iHeight+3, sizeof(int) );
    iaBoard_Length = iHeight+3;
    
    int iHeightSub1 = iHeight-1; 
    int iWidthSub1 = iWidth-1;

    iEmptyRowMask = (1 << iWidth) | 2;

    /* Set the border around the board. */
    for (int i = 0; i < iHeightSub1; i++)
      iaBoard[i] = iEmptyRowMask;
    iaBoard[iHeightSub1] = 0xFFFFFFFF;

    iFullRowMask = 1;
    for (int i = 0; i < iWidthSub1; i++)
      iFullRowMask = (iFullRowMask << 1) | 1;
    iFullRowMask = iFullRowMask << 1;
}

int* iaOverlayBlock() {
    /* Returns an integer array representing the Tetris board, with the current
       block being part of the board. */

    /* Copy the current board. */
    int* iaResult = (int *)calloc( iaBoard_Length, sizeof(int) );

    for (int i = 0; i < iBoardHeight; i++)
      iaResult[i] = iaBoard[i];

    updateBlockMask();

    /* Mask the block onto the board layout. */
    iaResult[iCurrentY+0] = iaBoard[iCurrentY+0] | iaBlockMask[0];
    iaResult[iCurrentY+1] = iaBoard[iCurrentY+1] | iaBlockMask[1];
    iaResult[iCurrentY+2] = iaBoard[iCurrentY+2] | iaBlockMask[2];
    iaResult[iCurrentY+3] = iaBoard[iCurrentY+3] | iaBlockMask[3];
    iaResult[iCurrentY+4] = iaBoard[iCurrentY+4] | iaBlockMask[4];
   
    return iaResult;
}

void updateBlockMask() {
    int iBlockMask = iaBlockData[(iCurrentBlockIndex*4)+iCurrentBlockRotation];
    iaBlockMask[0] = ( (iBlockMask & 0x1F00000) >> 20 ) << iCurrentX;
    iaBlockMask[1] = ( (iBlockMask & 0x00F8000) >> 15 ) << iCurrentX;
    iaBlockMask[2] = ( (iBlockMask & 0x0007C00) >> 10 ) << iCurrentX;
    iaBlockMask[3] = ( (iBlockMask & 0x00003E0) >>  5 ) << iCurrentX;
    iaBlockMask[4] = ( (iBlockMask & 0x000001F) >>  0 ) << iCurrentX;
//    return iaBlockMask;
}

short bBlockCollision() {
    /* Return TRUE if current block has collided with any board piece.
       Otherwise, return FALSE. */
    updateBlockMask();
//    iaBlockMask = iaGetBlockMask();
    for (int i = 0; i <= 4; i++) {
	if ( (iaBoard[iCurrentY+i] & iaBlockMask[i]) != 0) {
          return TRUE;
        }
    }
    return FALSE;
}

short bMoveDownOne() {
    /* Return TRUE if the current piece was successfully moved
       down one row.  Otherwise, make the block a permenant,
       start a new block, and return FALSE. */

    iCurrentY++;
    if (bBlockCollision()) {
	/* Move the block back where it was, since it can't move 
	   down any further. */
      iCurrentY--;

      /* Make block permanent part of board.  The block mask was
         obtained already when bBlockCollision() was called. */
      for (int i = 0; i <= 4; i++) {
        iaBoard[iCurrentY+i] = (iaBoard[iCurrentY+i] | iaBlockMask[i]);
      }    

      startNewBlock();

      return FALSE;
    }
    return TRUE;
}

short bMoveLeft() {
    iCurrentX++;
    if (bBlockCollision()) {
      iCurrentX--;
      return FALSE;
    }
    return TRUE;
}

short bMoveRight() {
    iCurrentX--;
    if ( (iCurrentX < 0) || bBlockCollision()) {
      iCurrentX++;
      return FALSE;
    }
    return TRUE;
}

short bRotate(int iRotateAmount) {
    /* iRotateAmount > 0 to rotate right
       iRotateAmount < 0 to rotate left */

    int iOriginalRotation = iCurrentBlockRotation;

    iCurrentBlockRotation = (iCurrentBlockRotation + iRotateAmount);
    if (iCurrentBlockRotation < 0)
      iCurrentBlockRotation = 3;
    else
      iCurrentBlockRotation = iCurrentBlockRotation % 4;

    if (bBlockCollision()) {
      iCurrentBlockRotation = iOriginalRotation;
      return FALSE;
    }

    return TRUE;
}

void startNewBlock() {
    //  0 and NUM_BLOCK_TYPES
    int p = (int)( (rand() / 32767.0) * NUM_BLOCK_TYPES );
    startNewBlock( p );
}

void clearFullRows() {  
    while (bRemoveLowestFullRow()) {
      iRowsRemoved++;
    }
}

short bRemoveLowestFullRow() {
    for (int i = iBoardHeight-2; i >= 0; i--) {
      if ( (iaBoard[i] & iFullRowMask) == iFullRowMask ) {
        for (int x = i; x > 0; x--) {
          iaBoard[x] = iaBoard[x-1];
        }
        iaBoard[0] = iEmptyRowMask;
        return TRUE;
      }
    }
    return FALSE;
}

void startNewBlock(int iBlockIndex) {
    iCurrentX = (DEFAULT_WIDTH / 2) - 2;
    iCurrentY = 0;
    iCurrentBlockIndex = iBlockIndex;
}

int iGetRowsRemoved() {
    return iRowsRemoved;
}

void showBoard() {
    
    int* iaBoard = iaOverlayBlock();

    for (int i = 0; i < iBoardHeight; i++) {

      int iMask = (1 << iBoardWidth);
      for (int x = 0; x <= iBoardWidth; x++) {
        if ( (iaBoard[i] & iMask) == 0 )
          printf(" ");
	
        else
          printf("*");
	
        iMask = (iMask >> 1);
      }
      printf("\n");
      

    }

}

/*
  int isready(int fd) {
  int rc;
  fd_set fds;
  struct timeval tv;
  FD_ZERO(&fds);
  FD_SET(fd, &fds);
  tv.tv_sec = tv.tv_usec = 0;
  rc = select(fd+1, &fds, NULL, NULL, &tv);
  if (rc < 0)
      return -1;
  return FD_ISSET(fd, &fds) ? 1 : 0;
}
*/

int main() {
    
    char ch;

    initialize();

    set_keypress();
    fflush(stdin);
    
    while (true) {

	showBoard();

/*	    if (isready(stdin)) { */
		fread(&ch, 1, 1, stdin);
		printf("%d\n", ch);
/*	    } */
	    fflush(stdin);
	
	switch (ch) {
	    case 'q':
	    case 'Q':
	    case 'x':
	    case 'X':
            // Close the standard input buffer.
            exit(0);
	    break;
	    case 's':
	    case 'S':
            bMoveLeft();
            break;
	    case 'd':
          case 'D':
            bMoveRight();
            break;
	    case 'f':
          case 'F':
            bRotate(1);
            break;
	    case 'a':
          case 'A':
            bRotate(-1);
            break;
          default:
             ch = '\0';
	     break;
        }
	//}

        if (!bMoveDownOne()) {
          startNewBlock();
          if (bBlockCollision()) {
            printf("Game Over");
            exit(1);
          }
          clearFullRows();
        }
	
    }
   return 0;
}
