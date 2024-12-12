//---------------------------------------------------------------------------

#ifndef MainUnitH
#define MainUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE �Ǘ��̃R���|�[�l���g
	TMemo *NoticeMemo;
	TListView *ListView;
	TButton *CheckButton;
	TButton *CloseButton;
	TLabel *NotYetCheckedLabel;
	TLabel *CheckingLabel;
	TLabel *BrokenLabel;
	TLabel *NotBrokenLabel;
	TLabel *MoreThanOneFileIsBrokenLabel;
	TLabel *CheckingButtonLabel;
	TButton *CopyResultButton;
	TLabel *TheResultWasCopiedIntoClipboardLabel;
	void __fastcall CloseButtonClick(TObject *Sender);
	void __fastcall CheckButtonClick(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall CopyResultButtonClick(TObject *Sender);
private:	// ���[�U�[�錾
	bool Checking; // �������`�F�b�N�����ǂ���
	bool Aborting; // ����{�^���������ꂽ��
	AnsiString BaseDir;
	AnsiString PublicKey;
	TListItem *CurrentCheckingItem;
	AnsiString AllFileList;
	int AllFileCount;
	DWORD LastDisplayedTick;
public:		// ���[�U�[�錾
	__fastcall TMainForm(TComponent* Owner);
private:	// ���[�U�[�錾
	void __fastcall MakeFileList(AnsiString path, int baselen);
	bool __fastcall SigProgressNotify(int percent);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
