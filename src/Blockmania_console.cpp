#include "Board_manager.h"
#include <iostream>

int main(int argc, char** argv)
{
  using namespace Blockmania;

  Board_manager board_manager;

  Board_manager::Board& board = board_manager.board();
  board.start_new_piece(0);

  while (true)
  {
    for (int r = 0; r < board.rows(); ++r)
    {
      for (int c = 0; c < board.cols(); ++c)
      {
        Board_manager::Board::Content_type board_content = board(r, c);
        std::cout << '[' << board_content << ']';
      }
      std::cout << std::endl;
    }
    
    char c;
    std::cin >> c;
    switch (c)
    {
      case 'Q':
      case 'q':
        board.move_current_piece_left();
        break;
      case 'W':
      case 'w':
        board.move_current_piece_right();
        break;
      case 'D':
      case 'd':
        if (board.drop_piece_failed(true))
        {
          board.stick_piece();
          board.remove_completed_rows();
          static unsigned int next_piece = 0;
          board.start_new_piece(next_piece);
        //  ++next_piece;
          if (next_piece >= board.number_of_puzzle_piece_types())
          {
            next_piece = 0;
          }
          if (board.drop_piece_failed(false))
          {
            std::cout << "!!! GAME OVER !!!\n";
            return 0;
          }
        }
        break;
      case 'R':
      case 'r':
        board.rotate_current_piece_right();
        break;
      case 'L':
      case 'l':
        board.rotate_current_piece_left();
        break;
      case 'X':
      case 'x':
        return 0;
        break;
    }
  }
  return 0;
}

