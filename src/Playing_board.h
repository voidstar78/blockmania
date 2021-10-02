#ifndef BLOCKMANIA_PLAYING_BOARD_H
#define BLOCKMANIA_PLAYING_BOARD_H

#include <cassert>  // used to assertion testing during debug builds
#include <vector>  // container used to store pointers to instantiated puzzle pieces
#include "IPuzzle_piece.h"  // interface class so we can operate on puzzle pieces abstractly

namespace Blockmania {

/*******************************************************************************
Steve Lewis - May 2006

The Playing_board is used to instantiate a specific Blockmania playing board,
which has a specific size (R = rows, C = columns) and Content Type (CT) for the
content within the playing on board.

The Playing_board provides interfaces the enforces the game rules of Blockmania.
The user is responsible for calling these interfaces in the proper sequence,
as appropriate for the front end application (i.e. GUI).

Template Argument Summary
-------------------------
CT     Content Type.  This is typically int or char, but can be a user defined
       type.  Whatever the type is, it must be able to be set equal to 0.  So,
       if a user defined type is used, it will likely need an operator==(int).
       When set to 0, the semantics is that the content is reset, cleared, or
       otherwise empty.
R      Number of rows in the board (typical value is 20)
C      Number of columns in the board (typical value is 10, or half the number
       of rows)
*******************************************************************************/
template<class CT>
class Playing_board
{

public:

  // Helper typedef for users to access the CT type passed to this template class.
  typedef CT Content_type;

  // "empty" represents an empty cell on the playing board
  static const CT empty = 0;

  // Max rows, cols of the playing board
  static const int max_rows = 40;
  static const int max_cols = 30;

  // Return type used for rows_removed() call (which should only be called
  // after a call to remove_completed_rows())
  typedef bool* Rows_removed;

  // Container type used to hold puzzle piece interface pointers.
  typedef std::vector< IPuzzle_piece<Content_type>* > Puzzle_pieces;

  // Constructor -- initializes the content of the board to empty.
  Playing_board(int nrows, int ncols);

  // Destructor -- deallocates each of the Puzzle_pieces added to this
  // playing board instance
  ~Playing_board();

  // Add the new_puzzle_piece to the Playing_boards' collection of pieces.
  // NOTE: This instance of Playing_board becomes the owner of the passed pointer,
  // and responsible for its deallocation.
  void add_piece(IPuzzle_piece<Content_type>* new_puzzle_piece);

  // Implement the game rules for dropping the current piece lower on the
  // playing board.  Returns TRUE if the piece could not be dropped (i.e. blocked
  // by an existing piece), otherwise returns FALSE if the piece is blocked
  // and unable to drop.  If "increment_row" is true, the piece will be lowered
  // (i.e. dropped) on the board if there is room.  Passing increment_row as
  // false is useful for detecting end game.
  bool drop_piece_failed(bool increment_row);

  // Per rules of Blockmania, if a piece can no longer drop, it becomes a
  // part of the board itself.  This method does that: the current piece is
  // stuck (or pasted) to the board and becomes a part of its content.  This
  // is typically called (by the user) when drop_piece_failed() returns true.
  void stick_piece();

  // start_new_piece allows a new piece to become the current piece, and centers
  // that pieces' location to the top of the playing board.  The "piece_index"
  // is used to select which piece is started.  Its value should be from 0
  // to number_of_puzzle_piece_types()-1 (inclusive).
  void start_new_piece(typename Puzzle_pieces::size_type piece_index);

  // Rotates the current piece to the right.  Returns FALSE if the rotation
  // fails (e.g. due to collision).
  bool rotate_current_piece_right();

  // Rotates the current piece to the left.  Returns FALSE if the rotation
  // fails (e.g. due to collision).
  bool rotate_current_piece_left();

  // Moves the current piece to the right.  Returns FALSE if the movement
  // fails (e.g. due to collision).
  bool move_current_piece_right();

  // Moves the current piece to the left.  Returns FALSE if the movement
  // fails (e.g. due to collision).
  bool move_current_piece_left();

  // This method scans the playing board from bottom to top for "completed
  // rows" (any row those contents in each column is not empty).  These rows
  // are removed, and the board content adjusted per rules of Blockmania.
  // This method returns the total number of rows that were removed.  Also,
  // this method populates the "rows_removed" boolean pointer array.  Therefore,
  // after calling this method, to determine which specific rows were removed,
  // you can use the rows_removed() accessor method (this is useful for user
  // side effects, e.g. GUI effects or playing sound).
  int remove_completed_rows();

  // Returns the number of puzzle piece types that have been added to this
  // instance of a playing board.
  inline typename Puzzle_pieces::size_type number_of_puzzle_piece_types() { return puzzle_pieces_.size(); }

  // Returns the contents of the board at the given row and col position.  0,0
  // is the top left of the board.  row must be between 0 and rows()-1 (inclusive),
  // and col must be between 0 and cols()-1 (inclusive).  This method returns
  // the contents of the board, as overlayed by the current puzzle piece.
  const Content_type& operator()(int row, int col) const;

  // Return the number of rows in this playing board instance.
  inline int rows() const { return nrows_; }

  // Return the number of cols in this playing board instance.
  inline int cols() const { return ncols_; }

  // This method is used after a call to remove_completed_rows() to query
  // which rows were removed.  The return type is an array of booleans,
  // where the first entry of the array corresponds to row 0 (the top row),
  // and so on.  A value of TRUE indicates that the row was removed, otherwise
  // the value is FALSE.  e.g.  if there are only six rows, then values of
  // {FALSE, FALSE, TRUE, FALSE, TRUE, TRUE} would indicate that rows 2, 4, 5
  // were removed (and from this the user could determine that 2 was the most
  // number of consecutive rows removed).
  Rows_removed rows_removed() const { return rows_removed_; }

private:

  // Number of rows on the playing board
  int nrows_;

  // Number of cols on the playing board
  int ncols_;

  // Users current row on the board.
  int current_row_;

  // Users current col on the board.
  int current_col_;

  // Set during call to remove_completed_rows(), used as return value to
  // rows_removed() so user can determine which rows were removed (if any)
  bool* rows_removed_;

  // Instance of container of pointers to the puzzle piece types.
  Puzzle_pieces puzzle_pieces_;

  // Iterator to the current puzzle piece being dropped on the board.
  typename Puzzle_pieces::const_iterator current_piece_;

  // Concrete content of the playing board itself.
  Content_type content_[max_rows][max_cols];

};

/*******************************************************************************
  Begin Playing_board template class implementations
*******************************************************************************/
template<class CT>
Playing_board<CT>::Playing_board(int nrows, int ncols)
:
  current_piece_(puzzle_pieces_.end()),
  nrows_(nrows),
  ncols_(ncols),
//  content_memory_(new Content_type[nrow*ncol]),
//  content_(new Content_type[nrow*ncol]),  //content_memory_),
//  content_(new Content_type[nrow][ncol]),
  rows_removed_(new bool[nrows])
{
  assert((nrows < max_rows) && (ncols < max_cols));
  // Clear the contents of the playing board
  for(int r = 0; r < nrows_; ++r)
  {
    for (int c = 0; c < ncols_; ++c)
    {
      content_[r][c] = empty;
    }
  }
}

template<class CT>
Playing_board<CT>::~Playing_board()
{
  // Iterate across the puzzle pieces and delete them from memory
  Puzzle_pieces::iterator puzzle_piece = puzzle_pieces_.begin();
  Puzzle_pieces::iterator puzzle_piece_end = puzzle_pieces_.end();
  while (puzzle_piece != puzzle_piece_end)
  {
    delete *puzzle_piece;
    ++puzzle_piece;
  }
  delete rows_removed_;
}

template<class CT>
void Playing_board<CT>::add_piece(IPuzzle_piece<Content_type>* new_puzzle_piece)
{
  assert(new_puzzle_piece != 0);

  // Automatically recalculate the helper values for the new puzzle piece
  // (assumes all desired rotation grids have already been added to the piece)
  new_puzzle_piece->recalculate_helper_values();

  // Add the puzzle piece to the playing boards collection of pieces
  puzzle_pieces_.push_back(new_puzzle_piece);

  // If this is the first puzzle piece, then initialize current_piece_ so that
  // it doesn't stay in the uninitialized state of the constructor
  if (puzzle_pieces_.size() == 1)
  {
    current_piece_ = puzzle_pieces_.begin();
  }
}

template<class CT>
bool Playing_board<CT>::drop_piece_failed(bool increment_row)
{
  // Assume at least one piece has been added (and so puzzle_pieces_ is initialized)
  assert(puzzle_pieces_.size() > 0);

  IPuzzle_piece<Content_type>& cpp = *(*current_piece_);

  // Have we reached the bottom of the board?
  int maxR = nrows_ - (cpp.height() - cpp.bottom_blank_rows());
  if (current_row_ >= maxR)
  {
    return true;  // collide with board bottom
  }
  // If we haven't reached the playing boards bottom yet, check to see if we
  // collide with an existing stuck piece...
  // -- for each row in the puzzle piece (starting with the bottom)...
  for (int r = cpp.height()-1; r >= 0; --r)
  {
    // Scan each column in the puzzle piece...
    for (int c = 0; c < cpp.width(); ++c)
    {
      // If the puzzle pieces' rotation grid is not empty at this r,c...
      if (cpp(r,c) != empty)
      {
        // See of the playing board content at the row below in this column
        // is empty...
        if (content_[current_row_+r+1][current_col_+c] != empty)
        {
          // If it is not empty, then the puzzle piece can not drop any further
          // because it collides (and so its row can not be incremented)
          return true;
        }
      }
    }
  }
  // If user wants to move the puzzle piece to the next row if we don't collide,
  // then do so here...
  if (increment_row)
  {
    ++current_row_;
  }
  return false;
}

template<class CT>
void Playing_board<CT>::stick_piece()
{
  // Assume at least one piece has been added (and so puzzle_pieces_ is initialized)
  assert(puzzle_pieces_.size() > 0);

  IPuzzle_piece<Content_type>& cpp = *(*current_piece_);
  for (int r = 0; r < cpp.height(); ++r)
  {
    for (int c = 0; c < cpp.width(); ++c)
    {
      Content_type content = cpp(r,c);
      if (content != empty)
      {
        content_[current_row_+r][current_col_+c] = content;
      }
    }
  }
}

template<class CT>
void Playing_board<CT>::start_new_piece(typename Puzzle_pieces::size_type piece_index)
{
  // Assume at least one piece has been added (and so puzzle_pieces_ is initialized)
  assert(puzzle_pieces_.size() > 0);
  assert(piece_index < puzzle_pieces_.size());

  //current_piece_ = &puzzle_pieces_[piece_index];  // WORKS WITH BCB6 (STLport) but not VS2005 STL
  current_piece_ = puzzle_pieces_.begin();
  while (piece_index > 0)
  {
    ++current_piece_;
    --piece_index;
  }

  IPuzzle_piece<Content_type>& cpp = *(*current_piece_);
  cpp.reset_rotation();

  current_col_ = (ncols_ - cpp.width()) / 2;
  current_row_ = 0;
}

template<class CT>
bool Playing_board<CT>::rotate_current_piece_right()
{
  // Assume at least one piece has been added (and so puzzle_pieces_ is initialized)
  assert(puzzle_pieces_.size() > 0);

  IPuzzle_piece<Content_type>& cpp = *(*current_piece_);

  // Calculate left and right side pad remaining...
  int left_side_pad_remaining = current_col_ + cpp.leftpad();
  int right_side_pad_remaining = ncols_ - left_side_pad_remaining - cpp.span();

  if (cpp.rotate_right(left_side_pad_remaining, right_side_pad_remaining))
  {
    // Collision Detection: does rotation collide with existing stick pieces?
    int maxR = current_row_ + cpp.height();
    if (maxR > nrows_)
    {
      maxR = nrows_;
    }
    int maxC = current_col_ + cpp.width();
    if (maxC > ncols_)
    {
      maxC = ncols_;
    }
    for (int r = current_row_; r < maxR; ++r)
    {
      for (int c = current_col_; c < maxC; ++c)
      {
        if (cpp(r - current_row_, c - current_col_) != empty)
        {
          if (content_[r][c] != empty)
          {
            // Collision detected, rotate the piece back to the left
            cpp.rotate_left(left_side_pad_remaining, right_side_pad_remaining);
            return false;
          }
        }
      }
    }
    // No collision detected
    return true;
  }
  // Wall collision detected
  return false;
}

template<class CT>
bool Playing_board<CT>::rotate_current_piece_left()
{
  // Assume at least one piece has been added (and so puzzle_pieces_ is initialized)
  assert(puzzle_pieces_.size() > 0);

  IPuzzle_piece<Content_type>& cpp = *(*current_piece_);

  int left_side_pad_remaining = current_col_ + cpp.leftpad();
  int right_side_pad_remaining = ncols_ - left_side_pad_remaining - cpp.span();

  if (cpp.rotate_left(left_side_pad_remaining, right_side_pad_remaining))
  {
    // Collision Detection
    int maxR = current_row_ + cpp.height();
    if (maxR >= nrows_)
    {
      maxR = nrows_;
    }
    int maxC = current_col_ + cpp.width();
    if (maxC >= ncols_)
    {
      maxC = ncols_;
    }
    for (int r = current_row_; r < maxR; ++r)
    {
      for (int c = current_col_; c < maxC; ++c)
      {
        if (cpp(r - current_row_, c - current_col_) != empty)
        {
          if (content_[r][c] != empty)
          {
            // Collision detected, rotate the piece back to the right
            cpp.rotate_right(left_side_pad_remaining, right_side_pad_remaining);
            return false;
          }
        }
      }
    }
    // No collision detected
    return true;
  }
  // Wall collision detected
  return false;
}

template<class CT>
bool Playing_board<CT>::move_current_piece_right()
{
  // Assume at least one piece has been added (and so puzzle_pieces_ is initialized)
  assert(puzzle_pieces_.size() > 0);

  IPuzzle_piece<Content_type>& cpp = *(*current_piece_);

  // Perform the move (assume no collision)
  // Wall Collision Detection: did moving the piece collide with the right side wall?
  int maxC = ncols_ - cpp.span() - cpp.leftpad();
  if ((current_col_+1) > maxC)
  {
    // Yes, move the piece back and report the wall collision
    return false;
  }

  // Existing Content Collision Detection: will moving the piece to the right
  // collide with an existing stuck piece?
  int maxR = current_row_ + cpp.height();
  if (maxR > nrows_)
  {
    maxR = nrows_;
  }
  maxC = current_col_ + cpp.width();
  if (maxC > ncols_)
  {
    maxC = ncols_;
  }
  for (int r = current_row_; r < maxR; ++r)
  {
    for (int c = current_col_; c < maxC; ++c)
    {
      if (
           (cpp(r-current_row_,c-current_col_) != empty)
        && (content_[r][c+1] != empty)
      )
      {
        // Yes, report the collision
        return false;
      }
    }
  }
  // No collision detected, move and return true
  ++current_col_;
  return true;
}

template<class CT>
bool Playing_board<CT>::move_current_piece_left()
{
  // Assume at least one piece has been added (and so puzzle_pieces_ is initialized)
  assert(puzzle_pieces_.size() > 0);

  IPuzzle_piece<Content_type>& cpp = *(*current_piece_);

  // Wall Collision Detection: will moving the piece collide with the left side wall?
  int minC = 0 - cpp.leftpad();
  if ((current_col_-1) < minC)
  {
    // Yes, move the piece back and report the wall collision
    return false;
  }

  // Existing Content Collision Detection: will moving the piece to the left
  // collide with an existing stuck piece?
  int maxR = current_row_ + cpp.height();
  if (maxR > nrows_)
  {
    maxR = nrows_;
  }
  int maxC = current_col_ + cpp.width();
  if (maxC > ncols_)
  {
    maxC = ncols_;
  }
  for (int r = current_row_; r < maxR; ++r)
  {
    for (int c = current_col_; c < maxC; ++c)
    {
      if (
           (cpp(r-current_row_,c-current_col_) != empty )
        && (content_[r][c-1] != empty)
      )
      {
        // Yes, report the collision
        return false;
      }
    }
  }
  // No collision detected, move and return true
  --current_col_;
  return true;
}

template<class CT>
int Playing_board<CT>::remove_completed_rows()
{
  // Clear all the row removed flags (assume no row is currently complete)
  for (int i = 0; i < nrows_; ++i)
  {
    rows_removed_[i] = false;
  }
  // Clear the rows removed counter
  int rows_removed = 0;

  // Scan each of the rows...
  for (int r = 0; r < nrows_; ++r)
  {
    // Scan each column in the current row
    int c = 0;
    for (; c < ncols_; ++c)
    {
      // If any column is empty, then the row is not complete (abort the column
      // loop early)
      if (content_[r][c] == empty)
      {
        break;
      }
    }
    // If the column loop was not aborted early, then the row is complete
    if (c == ncols_)
    {
      rows_removed_[r] = true;
      ++rows_removed;
    }
  }

  // If the first row is complete, just clear it...
  if (rows_removed_[0])
  {
    for (int c = 0; c < ncols_; ++c)
    {
      content_[0][c] = empty;
    }
  }
  // Scan each of the remaining rows...
  for (int r = 1; r < nrows_; ++r)
  {
    // If the row is completed...
    if (rows_removed_[r])
    {
      // Copy all the previous rows down one
      for (int cr = r-1; cr > 0; --cr)
      {
        for (int c = 0; c < ncols_; ++c)
        {
          content_[cr+1][c] = content_[cr][c];
        }
      }
    }
  }
  return rows_removed;
}

template<class CT>
const typename Playing_board<CT>::Content_type& Playing_board<CT>::operator()(int row, int col) const
{
  // Assume at least one piece has been added (and so puzzle_pieces_ is initialized)
  assert(puzzle_pieces_.size() > 0);
  assert( (row >= 0) && (row < nrows_) );
  assert( (col >= 0) && (col < ncols_) );

  const IPuzzle_piece<Content_type>& cpp = *(*current_piece_);

  // If the row, col being requested is within the region of the current puzzle piece...
  if (
    ( (row >= current_row_) && (row < ( current_row_ + cpp.height()) ) )
    && ( (col >= current_col_) && (col < ( current_col_ + cpp.width() ) ) )
  )
  {
    // And there is some content in the pieces' current rotation at this row, col...
    const Content_type& content = cpp( row - current_row_ , col - current_col_ );
    if (content != empty)
    {
      // Return the puzzle piece content...
      return cpp( row - current_row_ , col - current_col_ );
    }
  }
  // Otherwise, just return the playing board content...
  return content_[row][col];
}

}

#endif
