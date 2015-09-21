//----------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "FrmSplsh.h"
//----------------------------------------------------------
#pragma resource "*.dfm"
TSplashForm *SplashForm;
//----------------------------------------------------------
__fastcall TSplashForm::TSplashForm(TComponent* AOwner)
  : TForm(AOwner) {
  Status->Caption = "";
  ProductName->Caption = Application->Title;
  ProgramIcon->Picture->Graphic = Application->Icon;
}//---------------------------------------------------------
void __fastcall TSplashForm::CloseForm(TObject *Sender) {
  if (Status->Caption == "") Close();
}//---------------------------------------------------------
void __fastcall TSplashForm::FormKeyDown(TObject *Sender,
   WORD &Key, TShiftState Shift)
{
  CloseForm(this);
}//---------------------------------------------------------
