//----------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop
#include "MMaker.h"
//----------------------------------------------------------
#include "_Face.cpp"
//----------------------------------------------------------
// T M e s h M a k e r
//----------------------------------------------------------
void TMeshMaker::Clear() {
  if (vlist) delete[] vlist;
  if (flist) delete[] flist;
  FaceList.Clear();
  VertList.Clear();
  flist_len = 0;
  BadCount  = 0;
  DupCount  = 0;
  flist = NULL;
  vlist = NULL;
}//---------------------------------------------------------
DWORD TMeshMaker::AddVertex(TextVector &Vec) {
  TVertIter iter(VertList);
  TextVector *ptr; DWORD idx = 1;
  for (ptr = iter.home(); ptr; ptr = iter.next()) {
     if (*ptr == Vec) return idx;
     idx++;
  }
  VertList.add(Vec);
  return VertList.Count();
}//---------------------------------------------------------
DWORD TMeshMaker::AddFace(TFace &Face) {
  if (!Face.Valid()) {BadCount++; return 0;}
  TFaceIter iter(FaceList);
  DWORD idx = 1;
  for (TFace *ptr = iter.home(); ptr; ptr = iter.next()) {
     // TODO: equality of faces is not correct for very big models
     if (*ptr == Face) {
        DupCount++;
        OutputDebugString("\r\n");
        OutputDebugString(AnsiString("Trying to add face: " + Face.IndexesList()).c_str());
        OutputDebugString(AnsiString("Existing duplicate: " + ptr->IndexesList()).c_str());
        return idx;
     }
     idx++;
  }
  FaceList.add(Face);
  flist_len += Face.count() + 1;
  return FaceList.Count();
}//---------------------------------------------------------
int TMeshMaker::MakeMeshLists(bool DoubleSided) {
  delete[] flist;
  delete[] vlist;

  UINT size = VertList.Count();
  if (!size) return 0;
  vlist = new TextVector[size];
  TVertIter viter(VertList);
  TextVector *vptr; DWORD vidx = 0;
  for (vptr = viter.home(); vptr; vptr = viter.next())
     vlist[vidx++] = *vptr;

  size = FaceList.Count();
  if (!size) return 0;
  // «аполнение массива со списком граней:
  if (DoubleSided)
     flist = new DWORD[2*flist_len+1];
  else
     flist = new DWORD[flist_len+1];
  UINT fc = 0;
  // начальный индекс faces равен 1
  for (UINT i = 1; i <= size; i++) {
     TFace* face = FaceList.data(i);
     UINT vc = face->count();
     flist[fc++] = vc;
     for (UINT v = 1; v <= vc; v++)
        flist[fc++] = *face->data(v)-1;
     if (DoubleSided) {
        flist[fc++] = vc;
        for (UINT v = vc; v >= 1; v--)
           flist[fc++] = *face->data(v)-1;
     }
  }
  flist[fc++] = 0; //  онец списка
  return fc;
}//---------------------------------------------------------
