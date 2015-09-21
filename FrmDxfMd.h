//----------------------------------------------------------
#ifndef FrmDxfMdH
#define FrmDxfMdH
//----------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ExtCtrls.hpp>
#include <vcl\FileCtrl.hpp>
#include <vcl\ComCtrls.hpp>
#include <vcl\Buttons.hpp>
#include "FrmCnv.h"
#include "DxfModel.h"
//----------------------------------------------------------
class TDxfModCnvForm : public TConvertForm
{
__published:	// IDE-managed Components
  void __fastcall btnConvertClick(TObject *Sender);
  void __fastcall btnAbortClick(TObject *Sender);
   
private:	// User declarations
  int FaceCount, BadCount, DupCount, BadNodes;
  TDxfMeshModel *DxfModel;
  TVertList                    VertList;
  ChainList<int, string>       RCTable;
  ChainList<TFaceList, string> FaceTable;
  ChainList<TInsert, int>      DFTable;
  bool __fastcall IsFeaLyer(string &lname);
  void __fastcall MakeRCTable();
  void __fastcall MakeVertList();
  void __fastcall MakeFaceTable();
  void __fastcall MakeDFTable();
  int  __fastcall VertIndex(const TextVector& v);
  void __fastcall WriteMod(char *fname);
  void __fastcall WriteGeo(char *fname);
public:		// User declarations
  __fastcall TDxfModCnvForm(TComponent* Owner);
  __fastcall ~TDxfModCnvForm();
  MESSAGE void __fastcall WMHelp(TMessage &Message);
BEGIN_MESSAGE_MAP
  MESSAGE_HANDLER(WM_HELP, TMessage, WMHelp);
END_MESSAGE_MAP(TForm);
};
//----------------------------------------------------------
extern TDxfModCnvForm *DxfModCnvForm;
//----------------------------------------------------------
#endif
