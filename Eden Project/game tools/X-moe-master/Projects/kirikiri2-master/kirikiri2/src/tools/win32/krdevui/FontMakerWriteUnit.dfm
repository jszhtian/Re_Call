object FontMakerWriteForm: TFontMakerWriteForm
  Left = 104
  Top = 188
  BorderStyle = bsDialog
  Caption = '作成中 ...'
  ClientHeight = 64
  ClientWidth = 227
  Color = clBtnFace
  Font.Charset = SHIFTJIS_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'ＭＳ Ｐゴシック'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnCloseQuery = FormCloseQuery
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 12
  object ProgressBar: TProgressBar
    Left = 9
    Top = 8
    Width = 209
    Height = 16
    Min = 0
    Max = 100
    TabOrder = 0
  end
  object CancelButton: TButton
    Left = 76
    Top = 32
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'キャンセル'
    Enabled = False
    TabOrder = 1
    OnClick = CancelButtonClick
  end
  object Timer: TTimer
    Interval = 100
    OnTimer = TimerTimer
    Left = 16
    Top = 40
  end
end
