object TVPWatchForm: TTVPWatchForm
  Left = 50
  Top = 127
  Width = 299
  Height = 223
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  Caption = '�Ď���'
  Color = clBtnFace
  DefaultMonitor = dmDesktop
  Font.Charset = SHIFTJIS_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = '�l�r �o�S�V�b�N'
  Font.Style = []
  OldCreateOrder = False
  PopupMenu = PopupMenu
  Position = poDefaultPosOnly
  OnClose = FormClose
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 12
  object ToolBar: TToolBar
    Left = 0
    Top = 178
    Width = 89
    Height = 18
    Align = alNone
    Anchors = [akLeft, akBottom]
    ButtonHeight = 18
    ButtonWidth = 19
    Caption = 'ToolBar'
    EdgeBorders = []
    Flat = True
    Images = TVPMainForm.VerySmallIconImageList
    TabOrder = 0
    object UpdateButton: TToolButton
      Left = 0
      Top = 0
      Hint = '�X�V'
      Caption = '�X�V'
      ImageIndex = 4
      ParentShowHint = False
      ShowHint = True
      OnClick = UpdateButtonClick
    end
    object AutoUpdateButton: TToolButton
      Left = 19
      Top = 0
      Hint = '�����X�V'
      Caption = '�����X�V'
      ImageIndex = 5
      ParentShowHint = False
      ShowHint = True
      Style = tbsCheck
      OnClick = AutoUpdateButtonClick
    end
    object ToolButton3: TToolButton
      Left = 38
      Top = 0
      Width = 8
      Caption = 'ToolButton3'
      ImageIndex = 8
      Style = tbsSeparator
    end
    object NewExprButton: TToolButton
      Left = 46
      Top = 0
      Hint = '�V�K�̎�'
      Caption = '�V�K�̎�'
      ImageIndex = 6
      ParentShowHint = False
      ShowHint = True
      OnClick = NewExprButtonClick
    end
    object DeleteButton: TToolButton
      Left = 65
      Top = 0
      Hint = '�폜'
      Caption = '�폜'
      ImageIndex = 7
      ParentShowHint = False
      ShowHint = True
      OnClick = DeleteButtonClick
    end
  end
  object StatusBar: TStatusBar
    Left = 96
    Top = 177
    Width = 195
    Height = 19
    Align = alNone
    Anchors = [akLeft, akRight, akBottom]
    Panels = <>
    SimplePanel = False
  end
  object ListView: TListView
    Left = 0
    Top = 0
    Width = 289
    Height = 175
    Anchors = [akLeft, akTop, akRight, akBottom]
    Columns = <
      item
        Caption = '��'
        Width = 140
      end
      item
        Caption = '����'
        Width = 400
      end>
    ColumnClick = False
    MultiSelect = True
    TabOrder = 2
    ViewStyle = vsReport
    OnDblClick = ListViewDblClick
    OnEdited = ListViewEdited
    OnKeyPress = ListViewKeyPress
    OnMouseMove = ListViewMouseMove
  end
  object PopupMenu: TPopupMenu
    Images = TVPMainForm.SmallIconImageList
    OnPopup = PopupMenuPopup
    Left = 256
    Top = 24
    object NewExprMenuItem: TMenuItem
      Caption = '�V�K�̎�(&N) ...'
      Hint = '�V�K�̎�'
      ImageIndex = 27
      ShortCut = 45
      OnClick = NewExprButtonClick
    end
    object DeleteExprMenuItem: TMenuItem
      Caption = '�폜(&X)'
      Hint = '�폜'
      ImageIndex = 15
      ShortCut = 46
      OnClick = DeleteButtonClick
    end
    object EditExpressionMenuItem: TMenuItem
      Caption = '���̕ҏW(&E)'
      OnClick = EditExpressionMenuItemClick
    end
    object N2: TMenuItem
      Caption = '-'
    end
    object UpdateMenuItem: TMenuItem
      Caption = '�X�V(&U)'
      ImageIndex = 25
      ShortCut = 116
      OnClick = UpdateButtonClick
    end
    object AutoUpdateMenuItem: TMenuItem
      Caption = '�����X�V(&A)'
      ImageIndex = 26
      ShortCut = 16500
      OnClick = AutoUpdateMenuItemClick
    end
    object AutoUpdateIntervalMenuItem: TMenuItem
      Caption = '�����X�V�̊Ԋu(&I)'
      Hint = '�����X�V�̊Ԋu'
      object UIRealTimeMenuItem: TMenuItem
        Caption = '���A���^�C��(&R)'
        GroupIndex = 11
        Hint = '���A���^�C��'
        RadioItem = True
        Visible = False
        OnClick = UIRealTimeMenuItemClick
      end
      object UI0_2SecMenuItem: TMenuItem
        Tag = 200
        Caption = '0.2�b(&A)'
        GroupIndex = 11
        Hint = '0.2�b'
        RadioItem = True
        OnClick = UIRealTimeMenuItemClick
      end
      object UI0_5SecMenuItem: TMenuItem
        Tag = 500
        Caption = '0.5�b(&B)'
        GroupIndex = 11
        Hint = '0.5�b'
        RadioItem = True
        OnClick = UIRealTimeMenuItemClick
      end
      object N071: TMenuItem
        Tag = 700
        Caption = '0.7�b(&C)'
        GroupIndex = 11
        Hint = '0.7�b'
        RadioItem = True
        OnClick = UIRealTimeMenuItemClick
      end
      object UI1SecMenuItem: TMenuItem
        Tag = 1000
        Caption = '1�b(&1)'
        Checked = True
        GroupIndex = 11
        Hint = '1�b'
        RadioItem = True
        OnClick = UIRealTimeMenuItemClick
      end
      object N221: TMenuItem
        Tag = 2000
        Caption = '2�b(&2)'
        GroupIndex = 11
        Hint = '2�b'
        RadioItem = True
        OnClick = UIRealTimeMenuItemClick
      end
      object UI3SecMenuItem: TMenuItem
        Tag = 3000
        Caption = '3�b(&3)'
        GroupIndex = 11
        Hint = '3�b'
        RadioItem = True
        OnClick = UIRealTimeMenuItemClick
      end
      object UI5SecMenuItem: TMenuItem
        Tag = 5000
        Caption = '5�b(&5)'
        GroupIndex = 11
        Hint = '5�b'
        RadioItem = True
        OnClick = UIRealTimeMenuItemClick
      end
      object UI9SecMenuItem: TMenuItem
        Tag = 9000
        Caption = '9�b(&9)'
        GroupIndex = 11
        Hint = '9�b'
        RadioItem = True
        OnClick = UIRealTimeMenuItemClick
      end
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
      ShortCut = 8306
      Visible = False
      OnClick = ShowWatchMenuItemClick
    end
    object ShowConsoleMenuItem: TMenuItem
      Caption = '�R���\�[��(&D)'
      Hint = '�R���\�[����\��'
      ImageIndex = 13
      ShortCut = 8307
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
  object EliminateTimer: TTimer
    Enabled = False
    Interval = 100
    OnTimer = EliminateTimerTimer
    Left = 256
    Top = 56
  end
  object UpdateTimer: TTimer
    Enabled = False
    OnTimer = UpdateTimerTimer
    Left = 256
    Top = 88
  end
end
