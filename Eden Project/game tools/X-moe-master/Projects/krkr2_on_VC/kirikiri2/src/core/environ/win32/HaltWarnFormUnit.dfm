object TVPHaltWarnForm: TTVPHaltWarnForm
  Left = 53
  Top = 251
  BorderStyle = bsDialog
  ClientHeight = 125
  ClientWidth = 476
  Color = clBtnFace
  DefaultMonitor = dmDesktop
  Font.Charset = SHIFTJIS_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = '�l�r �o�S�V�b�N'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 12
  object Label1: TLabel
    Left = 72
    Top = 16
    Width = 383
    Height = 12
    Caption = 
      '���̃v���O�����́A���݃C�x���g�z�M����~���Ă��邩�A�ҋ@��Ԃɂ�' +
      '�邽��'
  end
  object Label2: TLabel
    Left = 72
    Top = 32
    Width = 76
    Height = 12
    Caption = '�������܂���B'
  end
  object Label3: TLabel
    Left = 72
    Top = 48
    Width = 384
    Height = 12
    Caption = 
      '�v���O�������I������ɂ� [�����I��] �{�^�����A���s����ɂ� [���s' +
      '] �{�^��'
  end
  object Label4: TLabel
    Left = 72
    Top = 64
    Width = 100
    Height = 12
    Caption = '��I�����Ă��������B'
  end
  object IconPaintBox: TPaintBox
    Left = 16
    Top = 16
    Width = 49
    Height = 49
    OnPaint = IconPaintBoxPaint
  end
  object ExitButton: TButton
    Left = 141
    Top = 88
    Width = 91
    Height = 25
    Caption = '�����I��(&E)'
    TabOrder = 0
    OnClick = ExitButtonClick
  end
  object ContinueButton: TButton
    Left = 245
    Top = 88
    Width = 89
    Height = 25
    Cancel = True
    Caption = '���s(&C)'
    Default = True
    TabOrder = 1
    OnClick = ContinueButtonClick
  end
end
