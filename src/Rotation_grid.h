#ifndef ROTATION_GRID_H
#define ROTATION_GRID_H

#include <cassert>

#include "Rotation_helper_values.h"

namespace Blockmania {

/*******************************************************************************
  The Rotation_grid is used to define user-defined puzzle piece shapes.  These
  shapes can be customized into a variety of patterns to make for more interesting
  Blockmania games.  A Puzzle_piece is really a collection of rotations, since
  per the nature of the Blockmania gameplay, pieces can be rotated left and
  right.  Rotating right really just means incrementing to the next rotation pattern,
  and rotating left means decrementing to the previous rotation pattern.

  Each rotation is simply a two dimensional array of cells, or "content".  The
  Content_type (CT) template parameter is used to specify the type associated
  with a cell.  This type is typically an int or a char, and allows the user
  the flexibility to designate how they want to specify cells.  The value of a
  cell can be used to indicate how that cell should be rendered in a UI.

  The following is an example of a Rotation_grid and its content, and what
  the associate helper values mean:

  000000000  Width == 9        (the number of columns for the entire rotation grid)
  000110000  Left Pad == 1     (number of empty columns on the left side of the contained shape)
  011111000  Right Pad == 2    (number of empty columns on the right side of the contained shape)
  000111100  Span == 6         (length of the shape itself within the grid)
  000000000  Bottom Blank Rows == 1

  These helper values are used when moving and rotating the puzzle piece within
  the Blockmania playing board (without having to continuously recompute these values).

  The rotation helper values can only be set when given the next and previous
  rotation relative to the current instance.  That is, their values are relative
  to how the rotation grid is changed when rotating left or right.  See the
  Rotation_helper_values struct for more information.

Template Arguments
------------------
CT = Content Type (typically int or char)
R  = Rows (should match the number of rows in the owning Puzzle_piece)
C  = Cols (should match the number of cols in the owning Puzzle_piece)

*******************************************************************************/
template<class CT>
class Rotation_grid
{

public:

  static const CT empty = 0;

  Rotation_grid(int nrow, int ncol);

  // Use operator<< to fill in the contents of this rotation grid (fills by row)
  Rotation_grid<CT>& operator<<(const CT& content);

  // Use operator() to access the contents at the specified row and col
  inline const CT& operator()(int row, int col) const { return content_[row][col]; }

  void recalculate_pad_values();

  void recalculate_rotation_helper_values(const Rotation_grid<CT>& prev, const Rotation_grid<CT>& next);
  void recalculate_bottom_blank_rows();
  void recalculate_top_blank_rows();  

  // Clear contents of this rotation grid.  This is used for: (1) on instantiation
  // of rotation grid instances, (2) to reset the operator<< processing, (3) support
  // reuse of local instantiations.
  void clear_contents();

  inline int leftpad() const { return leftpad_; }
  inline int rightpad() const { return rightpad_; }
  inline int span() const { return span_; }
  inline int bottom_blank_rows() const { return rotation_helper_values_.bottom_blank_rows; }
  inline int top_blank_rows() const { return rotation_helper_values_.top_blank_rows; }

  // External accessor to the Rotation Helper Values
  inline const Rotation_helper_values& rotation_helper_values() const { return rotation_helper_values_; }

private:

  // Actual storage of the puzzle piece, as represented during this rotation.
  static const int max_rows = 20;
  static const int max_cols = 20;
  CT content_[max_rows][max_cols];

  int nrows_;
  int ncols_;

  // These index attributes are used by the operator<< method to know
  // which content_ entry to fill.
  int crow_index_;
  int ccol_index_;

  // Left pad is the number of empty columns on the left side of this rotation grid
  int leftpad_;

  // Right pad is the number of empty columns on the right side of this rotation grid
  int rightpad_;

  // Physical span of the puzzle piece, as represented in this rotation.
  int span_;

  Rotation_helper_values rotation_helper_values_;

};

/*******************************************************************************
  Begin Rotation_grid template class implementations
*******************************************************************************/
template<class CT>
Rotation_grid<CT>::Rotation_grid(int nrows, int ncols)
:
  nrows_(nrows),
  ncols_(ncols)
{
  assert((nrows < max_rows) && (ncols < max_cols));

  clear_contents();
}

template<class CT>
Rotation_grid<CT>& Rotation_grid<CT>::operator<<(const CT& content)
{
  // If the following assert fails, then the user has attempted to insert
  // to much content into the current rotation grid.
  assert( crow_index_ < nrows_ );

  // Set the given content at the current row and col index position
  content_[crow_index_][ccol_index_] = content;

  // Increment to the next column in the current row
  ++ccol_index_;

  // If we have completed a row...
  if (ccol_index_ == ncols_)
  {
    // Increment to the next row..
    ++crow_index_;
    ccol_index_ = 0;
  }

  return *this;
}

template<class CT>
void Rotation_grid<CT>::recalculate_pad_values()
{
  assert((nrows_ > 0) && (ncols_ > 0));

// NOTE: the leftpad and rightpad inspection could be done in parallel.  But
// they can not be done within the same column for loop, because one or the
// other may abort first.

  // Initially assume that the left pad space is 0
  leftpad_ = 0;
  // Check each column, starting on the left...
  for (int c = 0; c < ncols_; ++c)
  {
    int r = 0;
    for (; r < nrows_; ++r)
    {
      // If we find any content in this row, abort the row loop
      if (content_[r][c] != empty)
      {
        break;
      }
    }
    // If we aborted the row loop early, we found content and are done
    // calculating the leftpad
    if (r < nrows_)
    {
      break;
    }
    // Else, increment the left pad and go to the next column
    ++leftpad_;
  }

  // Initially assume that the right pad space is 0
  rightpad_ = 0;
  // Check each column, starting on the right...
  for (int c = ncols_-1; c >= 0; --c)
  {
    int r = 0;
    for (; r < nrows_; ++r)
    {
       // If we find any content in this row, abort the row loop
      if (content_[r][c] != empty)
      {
        break;
      }
    }
    // If we aborted the row loop early, we found content and are done
    // calculating the rightpad
    if (r < nrows_)
    {
      break;
    }
    // Else, increment the right pad and go to the next column
    ++rightpad_;
  }

  // Also calculate the span (which is the total column width minus the
  // amount of pad on the left and right)
  span_ = ncols_ - rightpad_ - leftpad_;
}

template<class CT>
void Rotation_grid<CT>::recalculate_rotation_helper_values(const Rotation_grid<CT>& prev, const Rotation_grid<CT>& next)
{
  // The number of columns gained or lost by rotating left (prev)
  rotation_helper_values_.leftside_rot_left = this->leftpad_ - prev.leftpad_;
  rotation_helper_values_.rightside_rot_left = this->rightpad_ - prev.rightpad_;

  // The number of columns gained or lost by rotating right (next)
  rotation_helper_values_.leftside_rot_right = this->leftpad_ - next.leftpad_;
  rotation_helper_values_.rightside_rot_right = this->rightpad_ - next.rightpad_;
}

template<class CT>
void Rotation_grid<CT>::recalculate_bottom_blank_rows()
{
  // Initially assume that there are 0 bottom blank rows for this rotation grid...
  rotation_helper_values_.bottom_blank_rows = 0;
  // Start inspecting each row, starting from the bottom...
  for (int r = nrows_-1; r >= 0; --r)
  {
    // Scan the current column...
    int c = 0;
    for (; c < ncols_; ++c)
    {
      // If we find any content in this column, abort the column loop
      if (content_[r][c] != empty)
      {
        break;
      }
    }
    // If we aborted the column loop early, we found content and are done
    // calculating the bottom blank rows
    if (c < ncols_)
    {
      break;
    }
    // Else, increment the bottom blank row count and proceed to the row up
    ++rotation_helper_values_.bottom_blank_rows;
  }
}

template<class CT>
void Rotation_grid<CT>::recalculate_top_blank_rows()
{
  // Initially assume that there are 0 bottom blank rows for this rotation grid...
  rotation_helper_values_.top_blank_rows = 0;
  // Start inspecting each row, starting from the top...
  for (int r = 0; r < nrows_; ++r)
  {
    // Scan the current column...
    int c = 0;
    for (; c < ncols_; ++c)
    {
      // If we find any content in this column, abort the column loop
      if (content_[r][c] != empty)
      {
        break;
      }
    }
    // If we aborted the column loop early, we found content and are done
    // calculating the bottom blank rows
    if (c < ncols_)
    {
      break;
    }
    // Else, increment the bottom blank row count and proceed to the row up
    ++rotation_helper_values_.top_blank_rows;
  }
}

template<class CT>
void Rotation_grid<CT>::clear_contents()
{
  leftpad_ = 0;
  rightpad_ = 0;
  span_ = 0;
  crow_index_ = 0;
  ccol_index_ = 0;
  rotation_helper_values_.clear();
  for(int r = 0; r < nrows_; ++r)
  {
    for (int c = 0; c < ncols_; ++c)
    {
      content_[r][c] = empty;
    }
  }
}

}

#endif
