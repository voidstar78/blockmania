/*

TITLE....: Tris Java API
AUTHOR...: Steve Lewis
DATE.....: 09/16/2001

TO BE DONE:
- Completed rows are not removed.
- No score.
- More debugging in moving blocks to the right side.

*/

public class TetrisBoard {

  private static int DEFAULT_HEIGHT = 22;
  private static int DEFAULT_WIDTH = 16;  // Width limited to 32

  // Current board height (depth) and width.
  private int iBoardHeight = -1;
  private int iBoardWidth = -1;

  private int iCurrentX = -1;  // X index of current block (width)
  private int iCurrentY = -1;  // Y index of current block (height)

  // Index (into iaBlockData) of the current block that is dropping.
  private int iCurrentBlockIndex = -1;
  // Additional offset index (into iaBlockData) that represents
  //   the current rotation of the block that is dropping.
  private int iCurrentBlockRotation = 0;

  // An integer array that represents the current Tetris board.
  //    Index 0 is the top of the board, iaBoard[iBoardHeight-1] is the bottom.
  private int[] iaBoard = null; 

  private int[] iaBlockMask = new int[5];

  private int iEmptyRowMask = -1;
  private int iFullRowMask = -1;

  private int iRowsRemoved = 0;

  private static int NUM_BLOCK_TYPES = 7;
  private static int[] iaBlockData =
    {

      // BLOCK 0
      // -------
      // 00000       00000       00100       00000
      // 00100       00000       00100       00000
      // 00100       11110       00100       01111
      // 00100       00000       00100       00000
      // 00100       00000       00000       00000
      //          
      0x0021084,  0x0007800,  0x0421080,  0x0003C00,

      // BLOCK 1
      // -------
      // 00000       00000       00000       00000
      // 00100       00100       00000       00100
      // 01110       00110       01110       01100
      // 00000       00100       00100       00100
      // 00000       00000       00000       00000
      //          
      0x0023800,  0x0021880,  0x0003880,  0x0023080,

      // BLOCK 2
      // -------
      // 00000       00000       00000       00000
      // 00110       00110       00110       00110
      // 00110       00110       00110       00110
      // 00000       00000       00000       00000
      // 00000       00000       00000       00000
      //          
      0x0031800,  0x0031800,  0x0031800,  0x0031800,

      // BLOCK 3
      // -------
      // 00000       00000       00000       00000
      // 00100       00000       01000       00110
      // 00110       00110       01100       01100
      // 00010       01100       00100       00000
      // 00000       00000       00000       00000
      //          
      0x0021840,  0x0001980,  0x0043080,  0x0033000,

      // BLOCK 4
      // -------
      // 00000       00000       00000       00000
      // 00100       01100       00010       00000
      // 01100       00110       00110       01100
      // 01000       00000       00100       00110
      // 00000       00000       00000       00000
      //        
      0x0023100,  0x0061800,  0x0011880,  0x00030C0,

      // BLOCK 5
      // -------
      // 00000       00000       00000       00000
      // 00110       00000       00100       01000
      // 00100       01110       00100       01110
      // 00100       00010       01100       00000
      // 00000       00000       00000       00000
      0x0031080,  0x0003840,  0x0021180,  0x0043800,

      // BLOCK 6
      // -------
      // 00000       00000       00000       00000
      // 01100       00010       00100       00000
      // 00100       01110       00100       01110
      // 00100       00000       00110       01000
      // 00000       00000       00000       00000
      0x0061080,  0x0013800,  0x00210C0,  0x0003900

    };

  // CONSTRUCTORS
  public TetrisBoard() {
    this(DEFAULT_HEIGHT, DEFAULT_WIDTH);
  }

  public TetrisBoard(int iDefaultHeight, int iDefaultWidth) {
    initializeBoard(iDefaultHeight, iDefaultWidth);
    startNewBlock();
  }

  // CLASS METOHDS
  private void initializeBoard(int iHeight, int iWidth) {
    iBoardHeight = iHeight;
    iBoardWidth = iWidth;
    iaBoard = new int[iHeight+3]; 

    int iHeightSub1 = iHeight-1; 
    int iWidthSub1 = iWidth-1;

    iEmptyRowMask = (1 << iWidth) | 2;

    // Set the border around the board.
    for (int i = 0; i < iHeightSub1; i++)
      iaBoard[i] = iEmptyRowMask;
    iaBoard[iHeightSub1] = 0xFFFFFFFF;

    iFullRowMask = 1;
    for (int i = 0; i < iWidthSub1; i++)
      iFullRowMask = (iFullRowMask << 1) | 1;
    iFullRowMask = iFullRowMask << 1;
  }

  private int[] iaOverlayBlock() {
    // Returns an integer array representing the Tetris board, with the current
    //   block being part of the board.

    // Copy the current board.
    int[] iaResult = new int[iaBoard.length];
    for (int i = 0; i < iBoardHeight; i++)
      iaResult[i] = iaBoard[i];

    iaBlockMask = iaGetBlockMask();

    // Mask the block onto the board layout.
    iaResult[iCurrentY+0] = iaBoard[iCurrentY+0] | iaBlockMask[0];
    iaResult[iCurrentY+1] = iaBoard[iCurrentY+1] | iaBlockMask[1];
    iaResult[iCurrentY+2] = iaBoard[iCurrentY+2] | iaBlockMask[2];
    iaResult[iCurrentY+3] = iaBoard[iCurrentY+3] | iaBlockMask[3];
    iaResult[iCurrentY+4] = iaBoard[iCurrentY+4] | iaBlockMask[4];
   
    return iaResult;
  }

  private int[] iaGetBlockMask() {
    int iBlockMask = iaBlockData[(iCurrentBlockIndex*4)+iCurrentBlockRotation];
    iaBlockMask[0] = ( (iBlockMask & 0x1F00000) >> 20 ) << iCurrentX;
    iaBlockMask[1] = ( (iBlockMask & 0x00F8000) >> 15 ) << iCurrentX;
    iaBlockMask[2] = ( (iBlockMask & 0x0007C00) >> 10 ) << iCurrentX;
    iaBlockMask[3] = ( (iBlockMask & 0x00003E0) >>  5 ) << iCurrentX;
    iaBlockMask[4] = ( (iBlockMask & 0x000001F) >>  0 ) << iCurrentX;
    return iaBlockMask;
  }

  public boolean bBlockCollision() {
    // Return TRUE if current block has collided with any board piece.
    //   Otherwise, return FALSE.
    iaBlockMask = iaGetBlockMask();
    for (int i = 0; i <= 4; i++) {
	if ( (iaBoard[iCurrentY+i] & iaBlockMask[i]) != 0) {
          return true;
        }
    }
    return false;
  }

  public boolean bMoveDownOne() {
    // Return TRUE if the current piece was successfully moved
    //   down one row.  Otherwise, make the block a permenant,
    //   start a new block, and return FALSE.

    iCurrentY++;
    if (bBlockCollision()) {
      // Move the block back where it was, since it can't move 
      //   down any further.
      iCurrentY--;

      // Make block permanent part of board.  The block mask was
      //   obtained already when bBlockCollision() was called.
      for (int i = 0; i <= 4; i++) {
        iaBoard[iCurrentY+i] = (iaBoard[iCurrentY+i] | iaBlockMask[i]);
      }    

      startNewBlock();

      return false;
    }
    return true;  
  }

  public boolean bMoveLeft() {
    iCurrentX++;
    if (bBlockCollision()) {
      iCurrentX--;
      return false;
    }
    return true;
  }

  public boolean bMoveRight() {
    iCurrentX--;
    if ( (iCurrentX < 0) || bBlockCollision()) {
      iCurrentX++;
      return false;
    }
    return true;
  }

  public boolean bRotate(int iRotateAmount) {
    // iRotateAmount > 0 to rotate right
    // iRotateAmount < 0 to rotate left 

    int iOriginalRotation = iCurrentBlockRotation;

    iCurrentBlockRotation = (iCurrentBlockRotation + iRotateAmount);
    if (iCurrentBlockRotation < 0)
      iCurrentBlockRotation = 3;
    else
      iCurrentBlockRotation = iCurrentBlockRotation % 4;

    if (bBlockCollision()) {
      iCurrentBlockRotation = iOriginalRotation;
      return false;
    }

    return true;
  }

  public void startNewBlock() {
    startNewBlock( (int)(Math.random() * NUM_BLOCK_TYPES) );
  }

  public void clearFullRows() {  
    while (bRemoveLowestFullRow()) {
      iRowsRemoved++;
    }
  }

  private boolean bRemoveLowestFullRow() {
    for (int i = iBoardHeight-2; i >= 0; i--) {
      if ( (iaBoard[i] & iFullRowMask) == iFullRowMask ) {
        for (int x = i; x > 0; x--) {
          iaBoard[x] = iaBoard[x-1];
        }
        iaBoard[0] = iEmptyRowMask;
        return true;
      }
    }
    return false;
  }

  public void startNewBlock(int iBlockIndex) {
    iCurrentX = (DEFAULT_WIDTH / 2) - 2;
    iCurrentY = 0;
    iCurrentBlockIndex = iBlockIndex;
  }

  public int iGetRowsRemoved() {
    return iRowsRemoved;
  }

  public String toString() {

    StringBuffer sbResult = new StringBuffer();

    int[] iaBoard = iaOverlayBlock();

    for (int i = 0; i < iBoardHeight; i++) {

      int iMask = (1 << iBoardWidth);
      for (int x = 0; x <= iBoardWidth; x++) {
        if ( (iaBoard[i] & iMask) == 0 )
          sbResult.append(' ');
        else
          sbResult.append('*');
        iMask = (iMask >> 1);
      }
      sbResult.append('\n');

    }

    return sbResult.toString();
  }

}  // end class TetrisBoard

