#!/appserv/perl/bin/perl

#use Term::ReadKey qw( ReadMode ReadKey );
use Term::ReadKey;
use Fcntl;
open(IN, "</dev/tty");

# The following line enabled "getc" to read a key from the keyboard
# without the user having to press ENTER.
#system "stty", "-icanon", "eol", "\001";

$DEFAULT_HEIGHT = 22;
$DEFAULT_WIDTH = 16;

$MOVE_DOWN_ONE = -1;  # Holds result of call to moveDownOne()
$BLOCK_COLLISION = -1;  # Holds result of call to checkBlockCollision()
$REMOVED_A_ROW = -1;  # Holds result of call to removeLowestFullRow()

$TRUE = 1;
$FALSE = 0;

$CLOCKWISE = 1;
$COUNTERCLOCKWISE = -1;

$iRowsRemoved = 0;  # Holds count of total rows removed

$iBoardHeight = -1;
$iBoardWidth = -1;

$iCurrentX = -1;
$iCurrentY = -1;

$iCurrentBlockIndex = -1;
$iCurrentBlockRotation = -1;

@iaBoard[0] = 0;

$NUM_BLOCK_TYPES = 7;

# Bit pattern for all rotations of block 0
@iaBlockData[0] = 0x0021084;
@iaBlockData[1] = 0x0007800;
@iaBlockData[2] = 0x0421080;
@iaBlockData[3] = 0x0003C00;

# Bit pattern for all rotations of block 1
@iaBlockData[4] = 0x0023800;
@iaBlockData[5] = 0x0021880;
@iaBlockData[6] = 0x0003880;
@iaBlockData[7] = 0x0023080;

# Bit pattern for all rotations of block 2
@iaBlockData[8] = 0x0031800;
@iaBlockData[9] = 0x0031800;
@iaBlockData[10] = 0x0031800;
@iaBlockData[11] = 0x0031800;

# Bit pattern for all rotations of block 3
@iaBlockData[12] = 0x0021840;
@iaBlockData[13] = 0x0001980;
@iaBlockData[14] = 0x0043080;
@iaBlockData[15] = 0x0033000;

# Bit pattern for all rotations of block 4
@iaBlockData[16] = 0x0023100;
@iaBlockData[17] = 0x0061800;
@iaBlockData[18] = 0x0011880;
@iaBlockData[19] = 0x00030C0;

# Bit pattern for all rotations of block 5
@iaBlockData[20] = 0x0031080;
@iaBlockData[21] = 0x0003840;
@iaBlockData[22] = 0x0021180;
@iaBlockData[23] = 0x0043800;

# Bit pattern for all rotations of block 6
@iaBlockData[24] = 0x0061080;
@iaBlockData[25] = 0x0013800;
@iaBlockData[26] = 0x00210C0;
@iaBlockData[27] = 0x0003900;

@iaBlockMask[0] = 0;

$iEmptyRowMask = -1;
$iFullRowMask = -1;

$iRowsRemoved = 0;

$iNextPiece = int(rand $NUM_BLOCK_TYPES);

# -- main --------------------------------

  ReadMode 3, IN;
  $in = \*IN;

  initialize();

  $oldtime = time();

  while (true) {

    system "clear";
    showBoard();
    print "Rows Cleared = $iRowsRemoved \n";
    print "Next Piece = $iNextPiece \n";

    $key = ReadKey(-1, $in);
    #$key = getc;

    if ($key eq 'q') {
      die "User Quits\n";
    }
    elsif ($key eq 's') {
      moveLeft();
    }
    elsif ($key eq 'd') {
      moveRight();
    }
    elsif ($key eq 'f') {
      doRotate($CLOCKWISE);
    }
    elsif ($key eq 'a') {
      doRotate($COUNTERCLOCKWISE);
    }
    elsif ($key eq ' ') {
      do {
        moveDownOne();
      } while ($MOVE_DOWN_ONE == $TRUE);
    }

    if ( (time() - $oldtime) > 0) {
      $oldtime = time(); 
      moveDownOne();
      if ($MOVE_DOWN_ONE == $FALSE) {
        startNewRandomBlock();
        checkBlockCollision();
        if ($BLOCK_COLLISION == $TRUE) {
          die "Game Over!\n";
        }
        clearFullRows();
      }
    }

  }

# -------------------------------------------------------

sub doRotate {
  # Parameter 0: iRotateAmount
  # If iRotateAmount > 0 to rotate clockwise
  # If iRotateAmount < 0 to rotate counter clockwise

  $iOriginalRotation = $iCurrentBlockRotation;

  $iCurrentBlockRotation += $_[0];
  if ($iCurrentBlockRotation < 0) {
    $iCurrentBlockRotation = 3;
  } else {
    $iCurrentBlockRotation = $iCurrentBlockRotation % 4;
  }

  checkBlockCollision();
  if ($BLOCK_COLLISION == 1) {
    $iCurrentBlockRotation = $iOriginalRotation;
  }

}

sub moveLeft {
  $iCurrentX++;
  checkBlockCollision();
  if ($BLOCK_COLLISION == 1) {
    $iCurrentX--;
  }
}

sub moveRight {
  $iCurrentX--;
  checkBlockCollision();
  if ( ($iCurrentX < 0) || ($BLOCK_COLLISION == $TRUE)) {
    $iCurrentX++;
  }
}

sub moveDownOne {

  $iCurrentY++;

  checkBlockCollision();

  $MOVE_DOWN_ONE = $TRUE;
  if ($BLOCK_COLLISION == $TRUE) {
    $iCurrentY--;
    for ($i = 0; $i <= 4; $i++) {
      @iaBoard[$iCurrentY+$i] = (@iaBoard[$iCurrentY+$i] | $iaBlockMask[$i]);
    }
    $MOVE_DOWN_ONE = $FALSE;
  }
}

sub clearFullRows {  
  do {
    removeLowestFullRow();
  } while ($REMOVED_A_ROW == $TRUE);
}

sub removeLowestFullRow {
# Sets REMOVED_A_ROW if one row was removed.  This does not mean that
# all of the lowest full rows have been removed.

  $REMOVED_A_ROW = $FALSE;
  for ($i = $iBoardHeight-2; $i >= 0; $i--) {
    # If the ith row is full...
    if ( (@iaBoard[$i] & $iFullRowMask) == $iFullRowMask ) {
      # Move down all the rows above the ith row...
      for ($x = $i; $x > 0; $x--) {
        @iaBoard[$x] = @iaBoard[$x-1];
      }
      # and clear the top row...
      @iaBoard[0] = $iEmptyRowMask;

      $REMOVED_A_ROW = $TRUE;
      $iRowsRemoved++;
      break;
    }
  }

}

sub checkBlockCollision {
# Sets BLOCK_COLLISION if the current block has collided with any part
# of the board (including pieces that have are been dropped).  If this
# is true, then the block should be moved back to it's previous location.

  updateBlockMask();

  $BLOCK_COLLISION = $FALSE;
  for ($i = 0; $i <= 4; $i++) {
    if ( (@iaBoard[$iCurrentY+$i] & $iaBlockMask[$i]) != 0) {
      $BLOCK_COLLISION = $TRUE;
      break;
    }
  }

}

sub startNewBlock {
  # Parameter 0: iBlockIndex
  $iCurrentX = ($DEFAULT_WIDTH / 2) - 2;
  $iCurrentY = 0;
  $iCurrentBlockIndex = $_[0];
  $iCurrentBlockRotation = 0;
}

sub startNewRandomBlock {
  startNewBlock($iNextPiece);
  $iNextPiece = int(rand $NUM_BLOCK_TYPES);
}

sub doInitialize {
  # Parameter 0: iDefaultHeight
  # Parameter 1: iDefaultWidth
  initBlockData;
  initializeBoard($_[0], $_[1]);
  startNewRandomBlock;
}

sub initialize {
  doInitialize($DEFAULT_HEIGHT, $DEFAULT_WIDTH);
}

sub initializeBoard {
  # Parameter 0: iHeight
  # Parameter 1: iWidth

  $iBoardHeight = $_[0];
  $iBoardWidth = $_[1];

  $iHeightSub1 = $iBoardHeight-1; 
  $iWidthSub1 = $iBoardWidth-1;

  $iEmptyRowMask = (1 << $iBoardWidth) | 2;

  for ($i = 0; $i < $iHeightSub1; $i++) {
    @iaBoard[$i] = $iEmptyRowMask;
  }
  @iaBoard[$iHeightSub1] = 0xFFFFFFFE;

  $iFullRowMask = 1;
  for ($i = 0; $i < $iWidthSub1; $i++) {
    $iFullRowMask = ($iFullRowMask << 1) | 1;
  }
  $iFullRowMask = $iFullRowMask << 1;
}

sub updateBlockMask {

  $iBlockMask = @iaBlockData[($iCurrentBlockIndex*4)+$iCurrentBlockRotation];

  @iaBlockMask[0] = ( ($iBlockMask & 0x1F00000) >> 20 ) << $iCurrentX;
  @iaBlockMask[1] = ( ($iBlockMask & 0x00F8000) >> 15 ) << $iCurrentX;
  @iaBlockMask[2] = ( ($iBlockMask & 0x0007C00) >> 10 ) << $iCurrentX;
  @iaBlockMask[3] = ( ($iBlockMask & 0x00003E0) >>  5 ) << $iCurrentX;
  @iaBlockMask[4] = ( ($iBlockMask & 0x000001F) >>  0 ) << $iCurrentX;

}

sub showBoard {

  for ($i = 0; $i < $iBoardHeight; $i++) {
    @iaResult[$i] = @iaBoard[$i];
  }

  updateBlockMask;

  @iaResult[$iCurrentY+0] = @iaBoard[$iCurrentY+0] | @iaBlockMask[0];
  @iaResult[$iCurrentY+1] = @iaBoard[$iCurrentY+1] | @iaBlockMask[1];
  @iaResult[$iCurrentY+2] = @iaBoard[$iCurrentY+2] | @iaBlockMask[2];
  @iaResult[$iCurrentY+3] = @iaBoard[$iCurrentY+3] | @iaBlockMask[3];
  @iaResult[$iCurrentY+4] = @iaBoard[$iCurrentY+4] | @iaBlockMask[4];

  for ($i = 0; $i < $iBoardHeight; $i++) {
    $iMask = (1 << $iBoardWidth);
    for ($x = 0; $x <= $iBoardWidth; $x++) {
      if ( (@iaResult[$i] & $iMask) == 0 ) {
        print(" ");
      } else {
        print("*");
      }
      $iMask = ($iMask >> 1);
    }
    print("\n");
  }

}
