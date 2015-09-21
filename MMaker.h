//----------------------------------------------------------
#ifndef MMakerH
#define MMakerH
#include <cstring.h>
#include <vcl/dstring.h>
#include "LinkList.hpp"
//----------------------------------------------------------
#ifndef _DWORD_DEFINED
#define DWORD unsigned long
#endif
//----------------------------------------------------------
// V E C T O R
//----------------------------------------------------------
#ifndef __D3DRMWIN_H__
struct TextVector {
  // TODO: DraftSight does not allow changing precision in DXF output
  // Comparing as text avoids precision mismatch issues
  AnsiString x, y, z;
  TextVector () {x = y = z = "0";}
  TextVector (AnsiString f) {x = y = z = f;}
  TextVector (AnsiString a, AnsiString b, AnsiString c) {x = a; y = b; z = c;}
  AnsiString ToString() const {
     return x + "," + y + "," + z;
  }
//  bool operator ==(const TextVector& rhs) const {
//     return x==rhs.x && y==rhs.y && z == rhs.z;
//  }
  friend int operator == (const TextVector& a, const TextVector& b){
     return a.x==b.x && a.y==b.y && a.z == b.z;
  }
};//--------------------------------------------------------
#else
#define VECTOR D3DVECTOR
#endif // __D3DRMWIN_H__
//----------------------------------------------------------
typedef LinkList<DWORD>  TIndexList;
typedef LinkIter<DWORD>  TIndexIter;
typedef LinkList<TextVector> TVertList;
typedef LinkIter<TextVector> TVertIter;
//----------------------------------------------------------
// T F a c e
//----------------------------------------------------------
class TFace : public TIndexList {
  DWORD dwUnique;
public:
  TFace() : TIndexList() {
     dwUnique = 0;
     Color    = 7; // AutoCAD white color
  }
  int      Color;
  bool     Valid();
  DWORD    Unique() const { return dwUnique; }
  DWORD    AddVertex(DWORD vindex);
  AnsiString IndexesList();
  TFace& operator = (const TFace& n);
  friend int operator == (const TFace& a, const TFace& b);
};//--------------------------------------------------------
typedef LinkList<TFace>  TFaceList;
typedef LinkIter<TFace>  TFaceIter;
//----------------------------------------------------------
// T M e s h M a k e r
//----------------------------------------------------------
class TMeshMaker {
  DWORD flist_len;
public:
  DWORD BadCount, DupCount;
  TMeshMaker(){
     flist_len = 0;
     flist = NULL;
     vlist = NULL;
  }
  ~TMeshMaker(){Clear();}
  void  Clear();
  int   MakeMeshLists(bool DoubleSided = false);
  DWORD AddVertex(TextVector &Vec);
  DWORD AddFace(TFace &Face);
  DWORD vcount() { return VertList.count(); }
  DWORD *flist;
  TextVector *vlist;
  TVertList VertList;
  TFaceList FaceList;
};//--------------------------------------------------------
#endif