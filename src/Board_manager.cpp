#include "Board_manager.h"

#include "Puzzle_piece.h"  // used as concrete class definition for IPuzzle_piece

namespace Blockmania {

Board_manager::Board_manager()
:
  board_(height, width)
{
  initialize_puzzle_pieces();
}

void Board_manager::initialize_puzzle_pieces()
{
// NOTE1: When the rotation grid pointer is added to the puzzle piece,
// the puzzle piece then becomes the "owner" of that pointer and is
// responsible for its deallocation.  Likewise, when a puzzle piece
// pointer is added to the board, the board then becomes the owner
// of that pointer and is responsible for its deallocation.  In otherwords,
// the Board_manager is not responsible for the memory clean up of puzzle
// piece and rotation grid allocations.

// NOTE2: By default, the Playing_board is configured such that the content of
// the puzzle pieces are represented as integers.  This could be changed to be
// a char or user defined type, if desired.  Therefore, when the rotation grids
// are added to the puzzle pieces below, the integer values represent the
// "style content" associated with that puzzle piece and that rotation.  The
// user (e.g. GUI) can then interpret these values however desired (e.g. to
// control the color or fill-style used when rendering the contents of a puzzle
// piece).

  // PIECE TYPE #1 a 5x5 grid containing a line shape, rotated about the center
  {
    Puzzle_piece<Board_content_type>* pp55 = new Puzzle_piece<Board_content_type>(5,5);
// 00000  00000  00#00  00000
// 00#00  00000  00#00  00000
// 00#00  ####0  00#00  0####
// 00#00  00000  00#00  00000
// 00#00  00000  00000  00000
    Rotation_grid<Board_content_type>* r_grid55 = new Rotation_grid<Board_content_type>(5,5);
    *r_grid55
      << 0 << 0 << 0 << 0 << 0
      << 0 << 0 << 1 << 0 << 0
      << 0 << 0 << 1 << 0 << 0
      << 0 << 0 << 1 << 0 << 0
      << 0 << 0 << 1 << 0 << 0;
    pp55->add_rotation_grid(r_grid55);

    r_grid55 = new Rotation_grid<Board_content_type>(5,5);
    *r_grid55
      << 0 << 0 << 0 << 0 << 0
      << 0 << 0 << 0 << 0 << 0
      << 1 << 1 << 1 << 1 << 0
      << 0 << 0 << 0 << 0 << 0
      << 0 << 0 << 0 << 0 << 0;
    pp55->add_rotation_grid(r_grid55);

    r_grid55 = new Rotation_grid<Board_content_type>(5,5);
    *r_grid55
      << 0 << 0 << 1 << 0 << 0
      << 0 << 0 << 1 << 0 << 0
      << 0 << 0 << 1 << 0 << 0
      << 0 << 0 << 1 << 0 << 0
      << 0 << 0 << 0 << 0 << 0;
    pp55->add_rotation_grid(r_grid55);

    r_grid55 = new Rotation_grid<Board_content_type>(5,5);
    *r_grid55
      << 0 << 0 << 0 << 0 << 0
      << 0 << 0 << 0 << 0 << 0
      << 0 << 1 << 1 << 1 << 1
      << 0 << 0 << 0 << 0 << 0
      << 0 << 0 << 0 << 0 << 0;
    pp55->add_rotation_grid(r_grid55);

    board_.add_piece(pp55);
  }

  // PIECE TYPE #2: (diag-left)
  {
    Puzzle_piece<Board_content_type>* pp33 = new Puzzle_piece<Board_content_type>(3,3);
// ##0  00#  000  0#0
// 0##  0##  ##0  ##0
// 000  0#0  0##  #00
    Rotation_grid<Board_content_type>* r_grid33 = new Rotation_grid<Board_content_type>(3,3);
    *r_grid33
      << 2 << 2 << 0
      << 0 << 2 << 2
      << 0 << 0 << 0;
    pp33->add_rotation_grid(r_grid33);

    r_grid33 = new Rotation_grid<Board_content_type>(3,3);
    *r_grid33
      << 0 << 0 << 2
      << 0 << 2 << 2
      << 0 << 2 << 0;
    pp33->add_rotation_grid(r_grid33);

    r_grid33 = new Rotation_grid<Board_content_type>(3,3);
    *r_grid33
      << 0 << 0 << 0
      << 2 << 2 << 0
      << 0 << 2 << 2;
    pp33->add_rotation_grid(r_grid33);

    r_grid33 = new Rotation_grid<Board_content_type>(3,3);
    *r_grid33
      << 0 << 2 << 0
      << 2 << 2 << 0
      << 2 << 0 << 0;
    pp33->add_rotation_grid(r_grid33);

    board_.add_piece(pp33);
  }

  // PEICE TYPE #3: (diag-right)
  {
    Puzzle_piece<Board_content_type>* pp33 = new Puzzle_piece<Board_content_type>(3,3);
// 0##  0#0  000  #00
// ##0  0##  0##  ##0
// 000  00#  ##0  0#0
    Rotation_grid<Board_content_type>* r_grid33 = new Rotation_grid<Board_content_type>(3,3);
    r_grid33 = new Rotation_grid<Board_content_type>(3,3);
    *r_grid33
      << 0 << 3 << 3
      << 3 << 3 << 0
      << 0 << 0 << 0;
    pp33->add_rotation_grid(r_grid33);

    r_grid33 = new Rotation_grid<Board_content_type>(3,3);
    *r_grid33
      << 0 << 3 << 0
      << 0 << 3 << 3
      << 0 << 0 << 3;
    pp33->add_rotation_grid(r_grid33);

    r_grid33 = new Rotation_grid<Board_content_type>(3,3);
    *r_grid33
      << 0 << 0 << 0
      << 0 << 3 << 3
      << 3 << 3 << 0;
    pp33->add_rotation_grid(r_grid33);

    r_grid33 = new Rotation_grid<Board_content_type>(3,3);
    *r_grid33
      << 3 << 0 << 0
      << 3 << 3 << 0
      << 0 << 3 << 0;
    pp33->add_rotation_grid(r_grid33);

    board_.add_piece(pp33);
  }

  // PEICE TYPE #4: (L-left)
  {
    Puzzle_piece<Board_content_type>* pp33 = new Puzzle_piece<Board_content_type>(3,3);
// 0#0  000  ##0  00#
// 0#0  ###  0#0  ###
// 0##  #00  0#0  000
    Rotation_grid<Board_content_type>* r_grid33 = new Rotation_grid<Board_content_type>(3,3);
    *r_grid33
      << 0 << 4 << 0
      << 0 << 4 << 0
      << 0 << 4 << 4;
    pp33->add_rotation_grid(r_grid33);

    r_grid33 = new Rotation_grid<Board_content_type>(3,3);
    *r_grid33
      << 0 << 0 << 0
      << 4 << 4 << 4
      << 4 << 0 << 0;
    pp33->add_rotation_grid(r_grid33);

    r_grid33 = new Rotation_grid<Board_content_type>(3,3);
    *r_grid33
      << 4 << 4 << 0
      << 0 << 4 << 0
      << 0 << 4 << 0;
    pp33->add_rotation_grid(r_grid33);

    r_grid33 = new Rotation_grid<Board_content_type>(3,3);
    *r_grid33
      << 0 << 0 << 4
      << 4 << 4 << 4
      << 0 << 0 << 0;
    pp33->add_rotation_grid(r_grid33);

    board_.add_piece(pp33);
  }

  // PEICE TYPE #5: (L-right)
  {
    Puzzle_piece<Board_content_type>* pp33 = new Puzzle_piece<Board_content_type>(3,3);
// 0#0  #00  0##  000
// 0#0  ###  0#0  ###
// ##0  000  0#0  00#
    Rotation_grid<Board_content_type>* r_grid33 = new Rotation_grid<Board_content_type>(3,3);
    *r_grid33
      << 0 << 5 << 0
      << 0 << 5 << 0
      << 5 << 5 << 0;
    pp33->add_rotation_grid(r_grid33);

    r_grid33 = new Rotation_grid<Board_content_type>(3,3);
    *r_grid33
      << 5 << 0 << 0
      << 5 << 5 << 5
      << 0 << 0 << 0;
    pp33->add_rotation_grid(r_grid33);

    r_grid33 = new Rotation_grid<Board_content_type>(3,3);
    *r_grid33
      << 0 << 5 << 5
      << 0 << 5 << 0
      << 0 << 5 << 0;
    pp33->add_rotation_grid(r_grid33);

    r_grid33 = new Rotation_grid<Board_content_type>(3,3);
    *r_grid33
      << 0 << 0 << 0
      << 5 << 5 << 5
      << 0 << 0 << 5;
    pp33->add_rotation_grid(r_grid33);

    board_.add_piece(pp33);
  }

  // PEICE TYPE #6: (square)
  {
    Puzzle_piece<Board_content_type>* pp22 = new Puzzle_piece<Board_content_type>(2,2);
// 11
// 11
    Rotation_grid<Board_content_type>* r_grid22 = new Rotation_grid<Board_content_type>(2,2);
    *r_grid22
      << 6 << 6
      << 6 << 6;
    pp22->add_rotation_grid(r_grid22);

    board_.add_piece(pp22);
  }

  // PEICE TYPE #7: (triangle)
  {
    Puzzle_piece<Board_content_type>* pp33 = new Puzzle_piece<Board_content_type>(3,3);
// 0#0  000  0#0  0#0
// 0##  ###  ##0  ###
// 0#0  0#0  0#0  000
    Rotation_grid<Board_content_type>* r_grid33 = new Rotation_grid<Board_content_type>(3,3);
    *r_grid33
      << 0 << 7 << 0
      << 0 << 7 << 7
      << 0 << 7 << 0;
    pp33->add_rotation_grid(r_grid33);

    r_grid33 = new Rotation_grid<Board_content_type>(3,3);
    *r_grid33
      << 0 << 0 << 0
      << 7 << 7 << 7
      << 0 << 7 << 0;
    pp33->add_rotation_grid(r_grid33);

    r_grid33 = new Rotation_grid<Board_content_type>(3,3);
    *r_grid33
      << 0 << 7 << 0
      << 7 << 7 << 0
      << 0 << 7 << 0;
    pp33->add_rotation_grid(r_grid33);

    r_grid33 = new Rotation_grid<Board_content_type>(3,3);
    *r_grid33
      << 0 << 7 << 0
      << 7 << 7 << 7
      << 0 << 0 << 0;
    pp33->add_rotation_grid(r_grid33);

    board_.add_piece(pp33);
  }

  // PIECE TYPE #8  exotic #1
  {
    Puzzle_piece<Board_content_type>* pp33 = new Puzzle_piece<Board_content_type>(3,3);
// 000  0##  #0#  0##
// ###  00#  ###  0#0
// #0#  0##  000  0##
    Rotation_grid<Board_content_type>* r_grid33 = new Rotation_grid<Board_content_type>(3,3);
    *r_grid33
      << 0 << 0 << 0
      << 2 << 3 << 4
      << 1 << 0 << 5;
    pp33->add_rotation_grid(r_grid33);

    r_grid33 = new Rotation_grid<Board_content_type>(3,3);
    *r_grid33
      << 1 << 2 << 0
      << 0 << 3 << 0
      << 5 << 4 << 0;
    pp33->add_rotation_grid(r_grid33);

    r_grid33 = new Rotation_grid<Board_content_type>(3,3);
    *r_grid33
      << 5 << 0 << 1
      << 4 << 3 << 2
      << 0 << 0 << 0;
    pp33->add_rotation_grid(r_grid33);

    r_grid33 = new Rotation_grid<Board_content_type>(3,3);
    *r_grid33
      << 2 << 1 << 0
      << 3 << 0 << 0
      << 4 << 5 << 0;
    pp33->add_rotation_grid(r_grid33);

    board_.add_piece(pp33);
  }

  // PEICE TYPE #9: exotic #2
  {
    Puzzle_piece<Board_content_type>* pp33 = new Puzzle_piece<Board_content_type>(3,3);
// #00  0##  #00  0##
// ###  0#0  ###  0#0
// 00#  ##0  00#  ##0
    Rotation_grid<Board_content_type>* r_grid33 = new Rotation_grid<Board_content_type>(3,3);
    *r_grid33
      << 1 << 0 << 0
      << 1 << 1 << 1
      << 0 << 0 << 1;
    pp33->add_rotation_grid(r_grid33);

    r_grid33 = new Rotation_grid<Board_content_type>(3,3);
    *r_grid33
      << 0 << 2 << 2
      << 0 << 2 << 0
      << 2 << 2 << 0;
    pp33->add_rotation_grid(r_grid33);

    r_grid33 = new Rotation_grid<Board_content_type>(3,3);
    *r_grid33
      << 3 << 0 << 0
      << 3 << 3 << 3
      << 0 << 0 << 3;
    pp33->add_rotation_grid(r_grid33);

    r_grid33 = new Rotation_grid<Board_content_type>(3,3);
    *r_grid33
      << 0 << 4 << 4
      << 0 << 4 << 0
      << 4 << 4 << 0;
    pp33->add_rotation_grid(r_grid33);

    board_.add_piece(pp33);
  }

  // PEICE TYPE #10: exotic #3
  {
    Puzzle_piece<Board_content_type>* pp33 = new Puzzle_piece<Board_content_type>(3,3);
// 000  0#0  0#0  000
// ##0  ##0  0##  0##
// 0#0  009  000  0#0
    Rotation_grid<Board_content_type>* r_grid33 = new Rotation_grid<Board_content_type>(3,3);
    *r_grid33
      << 0 << 0 << 0
      << 7 << 7 << 0
      << 0 << 7 << 0;
    pp33->add_rotation_grid(r_grid33);

    r_grid33 = new Rotation_grid<Board_content_type>(3,3);
    *r_grid33
      << 0 << 7 << 0
      << 7 << 7 << 0
      << 0 << 0 << 0;
    pp33->add_rotation_grid(r_grid33);

    r_grid33 = new Rotation_grid<Board_content_type>(3,3);
    *r_grid33
      << 0 << 7 << 0
      << 0 << 7 << 7
      << 0 << 0 << 0;
    pp33->add_rotation_grid(r_grid33);

    r_grid33 = new Rotation_grid<Board_content_type>(3,3);
    *r_grid33
      << 0 << 0 << 0
      << 0 << 7 << 7
      << 0 << 7 << 0;
    pp33->add_rotation_grid(r_grid33);

    board_.add_piece(pp33);
  }

  // PEICE TYPE #11: exotic #4
  {
    Puzzle_piece<Board_content_type>* pp33 = new Puzzle_piece<Board_content_type>(3,3);
// 000  000  000  000  000  000  #00  ##0  ###
// 0#0  0##  0##  0##  0##  ###  ###  ###  ###
// 000  000  00#  0##  ###  ###  ###  ###  ###
    Rotation_grid<Board_content_type>* r_grid33 = new Rotation_grid<Board_content_type>(3,3);
    *r_grid33
      << 0 << 0 << 0
      << 0 << 5 << 0
      << 0 << 0 << 0;
    pp33->add_rotation_grid(r_grid33);

    r_grid33 = new Rotation_grid<Board_content_type>(3,3);
    *r_grid33
      << 0 << 0 << 0
      << 0 << 5 << 5
      << 0 << 0 << 0;
    pp33->add_rotation_grid(r_grid33);

    r_grid33 = new Rotation_grid<Board_content_type>(3,3);
    *r_grid33
      << 0 << 0 << 0
      << 0 << 5 << 5
      << 0 << 0 << 5;
    pp33->add_rotation_grid(r_grid33);

    r_grid33 = new Rotation_grid<Board_content_type>(3,3);
    *r_grid33
      << 0 << 0 << 0
      << 0 << 5 << 5
      << 0 << 5 << 5;
    pp33->add_rotation_grid(r_grid33);

    r_grid33 = new Rotation_grid<Board_content_type>(3,3);
    *r_grid33
      << 0 << 0 << 0
      << 0 << 5 << 5
      << 0 << 5 << 5;
    pp33->add_rotation_grid(r_grid33);

    r_grid33 = new Rotation_grid<Board_content_type>(3,3);
    *r_grid33
      << 0 << 0 << 0
      << 0 << 5 << 5
      << 5 << 5 << 5;
    pp33->add_rotation_grid(r_grid33);

    r_grid33 = new Rotation_grid<Board_content_type>(3,3);
    *r_grid33
      << 0 << 0 << 0
      << 5 << 5 << 5
      << 5 << 5 << 5;
    pp33->add_rotation_grid(r_grid33);

    r_grid33 = new Rotation_grid<Board_content_type>(3,3);
    *r_grid33
      << 5 << 0 << 0
      << 5 << 5 << 5
      << 5 << 5 << 5;
    pp33->add_rotation_grid(r_grid33);

    r_grid33 = new Rotation_grid<Board_content_type>(3,3);
    *r_grid33
      << 5 << 5 << 0
      << 5 << 5 << 5
      << 5 << 5 << 5;
    pp33->add_rotation_grid(r_grid33);

    r_grid33 = new Rotation_grid<Board_content_type>(3,3);
    *r_grid33
      << 5 << 5 << 5
      << 5 << 5 << 5
      << 5 << 5 << 5;
    pp33->add_rotation_grid(r_grid33);

    board_.add_piece(pp33);
  }

  // PEICE TYPE #12: exotic #5
  {
    Puzzle_piece<Board_content_type>* pp44 = new Puzzle_piece<Board_content_type>(4,4);
// 000#  0000  0000  #000  0#00  00#0
// 00#0  00##  ##00  0#00  0#00  00#0
// 0#00  ##00  00##  00#0  00#0  0#00
// #000  0000  0000  000#  00#0  0#00
    Rotation_grid<Board_content_type>* r_grid44 = new Rotation_grid<Board_content_type>(4,4);
    *r_grid44
      << 0 << 0 << 0 << 1
      << 0 << 0 << 1 << 0
      << 0 << 1 << 0 << 0
      << 1 << 0 << 0 << 0;
    pp44->add_rotation_grid(r_grid44);

    r_grid44 = new Rotation_grid<Board_content_type>(4,4);
    *r_grid44
      << 0 << 0 << 0 << 0
      << 0 << 0 << 1 << 1
      << 1 << 1 << 0 << 0
      << 0 << 0 << 0 << 0;
    pp44->add_rotation_grid(r_grid44);

    r_grid44 = new Rotation_grid<Board_content_type>(4,4);
    *r_grid44
      << 0 << 0 << 0 << 0
      << 1 << 1 << 0 << 0
      << 0 << 0 << 1 << 1
      << 0 << 0 << 0 << 0;
    pp44->add_rotation_grid(r_grid44);

    r_grid44 = new Rotation_grid<Board_content_type>(4,4);
    *r_grid44
      << 1 << 0 << 0 << 0
      << 0 << 1 << 0 << 0
      << 0 << 0 << 1 << 0
      << 0 << 0 << 0 << 1;
    pp44->add_rotation_grid(r_grid44);

    r_grid44 = new Rotation_grid<Board_content_type>(4,4);
    *r_grid44
      << 0 << 1 << 0 << 0
      << 0 << 1 << 0 << 0
      << 0 << 0 << 1 << 0
      << 0 << 0 << 1 << 0;
    pp44->add_rotation_grid(r_grid44);

    r_grid44 = new Rotation_grid<Board_content_type>(4,4);
    *r_grid44
      << 0 << 0 << 1 << 0
      << 0 << 0 << 1 << 0
      << 0 << 1 << 0 << 0
      << 0 << 1 << 0 << 0;
    pp44->add_rotation_grid(r_grid44);

    board_.add_piece(pp44);
  }

}

}
