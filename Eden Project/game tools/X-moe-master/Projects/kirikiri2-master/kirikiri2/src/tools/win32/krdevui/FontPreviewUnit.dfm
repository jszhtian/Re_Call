object FontPreviewForm: TFontPreviewForm
  Left = 105
  Top = 114
  Width = 599
  Height = 390
  Caption = '�t�H���g�T���v��'
  Color = clBtnFace
  Font.Charset = SHIFTJIS_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = '�l�r �o�S�V�b�N'
  Font.Style = []
  OldCreateOrder = False
  Position = poDefaultPosOnly
  OnClose = FormClose
  OnDestroy = FormDestroy
  OnMouseWheelDown = FormMouseWheelDown
  OnMouseWheelUp = FormMouseWheelUp
  PixelsPerInch = 96
  TextHeight = 12
  object UpperPanel: TPanel
    Left = 0
    Top = 0
    Width = 591
    Height = 20
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 0
    object ZoomComboBox: TComboBox
      Left = 0
      Top = 0
      Width = 145
      Height = 20
      Style = csDropDownList
      DropDownCount = 16
      ItemHeight = 12
      TabOrder = 0
      OnChange = ZoomComboBoxChange
      Items.Strings = (
        '�g�嗦 ���{'
        '�g�嗦 2�{'
        '�g�嗦 3�{'
        '�g�嗦 4�{'
        '�g�嗦 5�{'
        '�g�嗦 6�{'
        '�g�嗦 7�{'
        '�g�嗦 8�{'
        '�g�嗦 9�{'
        '�g�嗦 10�{'
        '�g�嗦 11�{'
        '�g�嗦 12�{'
        '�g�嗦 13�{'
        '�g�嗦 14�{'
        '�g�嗦 15�{'
        '�g�嗦 16�{')
    end
    object CloseButton: TButton
      Left = 176
      Top = 0
      Width = 75
      Height = 20
      Cancel = True
      Caption = '����(&C)'
      TabOrder = 1
      OnClick = CloseButtonClick
    end
  end
  object StatusBar: TStatusBar
    Left = 0
    Top = 337
    Width = 591
    Height = 19
    Panels = <
      item
        Width = 120
      end
      item
        Width = 50
      end>
    SimplePanel = False
  end
  object PreviewPanel: TPanel
    Left = 0
    Top = 20
    Width = 591
    Height = 317
    Align = alClient
    BevelInner = bvLowered
    BevelOuter = bvNone
    TabOrder = 2
  end
end
