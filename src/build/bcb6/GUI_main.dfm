object FormBlockmania: TFormBlockmania
  Left = 502
  Top = 347
  Width = 411
  Height = 583
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSizeToolWin
  Caption = 'Blockmania'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -14
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnKeyDown = FormKeyDown
  PixelsPerInch = 120
  TextHeight = 16
  object ImageNextPiece: TImage
    Left = 20
    Top = 89
    Width = 60
    Height = 60
    Stretch = True
  end
  object SpeedButtonMoveRight: TSpeedButton
    Left = 89
    Top = 207
    Width = 28
    Height = 27
    Caption = '>'
    OnClick = SpeedButtonMoveRightClick
  end
  object SpeedButtonMoveLeft: TSpeedButton
    Left = 20
    Top = 207
    Width = 28
    Height = 27
    Caption = '<'
    OnClick = SpeedButtonMoveLeftClick
  end
  object SpeedButtonRotateRight: TSpeedButton
    Left = 89
    Top = 167
    Width = 28
    Height = 27
    Caption = '('
    OnClick = SpeedButtonRotateRightClick
  end
  object SpeedButtonRotateLeft: TSpeedButton
    Left = 20
    Top = 167
    Width = 28
    Height = 27
    Caption = ')'
    OnClick = SpeedButtonRotateLeftClick
  end
  object SpeedButtonDropOne: TSpeedButton
    Left = 20
    Top = 246
    Width = 99
    Height = 27
    Caption = 'v'
    OnClick = SpeedButtonDropOneClick
  end
  object SpeedButtonDropAll: TSpeedButton
    Left = 20
    Top = 286
    Width = 99
    Height = 27
    Caption = 'vv'
    OnClick = SpeedButtonDropAllClick
  end
  object Label1: TLabel
    Left = 20
    Top = 69
    Width = 27
    Height = 16
    Caption = 'Next'
  end
  object Label2: TLabel
    Left = 28
    Top = 384
    Width = 86
    Height = 16
    Caption = 'Rows cleared:'
  end
  object LabelRowsCleared: TLabel
    Left = 28
    Top = 404
    Width = 80
    Height = 35
    Alignment = taCenter
    AutoSize = False
    Caption = '0'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -28
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object ButtonStart: TButton
    Left = 20
    Top = 20
    Width = 92
    Height = 30
    Caption = '&Start'
    TabOrder = 0
    OnClick = ButtonStartClick
  end
  object MediaPlayer: TMediaPlayer
    Left = 55
    Top = 463
    Width = 29
    Height = 37
    Enabled = False
    VisibleButtons = []
    TabOrder = 1
  end
  object PageControl1: TPageControl
    Left = 131
    Top = 0
    Width = 272
    Height = 543
    ActivePage = TabSheet1
    Align = alRight
    TabIndex = 0
    TabOrder = 2
    object TabSheet1: TTabSheet
      Caption = 'Board'
      object PanelBoard: TPanel
        Left = 0
        Top = 0
        Width = 264
        Height = 512
        Align = alClient
        BevelOuter = bvNone
        TabOrder = 0
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'Instructions'
      ImageIndex = 1
      object Memo1: TMemo
        Left = 0
        Top = 0
        Width = 264
        Height = 512
        Align = alClient
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -20
        Font.Name = 'Courier New'
        Font.Style = []
        Lines.Strings = (
          'Rotate Right'
          '  E or NUM9'
          ''
          'Rotate Left'
          '  Q or NUM7'
          ''
          'Move Left'
          '  A or NUM4 or LEFT '
          'ARROW'
          ''
          'Move Right'
          '  D or NUM6 or RIGHT '
          'ARROW'
          ''
          'Down One'
          '  S or DOWN ARROW'
          ''
          'Down to Bottom'
          '  SPACE')
        ParentFont = False
        ReadOnly = True
        TabOrder = 0
      end
    end
  end
  object CheckBoxAllowExotic: TCheckBox
    Left = 24
    Top = 336
    Width = 97
    Height = 17
    Caption = 'Allow E&xotic'
    TabOrder = 3
  end
  object Timer: TTimer
    Enabled = False
    OnTimer = TimerTimer
    Left = 24
    Top = 264
  end
end
