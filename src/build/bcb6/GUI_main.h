//---------------------------------------------------------------------------

#ifndef GUI_mainH
#define GUI_mainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
#include <Board_manager.h>
#include <Buttons.hpp>
#include <MPlayer.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
using namespace Blockmania;

class TFormBlockmania : public TForm
{
__published:	// IDE-managed Components
  TButton *ButtonStart;
  TImage *ImageNextPiece;
  TTimer *Timer;
  TSpeedButton *SpeedButtonMoveRight;
  TSpeedButton *SpeedButtonMoveLeft;
  TSpeedButton *SpeedButtonRotateRight;
  TSpeedButton *SpeedButtonRotateLeft;
  TSpeedButton *SpeedButtonDropOne;
  TSpeedButton *SpeedButtonDropAll;
  TLabel *Label1;
  TLabel *Label2;
  TLabel *LabelRowsCleared;
  TMediaPlayer *MediaPlayer;
  TPageControl *PageControl1;
  TTabSheet *TabSheet1;
  TTabSheet *TabSheet2;
  TPanel *PanelBoard;
  TMemo *Memo1;
        TCheckBox *CheckBoxAllowExotic;
  void __fastcall ButtonStartClick(TObject *Sender);
  void __fastcall TimerTimer(TObject *Sender);
  void __fastcall SpeedButtonMoveRightClick(TObject *Sender);
  void __fastcall SpeedButtonMoveLeftClick(TObject *Sender);
  void __fastcall SpeedButtonRotateRightClick(TObject *Sender);
  void __fastcall SpeedButtonRotateLeftClick(TObject *Sender);
  void __fastcall SpeedButtonDropOneClick(TObject *Sender);
  void __fastcall SpeedButtonDropAllClick(TObject *Sender);
  void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
private:	// User declarations
  Blockmania::Board_manager board_manager_;

  void update_board();
  int next_random_piece();
  bool end_turn_processing();

  int current_piece_;
  int total_pieces_removed_;
  TImage* block_image_[Board_manager::height][Board_manager::width];
public:		// User declarations
  __fastcall TFormBlockmania(TComponent* Owner);
  __fastcall ~TFormBlockmania();
};
//---------------------------------------------------------------------------
extern PACKAGE TFormBlockmania *FormBlockmania;
//---------------------------------------------------------------------------
#endif
