#ifndef ROTATION_HELPER_VALUES_H
#define ROTATION_HELPER_VALUES_H

namespace Blockmania {

// All of these values are stored in a single struct, so they can be passed
// as a single argument, rather than separate accessor/mutator for each value.
struct Rotation_helper_values
{
// NOTE: All of these values are calculated automatically during a call to
// the Rotation_grid's recalculate_rotation_helper_values() method.

  // These values reflect the following information: if the associated Rotation_grid
  // were rotated right, how much column space would be lost (or gained, if negative)
  // on the left and right side of the puzzle pieces' local space.  These means that
  // in order to be rotated right, there must be at least this much space available
  // on the left and right side of the puzzle piece and the edges of the playing board.
  int leftside_rot_right;
  int rightside_rot_right;
  int leftside_rot_left;
  int rightside_rot_left;

  // The bottom_blank_rows value is not actually used to help with rotations,
  // but is used to help determine how far down a puzzle piece can drop before
  // it physically hits the bottom of the playing board.
  int bottom_blank_rows;
  int top_blank_rows;

  // ---------------------------------------------------------------------------
  // Utility method to initialize member attributes
  void clear()
  {
    leftside_rot_right = 0;
    rightside_rot_right = 0;
    leftside_rot_left = 0;
    rightside_rot_left = 0;
    top_blank_rows = 0;
    bottom_blank_rows = 0;
  }
};

}

#endif