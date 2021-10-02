#ifndef PUZZLE_PIECE_H
#define PUZZLE_PIECE_H

#include <vector>  // container used to store the rotation grids associated with this puzzle piece

#include "Rotation_grid.h"  // used to define the rotations associated with a puzzle piece
#include "IPuzzle_piece.h"  // inherited interface used by Puzzle_pieces that go on the Playing_board

namespace Blockmania {

/*******************************************************************************
Steve Lewis - May 2006

A Puzzle_piece is essentially a collection of rotation grids that are used
on the Playing_board.  The Puzzle_piece itself implements some of the rules
of the Blockmania game, such as rules related to rotation.  Interfaces are
also provided so the Playing_board can implement translation rules of a
Puzzle_piece.

A template class is used since a Puzzle_piece can really be of any user defined
size and also consists of custom user defined rotations.

CT = Content Type, used in rotations (typically int or char)

R  = Number of rows for this puzzle piece  
C  = Number of cols for this puzzle piece

NOTE: R and C template parameters were removed, allowing puzzle pieces
to be resized dynamically at runtime rather than statically at compile time.
*******************************************************************************/
template<class CT>
class Puzzle_piece
:
  public IPuzzle_piece<CT>  // This class implements the IPuzzle_piece interface
{

public:
  
  typedef Rotation_grid<CT> Rotation_grid_type;

  Puzzle_piece(int nrow, int ncol);
  ~Puzzle_piece();

  // Add a rotation grid to this puzzle piece.  This puzzle piece becomes
  // the owner of the pointer (and therefore responsible to delete it during
  // the destructor)
  void add_rotation_grid(Rotation_grid_type* rotation_grid);

  // -- IPuzzle_piece interfaces -----------------------------------------------
  virtual int width() const { return ncols_; }
  virtual int height() const { return nrows_; }
  virtual int span() const;
  virtual int leftpad() const;
  virtual int rightpad() const;
  virtual int top_blank_rows() const;
  virtual int bottom_blank_rows() const;
  virtual const CT& operator()(int row, int col) const;
  virtual void reset_rotation();
  virtual bool rotate_left(int leftside_pad_remaining, int rightside_pad_remaining);
  virtual bool rotate_right(int leftside_pad_remaining, int rightside_pad_remaining);
  virtual void recalculate_helper_values();
  // ---------------------------------------------------------------------------

private:

  typedef std::vector< Rotation_grid_type* > Rotation_grids;

  int nrows_;
  int ncols_;

  typename Rotation_grids::size_type current_rotation_grid_index_;
  Rotation_grids rotation_grids_;

};

/*******************************************************************************
  Begin Puzzle_piece template class implementations
*******************************************************************************/
template<class CT>
Puzzle_piece<CT>::Puzzle_piece(int nrow, int ncol)
:
  current_rotation_grid_index_(-1),
  nrows_(nrow),
  ncols_(ncol)
{
}

template<class CT>
Puzzle_piece<CT>::~Puzzle_piece()
{
  Rotation_grids::iterator rotation_grid = rotation_grids_.begin();
  Rotation_grids::iterator rotation_grid_end = rotation_grids_.end();
  while (rotation_grid != rotation_grid_end)
  {
    delete *rotation_grid;
    ++rotation_grid;
  }
}

template<class CT>
void Puzzle_piece<CT>::add_rotation_grid(Rotation_grid_type* rotation_grid)
{
  assert(rotation_grid != 0);

  // Automatically recalculate the left and right pad values for the given
  // rotation_grid...
  rotation_grid->recalculate_pad_values();

  // Add the rotation grid to the collection of grids for this puzzle piece...
  rotation_grids_.push_back(rotation_grid);

  // If this is the first grid, then initialize the current_rotation_grid_index_
  // (so that it doesn't stay in the uninitialized state set in the constructor)
  if (rotation_grids_.size() == 1)
  {
    current_rotation_grid_index_ = 0;
  }
}

template<class CT>
int Puzzle_piece<CT>::span() const
{
  // Assume that at least one rotation grid has been added already
  assert( current_rotation_grid_index_ < rotation_grids_.size() );

  return (*rotation_grids_[current_rotation_grid_index_]).span();
}

template<class CT>
int Puzzle_piece<CT>::leftpad() const
{
  // Assume that at least one rotation grid has been added already
  assert( current_rotation_grid_index_ < rotation_grids_.size() );

  return (*rotation_grids_[current_rotation_grid_index_]).leftpad();
}

template<class CT>
int Puzzle_piece<CT>::rightpad() const
{
  // Assume that at least one rotation grid has been added already
  assert( current_rotation_grid_index_ < rotation_grids_.size() );

  return (*rotation_grids_[current_rotation_grid_index_]).rightpad();
}

template<class CT>
int Puzzle_piece<CT>::bottom_blank_rows() const
{
  // Assume that at least one rotation grid has been added already
  assert( current_rotation_grid_index_ < rotation_grids_.size() );

  return (*rotation_grids_[current_rotation_grid_index_]).bottom_blank_rows();
}

template<class CT>
int Puzzle_piece<CT>::top_blank_rows() const
{
  // Assume that at least one rotation grid has been added already
  assert( current_rotation_grid_index_ < rotation_grids_.size() );

  return (*rotation_grids_[current_rotation_grid_index_]).top_blank_rows();
}

template<class CT>
const CT& Puzzle_piece<CT>::operator()(int row, int col) const
{
  // Assume that at least one rotation grid has been added already
  assert( current_rotation_grid_index_ < rotation_grids_.size() );

  return (*rotation_grids_[current_rotation_grid_index_])(row, col);
}

template<class CT>
void Puzzle_piece<CT>::reset_rotation()
{
  // Assume that at least one rotation grid has been added already
  assert( current_rotation_grid_index_ < rotation_grids_.size() );

  current_rotation_grid_index_ = 0;
}

template<class CT>
bool Puzzle_piece<CT>::rotate_left(int leftside_pad_remaining, int rightside_pad_remaining)
{
  // Assume that at least one rotation grid has been added already
  assert( current_rotation_grid_index_ < rotation_grids_.size() );

  const Rotation_helper_values& rhv = (*rotation_grids_[current_rotation_grid_index_]).rotation_helper_values();
  if (
       (rightside_pad_remaining < rhv.rightside_rot_left)
    || (leftside_pad_remaining < rhv.leftside_rot_left)
  )
  {
    // There is insufficient space between the puzzle peice and the left or right
    // playing board walls to perform the rotation.  Report that the rotation
    // could not be performed.
    return false;
  }

  // Since doing a left rotation, decrease which rotation grid to use
  if (current_rotation_grid_index_ == 0)
  {
    // Index out of bounds, wrap to the last rotation grid...
    current_rotation_grid_index_ = rotation_grids_.size()-1;
  }
  else
  {
    --current_rotation_grid_index_;
  }

  // Report that the rotation was successful
  return true;
}

template<class CT>
bool Puzzle_piece<CT>::rotate_right(int leftside_pad_remaining, int rightside_pad_remaining)
{
  // Assume that at least one rotation grid has been added already
  assert( current_rotation_grid_index_ < rotation_grids_.size() );

  const Rotation_helper_values& rhv = (*rotation_grids_[current_rotation_grid_index_]).rotation_helper_values();
  if (
       (rightside_pad_remaining < rhv.rightside_rot_right)
    || (leftside_pad_remaining < rhv.leftside_rot_right)
  )
  {
    // There is insufficient space between the puzzle peice and the left or right
    // playing board walls to perform the rotation.  Report that the rotation
    // could not be performed.
    return false;
  }

  // Since doing a right rotation, increase which rotation grid to use
  ++current_rotation_grid_index_;
  if (current_rotation_grid_index_ >= rotation_grids_.size())
  {
    // Index out of bounds, wrap to the first rotation grid...
    current_rotation_grid_index_ = 0;
  }

  // Report that the rotation was successful
  return true;
}

template<class CT>
void Puzzle_piece<CT>::recalculate_helper_values()
{
  // Assume that at least one rotation grid has been added already
  assert( current_rotation_grid_index_ < rotation_grids_.size() );

  // If there are multiple rotation grids...
  if (rotation_grids_.size() > 1)
  {
    // Recalculate helper values for the first rotation grid (wrapping to
    // the ending as appropriate)
    (*rotation_grids_.begin())->recalculate_rotation_helper_values(
      *(*(rotation_grids_.end() - 1)),
      *(*(rotation_grids_.begin() + 1))
    );
    (*rotation_grids_.begin())->recalculate_bottom_blank_rows();
    (*rotation_grids_.begin())->recalculate_top_blank_rows();

    // Recalculate helper values for the last rotation grid (wrapping to
    // to beginning as appropriate)
    (*(rotation_grids_.end()-1))->recalculate_rotation_helper_values(
      *(*(rotation_grids_.end() - 2)),
      *(*(rotation_grids_.begin()))
    );
    (*(rotation_grids_.end()-1))->recalculate_bottom_blank_rows();
    (*(rotation_grids_.end()-1))->recalculate_top_blank_rows();

    // Recalculate helper values for the intermediate rotation grids
    Rotation_grids::iterator rotation_grid = rotation_grids_.begin() + 1;
    Rotation_grids::iterator rotation_grid_end = rotation_grids_.end() - 1;
    while (rotation_grid != rotation_grid_end)
    {
      Rotation_grid_type& rgt = *(*rotation_grid);
      rgt.recalculate_rotation_helper_values(
        *(*(rotation_grid - 1)),
        *(*(rotation_grid + 1))
      );
      rgt.recalculate_bottom_blank_rows();
      rgt.recalculate_top_blank_rows();
      ++rotation_grid;
    }
  }
  else
  {
    (*rotation_grids_.begin())->recalculate_bottom_blank_rows();
    (*rotation_grids_.begin())->recalculate_top_blank_rows();
  }
}

}

#endif

