object TPCMainForm: TTPCMainForm
  Left = 152
  Top = 7
  ActiveControl = CloseButton
  BorderStyle = bsDialog
  Caption = '�摜�t�H�[�}�b�g�R���o�[�^'
  ClientHeight = 390
  ClientWidth = 546
  Color = clBtnFace
  Font.Charset = SHIFTJIS_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = '�l�r �o�S�V�b�N'
  Font.Style = []
  OldCreateOrder = False
  Position = poDefault
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 12
  object Label1: TLabel
    Left = 8
    Top = 9
    Width = 283
    Height = 12
    Caption = '�ϊ��������t�@�C�������̃E�B���h�E�Ƀh���b�v���Ă�������'
  end
  object TranspOuputFormatGroupBox: TGroupBox
    Left = 144
    Top = 32
    Width = 393
    Height = 209
    Caption = '���������̂���摜(&F)'
    TabOrder = 2
    object TranspMainFormatLabel: TLabel
      Left = 232
      Top = 28
      Width = 75
      Height = 12
      Caption = '���C���`��(&C) :'
      FocusControl = TranspMainFormatComboBox
    end
    object TranspMaskFormatLabel: TLabel
      Left = 232
      Top = 75
      Width = 79
      Height = 12
      Caption = '�}�X�N�`��(&M) :'
      FocusControl = TranspMaskFormatComboBox
    end
    object TranspFullTranspColorMethodLabel: TLabel
      Left = 16
      Top = 176
      Width = 146
      Height = 12
      Caption = '���S���������̐F���(&U) : '
      FocusControl = TranspFullTranspColorMethodComboBox
    end
    object TranspBMPFormatRadioButton: TRadioButton
      Left = 16
      Top = 24
      Width = 193
      Height = 17
      Caption = '���`�����l���t�� 32bit B&MP'
      TabOrder = 0
      OnClick = TranspBMPFormatRadioButtonClick
    end
    object TranspPNGFormatRadioButton: TRadioButton
      Left = 16
      Top = 40
      Width = 161
      Height = 17
      Caption = '���`���l���t�� P&NG'
      Checked = True
      TabOrder = 1
      TabStop = True
      OnClick = TranspPNGFormatRadioButtonClick
    end
    object TranspSeparatedFormatRadioButton: TRadioButton
      Left = 16
      Top = 88
      Width = 153
      Height = 17
      Caption = '���C��/�}�X�N�����`��(&S)'
      TabOrder = 4
      OnClick = TranspSeparatedFormatRadioButtonClick
    end
    object TranspMainFormatComboBox: TComboBox
      Left = 320
      Top = 24
      Width = 57
      Height = 20
      Style = csDropDownList
      ItemHeight = 12
      TabOrder = 5
      OnChange = TranspMainFormatComboBoxChange
      Items.Strings = (
        'BMP'
        'JPEG'
        'PNG')
    end
    object TranspMaskFormatComboBox: TComboBox
      Left = 320
      Top = 71
      Width = 57
      Height = 20
      Style = csDropDownList
      ItemHeight = 12
      TabOrder = 7
      OnChange = TranspMaskFormatComboBoxChange
      Items.Strings = (
        'BMP'
        'JPEG'
        'PNG')
    end
    object TranspMainJPEGOptionButton: TButton
      Left = 304
      Top = 48
      Width = 73
      Height = 20
      Caption = 'JPEG�ݒ� ...'
      TabOrder = 6
      OnClick = TranspMainJPEGOptionButtonClick
    end
    object TranspMaskJPEGOptionButton: TButton
      Left = 304
      Top = 95
      Width = 73
      Height = 20
      Caption = 'JPEG�ݒ� ...'
      TabOrder = 8
      OnClick = TranspMaskJPEGOptionButtonClick
    end
    object TranspFullTranspColorMethodComboBox: TComboBox
      Left = 176
      Top = 172
      Width = 97
      Height = 20
      Style = csDropDownList
      ItemHeight = 12
      TabOrder = 11
      Items.Strings = (
        '����'
        '���̂܂�'
        '����(1pixel)'
        '����(2pixel)'
        '����(3pixel)'
        '����(5pixel)'
        '����(8pixel)')
    end
    object TranspTLG5FormatRadioButton: TRadioButton
      Left = 16
      Top = 56
      Width = 153
      Height = 17
      Caption = '���`���l���t�� T&LG5'
      TabOrder = 2
      OnClick = TranspTLG5FormatRadioButtonClick
    end
    object TranspTLG6FormatRadioButton: TRadioButton
      Left = 16
      Top = 72
      Width = 145
      Height = 17
      Caption = '���`���l���t�� TLG&6'
      TabOrder = 3
      OnClick = TranspTLG6FormatRadioButtonClick
    end
    object TranspOutputAddAlphaFormatCheckBox: TCheckBox
      Left = 16
      Top = 152
      Width = 353
      Height = 17
      Caption = 'ltAddAlpha�`���ŏo�͂���(&A)'
      TabOrder = 10
      OnClick = TranspOutputAddAlphaFormatCheckBoxClick
    end
    object Bar1: TPanel
      Left = 8
      Top = 128
      Width = 372
      Height = 2
      BevelOuter = bvLowered
      TabOrder = 12
    end
    object TranspAssumeInputIsAddAlphaCheckBox: TCheckBox
      Left = 16
      Top = 136
      Width = 353
      Height = 17
      Caption = '���͉摜��ltAddAlpha�`���ł���Ƃ݂Ȃ�(&D)'
      TabOrder = 9
      OnClick = TranspAssumeInputIsAddAlphaCheckBoxClick
    end
  end
  object OutputFolderGroup: TGroupBox
    Left = 8
    Top = 248
    Width = 529
    Height = 105
    Caption = '�o�̓t�H���_(&O)'
    TabOrder = 3
    object SameFolderRadioButton: TRadioButton
      Left = 16
      Top = 24
      Width = 249
      Height = 17
      Caption = '���̓t�@�C���Ɠ����t�H���_(&I)'
      Checked = True
      TabOrder = 0
      TabStop = True
      OnClick = SameFolderRadioButtonClick
    end
    object SpecifyFolderRadioButton: TRadioButton
      Left = 16
      Top = 48
      Width = 137
      Height = 17
      Caption = '�w��t�H���_(&Y) :'
      TabOrder = 1
      OnClick = SpecifyFolderRadioButtonClick
    end
    object OutputFolderEdit: TEdit
      Left = 160
      Top = 46
      Width = 297
      Height = 20
      Font.Charset = SHIFTJIS_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = '�l�r �S�V�b�N'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentFont = False
      TabOrder = 2
    end
    object OutputFolderRefButton: TButton
      Left = 464
      Top = 46
      Width = 49
      Height = 20
      Caption = '�Q�� ...'
      TabOrder = 3
      OnClick = OutputFolderRefButtonClick
    end
    object OverwriteCheckBox: TCheckBox
      Left = 16
      Top = 72
      Width = 313
      Height = 17
      Caption = '�����̃t�@�C�����������ꍇ�ɏ㏑������(&O)'
      TabOrder = 4
    end
  end
  object CloseButton: TButton
    Left = 460
    Top = 360
    Width = 75
    Height = 21
    Cancel = True
    Caption = '����(&X)'
    TabOrder = 5
    OnClick = CloseButtonClick
  end
  object OpaqueOutputFormatGroupBox: TGroupBox
    Left = 8
    Top = 32
    Width = 129
    Height = 209
    Caption = '�s�����ȉ摜(&Q)'
    TabOrder = 1
    object OpaqueBMPFormatRadioButton: TRadioButton
      Left = 16
      Top = 24
      Width = 97
      Height = 17
      Caption = '24bit &BMP'
      TabOrder = 0
      OnClick = OpaqueBMPFormatRadioButtonClick
    end
    object OpaquePNGFormatRadioButton: TRadioButton
      Left = 16
      Top = 40
      Width = 97
      Height = 17
      Caption = '&PNG'
      Checked = True
      TabOrder = 1
      TabStop = True
      OnClick = OpaquePNGFormatRadioButtonClick
    end
    object OpaqueTLG5FormatRadioButton: TRadioButton
      Left = 16
      Top = 56
      Width = 97
      Height = 17
      Caption = '&TLG5'
      TabOrder = 2
      OnClick = OpaqueTLG5FormatRadioButtonClick
    end
    object OpaqueJPEGFormatRadioButton: TRadioButton
      Left = 16
      Top = 88
      Width = 97
      Height = 17
      Caption = '&JPEG'
      TabOrder = 4
      OnClick = OpaqueJPEGFormatRadioButtonClick
    end
    object OpaqueTLG6FormatRadioButton: TRadioButton
      Left = 16
      Top = 72
      Width = 97
      Height = 17
      Caption = 'TL&G6'
      TabOrder = 3
      OnClick = OpaqueTLG6FormatRadioButtonClick
    end
    object OpaqueJPEGOptionButton: TButton
      Left = 32
      Top = 109
      Width = 73
      Height = 20
      Caption = 'JPEG�ݒ� ...'
      TabOrder = 5
      OnClick = OpaqueJPEGOptionButtonClick
    end
  end
  object DoNotShowLogWhenNoErrorCheckBox: TCheckBox
    Left = 8
    Top = 361
    Width = 361
    Height = 17
    Caption = '�G���[���������Ȃ������ꍇ�̓��O��\�����Ȃ�(&K)'
    TabOrder = 4
  end
  object ExpandButton: TCheckBox
    Left = 312
    Top = 6
    Width = 129
    Height = 17
    Caption = '�I�v�V������\��(&V)'
    TabOrder = 0
    OnClick = ExpandButtonClick
  end
end
