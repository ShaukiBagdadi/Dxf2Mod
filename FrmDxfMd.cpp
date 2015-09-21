//----------------------------------------------------------
#include <vcl\vcl.h>
#include <errno.h>
#include <assert.h>

#pragma hdrstop

#include "FrmDxfMd.h"
#include "FrmSplsh.h"
//----------------------------------------------------------
#pragma link "FrmCnv"
#pragma resource "*.dfm"
TDxfModCnvForm *DxfModCnvForm;
//----------------------------------------------------------
static void OnProgress(long now, long total) {
  TProgressRange max = ::DxfModCnvForm->ProgressBar->Max;
  TProgressRange pos = TProgressRange(float(now)/total*max);
  ::DxfModCnvForm->ProgressBar->Position = pos;
  Application->ProcessMessages(); // позволить нажать кнопку
  // Abort .........
}//---------------------------------------------------------
__fastcall TDxfModCnvForm::TDxfModCnvForm(TComponent* Owner)
   : TConvertForm(Owner)
{
  DxfModel = new TDxfMeshModel;
  DxfModel->DoSkipBlocks = true;
  DxfModel->OnProgress   = OnProgress;
}//---------------------------------------------------------
__fastcall TDxfModCnvForm::~TDxfModCnvForm() {
  delete DxfModel;
  RCTable.Clear();
  VertList.Clear();
  FaceTable.Clear();
  DFTable.Clear();
}//---------------------------------------------------------
void __fastcall TDxfModCnvForm::WMHelp(TMessage &Message) {
  if (!FileExists(Application->HelpFile))
     SplashForm->ShowModal();
  else Application->HelpCommand(HELP_CONTENTS,0);
}//---------------------------------------------------------
void __fastcall TDxfModCnvForm::btnConvertClick(TObject*) {
  char buf[512];
  AnsiString DxfFile, ModFile, GeoFile;
  // Проверка наличия исходного файла:
  DxfFile = NameEdit->Text;
  if (!FileExists(DxfFile)) {
     sprintf(buf,
        "File \"%s\" not found!\n", DxfFile.c_str());
     Application->MessageBox(buf,
        Application->Title.c_str(),
        MB_OK | MB_ICONERROR);
     return;
  }
  // Предотвратить нечаянную запись поверх существующего файла:
  ModFile =  ChangeFileExt(DxfFile, ".mod");
  if (FileExists(ModFile)) {
     sprintf(buf,
        "File \"%s\" already exists!\nOk to overwrite?",
        ModFile.c_str());
     if (Application->MessageBox(buf,
         Application->Title.c_str(),
         MB_YESNO | MB_ICONQUESTION) == ID_NO)
     return;
  }
  GeoFile =  ChangeFileExt(DxfFile, ".geo");
  if (FileExists(GeoFile)) {
     sprintf(buf,
        "File \"%s\" already exists!\nOk to overwrite?",
        GeoFile.c_str());
     if (Application->MessageBox(buf,
         Application->Title.c_str(),
         MB_YESNO | MB_ICONQUESTION) == ID_NO)
     return;
  }
  // Установить состояния кнопок:
  btnConvert->Enabled = false;
  btnAbort->Enabled   = true;
  // Начать преобразование:
  try {
     DxfModel->ReadFromFile(DxfFile.c_str());
     MakeRCTable();
     MakeVertList();
     MakeFaceTable();
     MakeDFTable();
     WriteMod(ModFile.c_str());
     WriteGeo(GeoFile.c_str());
  } catch(Exception &exception) {
     // Перехватываем здесь исключения, для того, чтобы
     // смогли восстановит состояния органов управления
     Application->ShowException(&exception);
  }
  // Вернуть состояния кнопок для повторнго запуска:
  btnConvert->Enabled   = true;
  btnAbort->Enabled     = false;
  ProgressBar->Position = 0;
}//---------------------------------------------------------
void __fastcall TDxfModCnvForm::btnAbortClick(TObject*) {
  DxfModel->Aborted = true;
  ProgressBar->Position = 0;
}//---------------------------------------------------------
bool __fastcall TDxfModCnvForm::IsFeaLyer(string &lname) {
  if (lname.find("_") != NPOS) return true;
  return false;
}//---------------------------------------------------------
void __fastcall TDxfModCnvForm::MakeRCTable() {
  RCTable.Clear();
  TBlockIter BlockIter(DxfModel->BlockTable);
  if (BlockIter.home()) do {
     TLayIter LayIter(BlockIter.current()->LayTable);
     if (LayIter.home()) do {
        string lname = *LayIter.currentID();
        if (IsFeaLyer(lname)) RCTable[lname] = 0;
     } while(LayIter.next());
  } while(BlockIter.next());
  int rc = 1;
  ChainIter<int, string> RCIter(RCTable);
  for (int *pInt = RCIter.home(); pInt; pInt = RCIter.next())
     *pInt = rc++;
}//---------------------------------------------------------
void __fastcall TDxfModCnvForm::MakeVertList() {
  VertList.Clear();
  TBlock *Entities = &DxfModel->BlockTable["Entities"];
  TLayIter LayIter(Entities->LayTable);
  if (LayIter.home()) do {
     string lname = *LayIter.currentID();
     if (!IsFeaLyer(lname)) continue;
     TVertIter VertIter(VertList);
     TVertIter LVerIter(LayIter.current()->VertList);
     TextVector *pOld, *pNew;
     for (pNew = LVerIter.home(); pNew; pNew = LVerIter.next()) {
        for (pOld = VertIter.home(); pOld; pOld = VertIter.next())
           if (*pOld == *pNew) break;
        if (!pOld) VertList.add(*pNew);
     }
  } while(LayIter.next());
}//---------------------------------------------------------
void __fastcall TDxfModCnvForm::MakeFaceTable() {
  FaceCount = BadCount = DupCount = 0;
  FaceTable.Clear();
  TBlock *Entities = &DxfModel->BlockTable["Entities"];
  TLayIter LayIter(Entities->LayTable);
  if (LayIter.home()) do {
     string lname = *LayIter.currentID();
     if (!IsFeaLyer(lname)) continue;
     BadCount += LayIter.current()->BadCount;
     DupCount += LayIter.current()->DupCount;
     TVertList *VList = &LayIter.current()->VertList;
     TFaceList *pFaceList = &FaceTable[lname];
     TFaceIter FaceIter(LayIter.current()->FaceList);
     for (TFace *pF = FaceIter.home(); pF; pF = FaceIter.next()){
        TFace Face;
        TIndexIter IdxIter(*pF);
        for (DWORD *idx = IdxIter.home(); idx; idx = IdxIter.next()){
           TextVector *pV = VList->data(*idx);
           assert(pV);
           int idx = VertIndex(*pV);
           assert(idx);
           Face.AddVertex(idx);
        }
        pFaceList->add(Face);
        FaceCount++;
     }
  } while(LayIter.next());
  // TODO: listing detached faces (having one or zero only shared vertex)
}//---------------------------------------------------------
void __fastcall TDxfModCnvForm::MakeDFTable() {
  BadNodes = 0;
  DFTable.Clear();
  TBlock *Entities = &DxfModel->BlockTable["Entities"];
  TInsIter InsIter(Entities->InsList);
  if (InsIter.home()) do {
     TInsert *pIns = InsIter.current();
     if (pIns->bname == "FE-NODE") {
        TextVector tmp(TextVector(pIns->x,pIns->y,pIns->z));
        int idx = VertIndex(tmp);
        if (!idx) {
           BadNodes++;
           OutputDebugString("\r\n");
           OutputDebugString(AnsiString("Displaced node at: " + tmp.ToString()).c_str());
        }
        else
            DFTable[idx] = *pIns;
     }
  } while(InsIter.next());
}//---------------------------------------------------------
int __fastcall TDxfModCnvForm::VertIndex(const TextVector &v) {
  TextVector *pV; int result = 1;
  TVertIter VertIter(VertList);
  for (pV = VertIter.home(); pV; pV = VertIter.next()) {
     if (*pV == v)
        return result;
     result++;
  }
  return 0;
}//---------------------------------------------------------
void __fastcall TDxfModCnvForm::WriteMod(char *fname) {
  FILE *MOD = fopen(fname, "wt");
  fprintf(MOD,  "C**************************************************");
  fprintf(MOD,"\nC* Finite element interpreting of DXF files       *");
  fprintf(MOD,"\nC* Generated by utility of Ph.D. Shauki Z.Baghdadi*");
  fprintf(MOD,"\nC* COSMOS/M mod-file format                       *");
  fprintf(MOD,"\nC**************************************************");
  fprintf(MOD,"\nC* Total nodes .............. = %d", VertList.Count());
  fprintf(MOD,"\nC* Total shells ............. = %d", FaceCount);
  fprintf(MOD,"\nC* 3DFaces analysed ......... = %d", DxfModel->DxfFaceCount);
  fprintf(MOD,"\nC* Meshes  analysed ......... = %d", DxfModel->DxfMeshCount);
  fprintf(MOD,"\nC* Bad shells removed ....... = %d", BadCount);
  fprintf(MOD,"\nC* Duplicate shells removed . = %d", DupCount);
  fprintf(MOD,"\nC* Displaced node blocks .. . = %d", BadNodes);
  fprintf(MOD,"\nC* P R O P E R T I E S ============================");
  fprintf(MOD,"\nEG,   1, SHELL4T , 1, 0, 2, 0, 0, 0, 0, 0");
  fprintf(MOD,"\nEX,   1, 2.1E+011");
  fprintf(MOD,"\nNUXY, 1, 2.8E-01");
  fprintf(MOD,"\nGXY,  1, 7.9E+10");
  fprintf(MOD,"\nC* R E A L   C O N S T A N T S ====================");
  ChainIter<int, string> RCIter(RCTable);
  if (RCIter.home()) do {
     int rc = *RCIter.current();
     string lname = *RCIter.currentID();
     fprintf(MOD, "\nC* LAYER %s", lname.c_str());
     // LAYERNAME_THICK_15
     float t = atof(lname.substr(lname.rfind("_")+1).c_str());
     if (t<=0) t = 1;
     fprintf(MOD,"\nRC, 1, %d, %g", rc, t/1000.0); // in mm for EG 1
     fprintf(MOD,"\nRC, 2, %d, %g", rc, t/1000.0); // in mm for EG 2
  } while(RCIter.next());
  fprintf(MOD,"\nC* N O D E S ======================================");
  TextVector *pNode; int n = 1;
  TVertIter VertIter(VertList);
  for (pNode = VertIter.home(); pNode; pNode = VertIter.next())
     fprintf(MOD,"\nN,%d, %s,%s,%s",
                       n++, pNode->x.c_str(), pNode->y.c_str(), pNode->z.c_str());
  fprintf(MOD,"\nC* S H E L L S ====================================");
  int e = 1;
  if (FaceTable.home()) do {
     string lname = *FaceTable.currentID();
     fprintf(MOD,"\nACTIVE REAL %d", RCTable[lname]);
     TFaceIter FaceIter(*FaceTable.current());
     for (TFace *pF = FaceIter.home(); pF; pF = FaceIter.next()){
        TIndexIter IdxIter(*pF);
        fprintf(MOD,"\nE,%d ", e++);
        int four = 0;
        for (DWORD *idx = IdxIter.home(); idx; idx = IdxIter.next()) {
           fprintf(MOD,",%d", *idx);
           four++;
        }
        if (four < 4) fprintf(MOD,",%d", *IdxIter.home());
     }
  } while(FaceTable.next());
  fprintf(MOD,"\nC* B O U N D A R Y ================================");
  char *disp   = "UX UY UZ ROTX ROTY ROTZ";
  char *force  = "FX FY FZ";
  char *moment = "MX MY MZ";
  char *dtmpl  = "\nD,%d, %s,%g,,,,";
  char *ftmpl  = "\nF,%d, %s,%g,,,,";
  char *mtmpl  = "\nF,%d, %s,%g,,,,";
  if (DFTable.home()) do {
     float f;
     int n = *DFTable.currentID();
     TInsert *pIns = DFTable.current();
     TAttribIter AttIter(pIns->AttribList);
     if (AttIter.home()) do {
        string atname = *AttIter.currentID();
        string atval  = *AttIter.current();
        if (atval == "") continue;
        f = atof(atval.c_str());
        if (errno == ERANGE) continue;
        if (strstr(disp, atname.c_str()))
           fprintf(MOD, dtmpl , n, atname.c_str(), f);
        else
        if (strstr(force, atname.c_str()))
           fprintf(MOD, ftmpl , n, atname.c_str(), f);
        else
        if (strstr(moment, atname.c_str())) {
           //     if (atname == "MX") atname = "ROTX";
           //else if (atname == "MY") atname = "ROTY";
           //else if (atname == "MZ") atname = "ROTZ";
           fprintf(MOD, mtmpl , n, atname.c_str(), f);
        }
     } while (AttIter.next());
  } while (DFTable.next());

  fprintf(MOD, "\n\nRENUM ON");
  fclose(MOD);
}//---------------------------------------------------------
void __fastcall TDxfModCnvForm::WriteGeo(char *fname) {
  FILE *GEO = fopen(fname, "wt");
  fprintf(GEO,  "C**************************************************");
  fprintf(GEO,"\nC* Finite element interpreting of DXF files       *");
  fprintf(GEO,"\nC* Generated by utility of Ph.D. Shauki Z.Baghdadi*");
  fprintf(GEO,"\nC* COSMOS/M GeoStar file format                   *");
  fprintf(GEO,"\nC**************************************************");
  fprintf(GEO,"\nC* Total nodes .............. = %d", VertList.Count());
  fprintf(GEO,"\nC* Total shells ............. = %d", FaceCount);
  fprintf(GEO,"\nC* 3DFaces analysed ......... = %d", DxfModel->DxfFaceCount);
  fprintf(GEO,"\nC* Meshes  analysed ......... = %d", DxfModel->DxfMeshCount);
  fprintf(GEO,"\nC* Bad shells removed ....... = %d", BadCount);
  fprintf(GEO,"\nC* Duplicate shells removed . = %d", DupCount); // TODO: printing duplicates for finding them
  fprintf(GEO,"\nC* Displaced node blocks .. . = %d", BadNodes);
  fprintf(GEO,"\nC* P R O P E R T I E S ============================");
  fprintf(GEO,"\nEGROUP,   1, SHELL3 , 0, 0,0,0, 0,0,0,0");
  fprintf(GEO,"\nEGROUP,   2, SHELL4 , 2, 0,0,0, 0,0,0,0");
  fprintf(GEO,"\nMPROP, 1, EX,  2.1E+11");
  fprintf(GEO,"\nMPROP, 1, EY,  2.1E+11");
  fprintf(GEO,"\nMPROP, 1, EZ,  2.1E+11");
  fprintf(GEO,"\nMPROP, 1, NUXY,  2.8E-01");
  fprintf(GEO,"\nMPROP, 1, NUYZ,  2.8E-01");
  fprintf(GEO,"\nMPROP, 1, NUXZ,  2.8E-01");
  fprintf(GEO,"\nMPROP, 1, GXY,  7.8E+10");
  fprintf(GEO,"\nMPROP, 1, GYZ,  7.8E+10");
  fprintf(GEO,"\nMPROP, 1, GXZ,  7.8E+10");
  fprintf(GEO,"\nMPROP, 1, DENS,  7800");
  fprintf(GEO,"\nC* R E A L   C O N S T A N T S ====================");
  ChainIter<int, string> RCIter(RCTable);
  if (RCIter.home()) do {
     int rc = *RCIter.current();
     string lname = *RCIter.currentID();
     fprintf(GEO, "\nC* LAYER %s", lname.c_str());
     // LAYERNAME_THICK_15
     float t = atof(lname.substr(lname.rfind("_")+1).c_str());
     if (t<=0) t = 1;
     fprintf(GEO,"\nRCONST, 1, %d, 1,1, %g", rc, t/1000.0); // in mm for EGROUP1
     fprintf(GEO,"\nRCONST, 2, %d, 1,1, %g", rc, t/1000.0); // in mm for EGROUP2
  } while(RCIter.next());
  fprintf(GEO,"\nC* N O D E S ======================================");
  TextVector *pNode; int n = 1;
  TVertIter VertIter(VertList);
  for (pNode = VertIter.home(); pNode; pNode = VertIter.next())
     fprintf(GEO,"\nND,%d, %s,%s,%s, 0,0,0,0,0,0",
                       n++, pNode->x.c_str(), pNode->y.c_str(), pNode->z.c_str());
  fprintf(GEO,"\nC* S H E L L S ====================================");
  int e = 1;
  if (FaceTable.home()) do {
     string lname = *FaceTable.currentID();
     fprintf(GEO,"\nACTSET,RC, %d", RCTable[lname]);
     TFaceIter FaceIter(*FaceTable.current());
     for (TFace *pF = FaceIter.home(); pF; pF = FaceIter.next()){
        int cnt =  pF->count();
        if (3 == cnt) {
           fprintf(GEO,"\nACTSET,EG,1");
        } else {
            fprintf(GEO,"\nACTSET,EG,2");
        }
        TIndexIter IdxIter(*pF);
        fprintf(GEO,"\nEL,%d, SF,0,%d ", e++, cnt);
        for (DWORD *idx = IdxIter.home(); idx; idx = IdxIter.next()) {
           fprintf(GEO,",%d", *idx);
        }
        fprintf(GEO,",0,0,0,0,0,0");
     }
  } while(FaceTable.next());
  fprintf(GEO,"\nC* B O U N D A R Y ================================");
  char *disp   = "UX UY UZ ROTX ROTY ROTZ";
  char *force  = "FX FY FZ";
  char *moment = "MX MY MZ";
  char *dtmpl  = "\nDND,%d, %s,%g,%d,1";
  char *ftmpl  = "\nFND,%d, %s,%g,%d,1";
  char *mtmpl  = "\nFND,%d, %s,%g,%d,1";
  if (DFTable.home()) do {
     float f;
     int n = *DFTable.currentID();
     TInsert *pIns = DFTable.current();
     TAttribIter AttIter(pIns->AttribList);
     if (AttIter.home()) do {
        string atname = *AttIter.currentID();
        string atval  = *AttIter.current();
        if (atval == "") continue;
        f = atof(atval.c_str());
        if (errno == ERANGE) continue;
        if (strstr(disp, atname.c_str())) {
                if (atname == "ROTX") atname = "RX";
           else if (atname == "ROTY") atname = "RY";
           else if (atname == "ROTZ") atname = "RZ";
           fprintf(GEO, dtmpl , n, atname.c_str(), f, n);
        } else
        if (strstr(force, atname.c_str()))
           fprintf(GEO, ftmpl , n, atname.c_str(), f, n);
        else
        if (strstr(moment, atname.c_str())) {
           fprintf(GEO, mtmpl , n, atname.c_str(), f, n);
        }
     } while (AttIter.next());
  } while (DFTable.next());

  // RENUM is called by STAR and DSTAR to renumber the nodes in order to
  // minimize the bandwidth of the assembled stiffness matrix. Reduction in the
  // bandwidth results in a substantial reduction in the computational time and
  // resources required to solve the problem.
  fprintf(GEO, "\n\nRENUM OFF"); // want to keep my IDs 


  fprintf(GEO, "\n\nA_STATIC,G,0,0,1E-006,1E+010,0,0,0,0,0,0,0,0,0,0,0,0");
  // Loading flag
  // Inplace effect flag
  // Soft spring flag
  // Soft spring value
  // Bcs stiffness value
  // Save stiff matrix flag
  // Form stiff matrix flag
  // Update coordinates flag
  // End moment flag for Shell elements
  // Grid force balance
  // Inertia relief
  // Rigid connections
  // Solver option (sparce, skyline, iterative)
  // Solution accuracy
  // Change to 2nd order
  // Initial contact clearance
  // Large displacement

  fprintf(GEO, "\nHIDE_OPT,1,0,1");
  fprintf(GEO, "\n\nACTSET LC 1");

  fclose(GEO);
}//---------------------------------------------------------
