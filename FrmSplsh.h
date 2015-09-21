//----------------------------------------------------------------------------
#ifndef FrmSplshH
#define FrmSplshH
//----------------------------------------------------------------------------
#include <vcl\System.hpp>
#include <vcl\Windows.hpp>
#include <vcl\SysUtils.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\ExtCtrls.hpp>
//----------------------------------------------------------------------------
class TSplashForm : public TForm
{
__published:
	TPanel *Panel1;
	TImage *ProgramIcon;
	TLabel *ProductName;
	TLabel *Version;
	TLabel *Copyright;
	TLabel *Comments;
    TLabel *Status;
    void __fastcall CloseForm(TObject *Sender);
    
    
    
    
   void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
   TShiftState Shift);
private:
public:
	virtual __fastcall TSplashForm(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern TSplashForm *SplashForm;
//----------------------------------------------------------------------------
#endif
