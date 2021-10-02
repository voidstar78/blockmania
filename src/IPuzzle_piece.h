#ifndef IPUZZLE_PIECE_H
#define IPUZZLE_PIECE_H

namespace Blockmania {

// CT = Content Type
template<class CT>
class IPuzzle_piece
{

public:

  // Width of the puzzle piece (in total content cells)
  virtual int width() const = 0;

  // Height of the puzzle piece (in total content cells)
  virtual int height() const = 0;

  // Span of the puzzle pieces' current rotation (typically at least one,
  // also represents value of width() minus left and right pad space).
  virtual int span() const = 0;

  // Left pad space for the puzzle pieces' current rotation (number of unused
  // columns on the left side of the puzzle shape)
  virtual int leftpad() const = 0;

  // Right pad space for the puzzle pieces' current rotation (number of unused
  // columns on the eirght side of the puzzle shape)
  virtual int rightpad() const = 0;

  // Number of blank rows at the bottom of the puzzle pieces' current rotation.
  // Used to determine how many rows down the piece can fall before hitting the
  // "bottom"
  virtual int bottom_blank_rows() const = 0;

  // Number of blank rows at the top of the puzzle pieces' current rotation.
  // Used to determine how many rows down the piece can fall before hitting the
  // "bottom"
  virtual int top_blank_rows() const = 0;

  // Accessor to the contents of the puzzle pieces' current rotation (row is from
  // 0 to width()-1 inclusive, col is from 0 to height()-1 inclusive)
  virtual const CT& operator()(int row, int col) const = 0;

  // Reset the puzzle pieces' current rotation to the initial rotation.  This
  // should be called after a player has dropped a puzzle piece (otherwise, the
  // next time the piece is selected, it will appear in the same rotation as
  // the user dropped this puzzle type the previous time it was selected)
  virtual void reset_rotation() = 0;

  // Rotate the puzzle piece to the left.  leftside_pad_remaining is the number
  // of columns between the shape and the left wall of the playing board.
  // rightside_pad_remaining is the number of columns between the shape and the
  // right wall of the playing board.  These are used to determine if the
  // shape can be rotated or not.
  virtual bool rotate_left(int leftside_pad_remaining, int rightside_pad_remaining) = 0;

  // Rotate the puzzle piece to the right.  leftside_pad_remaining is the number
  // of columns between the shape and the left wall of the playing board.
  // rightside_pad_remaining is the number of columns between the shape and the
  // right wall of the playing board.  These are used to determine if the
  // shape can be rotated or not.
  virtual bool rotate_right(int leftside_pad_remaining, int rightside_pad_remaining) = 0;

  // After all the rotations for a puzzle piece type have been added, this method
  // should be called to update the helper values used during rotation. Otherwise,
  // these values would need to be recalculated continuously whenever a puzzle
  // piece is rotated.  This interface is called automatically when a puzzle piece
  // is added to a playing board (since it is assumed that all of its rotation
  // grid have already been added -- if not, this interface can be called manually).
  virtual void recalculate_helper_values() = 0;

};

}

#endif