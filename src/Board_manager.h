#ifndef BLOCKMANIA_BOARD_MANAGER_H
#define BLOCKMANIA_BOARD_MANAGER_H

#include "Playing_board.h"

namespace Blockmania {

/*******************************************************************************
Steve Lewis - May 2006

The Board_manager applies a "builder pattern" to instantiate and initialize the
Blockmania Playing_board template class.

The Playing_board consists of a collection of user-defined "pieces" that can be
used on the board.  Each piece has a sequence of rotations, which is also
user defined.

The Board_manager simply initializes a particular set of pieces and rotations,
and thereby primarily demonstrates how the Playing_board would be used.

Another Board_manager implementation could instead read pieces and rotation
configurations from a file, etc.  The particular pieces and rotations used
in this implementation model the standard Tetris block piece configuration.
*******************************************************************************/
class Board_manager
{

public:

  // Single location that defines the height and width of the playing board.
  static const int height = 22;
  static const int width = 12;

  // Define a concrete type for a particular type of Playing_board.
  typedef Playing_board<int> Board;

  // Helper type that brings the playing boards block content type into
  // this scope (so that users don't have to manually reference into the
  // Playing_board type)
  typedef Board::Content_type Board_content_type;

  // Constructor that initializes the playing board instance.  Throws
  // an exception if there is any problem (e.g. insufficient memory to
  // allocate pieces and rotations)
  Board_manager();

  // Accessor method to provide users a direct reference to the playing
  // board instance.
  inline Board& board() { return board_; }

private:

  void initialize_puzzle_pieces();

  Board board_;

};

}

#endif
