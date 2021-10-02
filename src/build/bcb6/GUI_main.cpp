//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "GUI_main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormBlockmania *FormBlockmania;
//---------------------------------------------------------------------------
__fastcall TFormBlockmania::TFormBlockmania(TComponent* Owner)
:
  TForm(Owner),
  total_pieces_removed_(0)
{
  for (int r = 0; r < Board_manager::height; ++r)
  {
    for (int c = 0; c < Board_manager::width; ++c)
    {
      block_image_[r][c] = new TImage(PanelBoard);
      TImage& image = *block_image_[r][c];
      image.Picture->LoadFromFile("images\\style0.bmp");
      image.AutoSize = false;
      image.Stretch = true;
      image.Width = 22;
      image.Height = 22;
      image.Left = c * image.Width;
      image.Top = r * image.Height;
      PanelBoard->InsertControl(&image);  // TPanel
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormBlockmania::ButtonStartClick(TObject *Sender)
{
  randomize();
  current_piece_ = next_random_piece();
  board_manager_.board().start_new_piece(current_piece_ - 1);
  current_piece_ = next_random_piece();
  Timer->Enabled = true;
  update_board();
  ButtonStart->Enabled = false;
}
//---------------------------------------------------------------------------
int TFormBlockmania::next_random_piece()  // returns 1 to N (inclusive, N == number of puzzle piece types)
{
  Board_manager::Board& board = board_manager_.board();
retry:
  int n = random( board.number_of_puzzle_piece_types() ) + 1;
  if (!CheckBoxAllowExotic->Checked)
  {
    if (n > 7)
    {
      goto retry;
    }
  }
  return n;
}

void TFormBlockmania::update_board()
{
  Board_manager::Board& board = board_manager_.board();
  AnsiString s;
  for (int r = 0; r < Board_manager::height; ++r)
  {
    for (int c = 0; c < Board_manager::width; ++c)
    {
      int i = board(r,c);
      s = "images\\style" + IntToStr(i) + ".bmp";
      TImage& image = *block_image_[r][c];
      image.Picture->LoadFromFile(s);
    }
  }

  s = "images\\style" + IntToStr(current_piece_) + ".bmp";
  ImageNextPiece->Picture->LoadFromFile(s);
}
void __fastcall TFormBlockmania::TimerTimer(TObject *Sender)
{
  Timer->Enabled = false;

  Board_manager::Board& board = board_manager_.board();
  if (board.drop_piece_failed(true))
  {
    if (end_turn_processing())
    {
      return;
    }
  }
  update_board();

  // Force focus back to the form (to intercept keyboard input)
  ButtonStart->Enabled = true;
  this->SetFocusedControl(ButtonStart);
  this->SetFocus();
  ButtonStart->Enabled = false;

  Timer->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFormBlockmania::SpeedButtonMoveRightClick(TObject *Sender)
{
  if (Timer->Enabled)
  {
    Board_manager::Board& board = board_manager_.board();
    board.move_current_piece_right();
    update_board();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormBlockmania::SpeedButtonMoveLeftClick(TObject *Sender)
{
  if (Timer->Enabled)
  {                                                  
    Board_manager::Board& board = board_manager_.board();
    board.move_current_piece_left();
    update_board();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormBlockmania::SpeedButtonRotateRightClick(
      TObject *Sender)
{
  if (Timer->Enabled)
  {
    Board_manager::Board& board = board_manager_.board();
    board.rotate_current_piece_right();
    update_board();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormBlockmania::SpeedButtonRotateLeftClick(
      TObject *Sender)
{
  if (Timer->Enabled)
  {
    Board_manager::Board& board = board_manager_.board();
    board.rotate_current_piece_left();
    update_board();
  }
}
//---------------------------------------------------------------------------
__fastcall TFormBlockmania::~TFormBlockmania()
{
  for (int r = 0; r < Board_manager::height; ++r)
  {
    for (int c = 0; c < Board_manager::width; ++c)
    {
      delete block_image_[r][c];
    }
  }
}
void __fastcall TFormBlockmania::SpeedButtonDropOneClick(TObject *Sender)
{
  Timer->Enabled = false;
  TimerTimer(0);
  Timer->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormBlockmania::SpeedButtonDropAllClick(TObject *Sender)
{
  if (!ButtonStart->Enabled)
  {
    Timer->Enabled = false;
    Board_manager::Board& board = board_manager_.board();
    while (!board.drop_piece_failed(true))
    {
      Sleep(1);
      update_board();
      Refresh();
    }
    end_turn_processing();
    update_board();
    Timer->Enabled = true;
  }
}
//---------------------------------------------------------------------------
bool TFormBlockmania::end_turn_processing()
{
  Board_manager::Board& board = board_manager_.board();
  board.stick_piece();
  int removed_pieces = board.remove_completed_rows();
//  total_pieces_removed_ += removed_pieces;
  if (removed_pieces > 0)
  {
    Board_manager::Board::Rows_removed rows_removed = board.rows_removed();
    for (int r = board.rows()-1; r >= 0; --r)
    {
      if (rows_removed[r])
      {
        for (int c = 0; c < Board_manager::width; ++c)
        {
          AnsiString s = "images\\styleX.bmp";
          TImage& image = *block_image_[r][c];
          image.Picture->LoadFromFile(s);
        }
        Repaint();
        ++total_pieces_removed_;
        Sleep(200);
      }
    }

    if (total_pieces_removed_ % 10 == 0)
    {
      Timer->Interval -= 100;
    }
  }
  LabelRowsCleared->Caption = IntToStr(total_pieces_removed_);
  board.start_new_piece(current_piece_ - 1);
  current_piece_ = next_random_piece();
  if (board.drop_piece_failed(false))
  {
    this->Caption = "!!! GAME OVER !!!";
    return true;
  }
  return false;
}

void __fastcall TFormBlockmania::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  switch (Key)
  {
  case VK_DOWN:
  case 'S':
    SpeedButtonDropOneClick(0);
    break;
  case VK_SPACE:
    SpeedButtonDropAllClick(0);
    break;
  case VK_NUMPAD7:
  case 'Q':
    SpeedButtonRotateLeftClick(0);
    break;
  case VK_NUMPAD9:
  case 'E':
    SpeedButtonRotateRightClick(0);
    break;
  case VK_NUMPAD4:
  case VK_LEFT:
  case 'A':
    SpeedButtonMoveLeftClick(0);
    break;
  case VK_NUMPAD6:
  case VK_RIGHT:
  case 'D':
    SpeedButtonMoveRightClick(0);
    break;
  }
}
//---------------------------------------------------------------------------



