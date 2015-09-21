//----------------------------------------------------------
#include <math.h>
//----------------------------------------------------------
#pragma hdrstop
#include "DxfModel.h"
//----------------------------------------------------------
#include "_Read.cpp"
#include "_Blocks.cpp"
#include "_Entit.cpp"
#include "_RFace.cpp"
#include "_RPoly.cpp"
#include "_RInsert.cpp"
//----------------------------------------------------------
// T P a i r
//----------------------------------------------------------
int  operator == (const TPair& a, const TPair& b) {
   if (a.key != b.key) return 0;
   if (strcmp(a.val, b.val) == 0) return 1;
   return 0;
}//---------------------------------------------------------
int  operator != (const TPair& a, const TPair& b) {
   return(!(a == b));
}//---------------------------------------------------------
// T I n s e r t
//----------------------------------------------------------
TInsert::TInsert() : bname(), lname() {
  rows = cols = 1;
  scalex = scaley = scalez = 1.0;
  x = y = z = "0";
  angle = rstep = cstep = 0.0;
}//---------------------------------------------------------
TInsert::~TInsert() {
  AttribList.Clear();
}//---------------------------------------------------------
// T B l o c k
//----------------------------------------------------------
TBlock::TBlock() {
  x = y = z = 0.0;
}//---------------------------------------------------------
TBlock::~TBlock() {
  InsList.Clear();
  LayTable.Clear();
}//---------------------------------------------------------
// T D x f M e s h M o d e l
//----------------------------------------------------------
TDxfMeshModel::TDxfMeshModel() :
  SECTION (0,"SECTION"),  ENDSEC  (0,"ENDSEC"),
  BLOCK   (0,"BLOCK"),    ENDBLK  (0,"ENDBLK"),
  BLOCKS  (2,"BLOCKS"),   ENTITIES(2,"ENTITIES"),
  _EOF    (0,"EOF"),      INSERT  (0,"INSERT"),
  _3DFACE (0,"3DFACE"),   SEQEND  (0,"SEQEND"),
  POLYLINE(0,"POLYLINE"), VERTEX  (0,"VERTEX"),
  ATTRIB  (0, "ATTRIB")
  {
  CurBlock     = NULL;
  CurMaker     = NULL;
  OnProgress   = NULL;
  Aborted      = false;
  DoSkipBlocks = false;
}//---------------------------------------------------------
TDxfMeshModel::~TDxfMeshModel() {
  Clear();
}//---------------------------------------------------------
void TDxfMeshModel::Clear() {
  stream.close();
  BlockTable.Clear();
  CurBlock = NULL;
  CurMaker = NULL;
  Aborted  = false;
  DxfFaceCount = DxfMeshCount = 0;
}//---------------------------------------------------------
