//----------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop
//----------------------------------------------------------
USEFORM("FrmCnv.cpp", ConvertForm);
USEFORM("FrmDxfMd.cpp", DxfModCnvForm);
USEFORM("FrmSplsh.cpp", SplashForm);
USEUNIT("MMaker.cpp");
USEUNIT("DxfModel.cpp");
USERES("Dxf2Mod.res");
USERC("Info.rc");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
  try {
     // Application->Initialize();
     Application->Title = "DXF2MOD 2.0";
     Application->HelpFile = "Dxf2mod.hlp";
     Application->CreateForm(__classid(TDxfModCnvForm), &DxfModCnvForm);
     Application->CreateForm(__classid(TSplashForm), &SplashForm);
     Application->Run();
  } catch (Exception &exception) {
     Application->ShowException(&exception);
  }
  return 0;
}//---------------------------------------------------------
