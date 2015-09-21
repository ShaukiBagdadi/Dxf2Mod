//----------------------------------------------------------
#ifndef FrmCnvH
#define FrmCnvH
//----------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\FileCtrl.hpp>
#include <vcl\ComCtrls.hpp>
#include <vcl\ExtCtrls.hpp>
#include <vcl\Buttons.hpp>
//----------------------------------------------------------
class TConvertForm : public TForm
{
__published:	// IDE-managed Components
  TFileListBox *FileListBox;
  TDirectoryListBox *DirectoryListBox;
  TDriveComboBox *DriveComboBox;
  TEdit *NameEdit;
  TProgressBar *ProgressBar;
  TLabel *Label1;
  TLabel *Label2;
  TBevel *Bevel1;
  TLabel *Label3;
  TLabel *Label4;
  TLabel *Label5;
  TLabel *Label7;
  TBitBtn *btnConvert;
  TBitBtn *btnClose;
  TBitBtn *btnAbort;
private:	// User declarations
public:		// User declarations
  __fastcall TConvertForm(TComponent* Owner);
};
//----------------------------------------------------------
extern TConvertForm *ConvertForm;
//----------------------------------------------------------
#endif
