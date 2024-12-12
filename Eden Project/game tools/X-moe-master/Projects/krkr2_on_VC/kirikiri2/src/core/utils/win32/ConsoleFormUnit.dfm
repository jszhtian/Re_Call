object TVPConsoleForm: TTVPConsoleForm
  Left = 63
  Top = 88
  Width = 515
  Height = 389
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  ActiveControl = ExprComboBox
  BorderIcons = [biSystemMenu, biMaximize]
  Caption = '�R���\�[��'
  Color = clBtnFace
  Constraints.MinHeight = 100
  Constraints.MinWidth = 70
  DefaultMonitor = dmDesktop
  Font.Charset = SHIFTJIS_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = '�l�r �o�S�V�b�N'
  Font.Style = []
  OldCreateOrder = False
  Position = poDefaultPosOnly
  OnClose = FormClose
  OnDestroy = FormDestroy
  OnHide = FormHide
  OnMouseWheelDown = FormMouseWheelDown
  OnMouseWheelUp = FormMouseWheelUp
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 12
  object StatusBar: TStatusBar
    Left = 488
    Top = 343
    Width = 19
    Height = 19
    Align = alNone
    Anchors = [akRight, akBottom]
    Panels = <
      item
        Bevel = pbNone
        Width = 50
      end>
    SimplePanel = False
  end
  object ExprComboBox: TComboBox
    Left = 32
    Top = 341
    Width = 457
    Height = 20
    Anchors = [akLeft, akRight, akBottom]
    Font.Charset = SHIFTJIS_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = '�l�r ����'
    Font.Style = []
    ItemHeight = 12
    ParentFont = False
    TabOrder = 1
    OnKeyDown = ExprComboBoxKeyDown
    OnKeyPress = ExprComboBoxKeyPress
  end
  object ToolBar: TToolBar
    Left = 0
    Top = 342
    Width = 25
    Height = 19
    Align = alNone
    Anchors = [akLeft, akBottom]
    ButtonHeight = 18
    ButtonWidth = 19
    Caption = 'ToolBar'
    EdgeBorders = []
    Flat = True
    Images = TVPMainForm.VerySmallIconImageList
    ParentShowHint = False
    ShowHint = True
    TabOrder = 2
    object ExecButton: TToolButton
      Left = 0
      Top = 0
      Hint = '���s'
      Caption = '���s'
      ImageIndex = 0
      OnClick = ExecButtonClick
    end
  end
  object LogPanel: TPanel
    Left = 96
    Top = 40
    Width = 257
    Height = 159
    Anchors = [akLeft, akTop, akRight, akBottom]
    BevelInner = bvLowered
    BevelOuter = bvLowered
    Font.Charset = SHIFTJIS_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = '�l�r ����'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
  end
  object PopupMenu: TPopupMenu
    Images = TVPMainForm.SmallIconImageList
    OnPopup = PopupMenuPopup
    Left = 24
    Top = 24
    object CopyMenuItem: TMenuItem
      Caption = '�R�s�[(&C)'
      Hint = '�R�s�['
      ImageIndex = 6
      OnClick = CopyMenuItemClick
    end
    object SelectAllMenuItem: TMenuItem
      Caption = '���ׂđI��(&A)'
      OnClick = SelectAllMenuItemClick
    end
    object N1: TMenuItem
      Caption = '-'
    end
    object ShowControllerMenuItem: TMenuItem
      Caption = '�R���g���[��(&Z)'
      Hint = '�R���g���[����\��'
      ImageIndex = 28
      ShortCut = 8304
      OnClick = ShowControllerMenuItemClick
    end
    object ShowScriptEditorMenuItem: TMenuItem
      Caption = '�X�N���v�g�G�f�B�^(&E)'
      Hint = '�X�N���v�g�G�f�B�^��\��'
      ImageIndex = 2
      ShortCut = 8305
      OnClick = ShowScriptEditorMenuItemClick
    end
    object ShowWatchMenuItem: TMenuItem
      Caption = '�Ď���(&W)'
      Hint = '�Ď�����\��'
      ImageIndex = 14
      ShortCut = 8306
      OnClick = ShowWatchMenuItemClick
    end
    object ShowConsoleMenuItem: TMenuItem
      Caption = '�R���\�[��(&D)'
      ShortCut = 8307
      Visible = False
      OnClick = ShowConsoleMenuItemClick
    end
    object N3: TMenuItem
      Caption = '-'
    end
    object ShowOnTopMenuItem: TMenuItem
      Caption = '��Ɏ�O�ɕ\��(&F)'
      ImageIndex = 10
      OnClick = ShowOnTopMenuItemClick
    end
    object N2: TMenuItem
      Caption = '-'
    end
    object SelectFontMenuItem: TMenuItem
      Caption = '�t�H���g(&N) ...'
      OnClick = SelectFontMenuItemClick
    end
    object AutoShowOnErrorMenuItem: TMenuItem
      Caption = '�G���[���Ɏ����I�ɕ\������(&V)'
      OnClick = AutoShowOnErrorMenuItemClick
    end
    object ShowAboutMenuItem: TMenuItem
      Caption = 'about ...'
      ShortCut = 16507
      Visible = False
      OnClick = ShowAboutMenuItemClick
    end
    object CopyImportantLogMenuItem: TMenuItem
      Caption = 'copy important log'
      ShortCut = 16506
      Visible = False
      OnClick = CopyImportantLogMenuItemClick
    end
  end
  object FontDialog: TFontDialog
    Font.Charset = SHIFTJIS_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = '�l�r �o�S�V�b�N'
    Font.Style = []
    MinFontSize = 0
    MaxFontSize = 0
    Options = [fdAnsiOnly, fdForceFontExist]
    Left = 416
    Top = 56
  end
end
