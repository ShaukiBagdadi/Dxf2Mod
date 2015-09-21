//----------------------------------------------------------
#ifndef DxfModelH
#define DxfModelH
#include <fstream.h>
#include <cstring.h>
#include "MMaker.h"
#include "ChainLst.hpp"
//----------------------------------------------------------
typedef void (*TProgressFunc) (long now, long total);
//----------------------------------------------------------
// T P a i r
//----------------------------------------------------------
struct TPair {
  int  key;
  char val[255];
  TPair () {memset(this, 0, sizeof(TPair));}
  TPair (int k, char* n) {
     key = k;
     strncpy(val, n, sizeof(val));
  }
  friend int operator == (const TPair& a, const TPair& b);
  friend int operator != (const TPair& a, const TPair& b);
};//--------------------------------------------------------
typedef ChainList<string, string> TAttribList;
typedef ChainIter<string, string> TAttribIter;
//----------------------------------------------------------
// T I n s e r t
//----------------------------------------------------------
struct TInsert {
  string bname;                  // название вставляемого блока
  string lname;                  // имя слоя
  AnsiString  x, y, z;           // положение
  float  scalex, scaley, scalez; // масштабы
  float  angle;                  // угол повоота при вставке
  int    rows, cols;             // количество рядов вставки
  float  rstep, cstep;           // шаг между рядами
  TAttribList AttribList;        // таблица атрибутов
  TInsert();
  ~TInsert();
};//--------------------------------------------------------
typedef ChainList<TMeshMaker, string> TLayTable;
typedef ChainIter<TMeshMaker, string> TLayIter;
typedef LinkList<TInsert>             TInsList;
typedef LinkIter<TInsert>             TInsIter;
//----------------------------------------------------------
// T B l o c k
//----------------------------------------------------------
struct TBlock {
  float x, y, z;       // координаты вставки блока
  TInsList  InsList;   // список вложенных блоков
  TLayTable LayTable;  // таблица используемых слоев
  TBlock();
  ~TBlock();
};//--------------------------------------------------------
typedef ChainList<TBlock, string> TBlockTable;
typedef ChainIter<TBlock, string> TBlockIter;
//----------------------------------------------------------
// T D x f M e s h M o d e l
//----------------------------------------------------------
class TDxfMeshModel {
private:
  TPair pair, SECTION, ENDSEC, BLOCK, ENDBLK, BLOCKS, ATTRIB,
  ENTITIES, _EOF, INSERT, _3DFACE, SEQEND, POLYLINE, VERTEX;

  TBlock     *CurBlock;
  TMeshMaker *CurMaker;

  fstream stream;
  long streamsize;

  void Clear();
  void GetPair();
  void SkipEntity();
  void ReadFace();
  void ReadPoly();
  void ReadAttrib(TInsert &ins);
  void ReadInsert();
  void ReadBlock();
  void ReadBlocks();
  void ReadEntities();
public:
  bool DoSkipBlocks;
  int DxfFaceCount, DxfMeshCount;
  TDxfMeshModel();
  ~TDxfMeshModel();
  void ReadFromFile(char *fname);
  TBlockTable BlockTable;
  bool Aborted;
  TProgressFunc OnProgress;
};//--------------------------------------------------------
#endif
