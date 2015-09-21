//----------------------------------------------------------
// FILE:    ChainLst.hpp
// AUTHOR:  Ph.D. Shauki Z. Baghdadi
//----------------------------------------------------------
// �����, ������������ ��� ��������������� ������ �����
// ������������ ��������� ��������� � ������������,
// � ����������� �� ���������
// �����, ��������� �������� ����� ��������� � ������
// ������ ����� ������������ �������� ������������
// � ����������� �� ���������
//----------------------------------------------------------
#ifndef ChainLstHPP
#define ChainLstHPP
//----------------------------------------------------------
#include <stddef.h>
#ifndef UINT
typedef unsigned int UINT;
#endif
//----------------------------------------------------------
// ���� ��������, ����������� ������������ ����� ���
// ����������� � ������������ ���������
#if defined(__DLL__)
   #define _LINK_AS _export
#elif defined(_IMPORT)
   #define _LINK_AS _import
#else
   #define _LINK_AS
#endif
//----------------------------------------------------------
template <class T, class idT> class _LINK_AS ChainList;
//----------------------------------------------------------
// ��������� ����� "������������������� �������"
template <class T, class idT> class _LINK_AS IdentOb {
public:      // � ���� ������ ��� ���� � ������ ������������
  idT id;    // "�������������" �������
  T Data;    // ��� ������
  IdentOb(){}// ����� ����������� �� ��������� �.�.
};
//----------------------------------------------------------
// ��������� ����� �������� ����� ������ "������������������
// ��������" ����������� �� ������
// "������������������ ������"
template <class T, class idT>
class _LINK_AS ChainOb: public IdentOb<T, idT> {
public:
  ChainOb *next, *prev;
  ChainOb() {          // �������� ��������� ��
     next = NULL;      // ��������� ������� ������
     prev = NULL;      // � �� ����������
  }
  ~ChainOb() {}        // ��������� ��� ������� Clear()
};
//----------------------------------------------------------
// ����� ��� ������ ������ ChainList. ������� ��� ������-
// ������ ������ ������� ������ ChainList ����� ����� ������
// ������������ �� ��������� �������� ��� ��������� ������.
template <class T, class idT> class _LINK_AS ChainIter {
  ChainOb<T, idT> *FCurrent;
  ChainList<T, idT> *FList;
public:
  // ���������������� �� ������� �� ������������ ������:
  ChainIter(ChainList<T, idT> &list) {
     FList = &list;
     FCurrent = FList->FFirst;
  }

  T *home() {
     FCurrent = FList->FFirst;
     if (FCurrent) return &FCurrent->Data;
     return NULL;
  }

  T *current() const {
     if (FCurrent) return &FCurrent->Data;
     return NULL;
  }

  idT *currentID() const {
     if (FCurrent) return &FCurrent->id;
     return NULL;
  }

  T *next() {
     if (FCurrent) FCurrent = FCurrent->next;
     if (FCurrent) return &FCurrent->Data;
     return NULL;
  }

  //T* operator ++() {return next();} //preincrement

  //T* operator ++(int) { //postincrement:
  //   T* result = current();
  //   next();
  //   return result;
  //}

};
//----------------------------------------------------------
// ��������������� ������� ������ "������������������
// ��������"
template <class T, class idT> class _LINK_AS ChainList {
  // ������� ������; ��������� �� ������, ��������� �
  // ������� �������� ������
  ChainOb<T, idT> *FFirst, *FLast, *FCurrent;
  // ������� ���������� ��������� � ������
  UINT FCount;
  friend class ChainIter<T, idT>;
public:
  ChainList()  {FFirst=FLast=FCurrent = NULL; FCount = 0;}
  ~ChainList() {Clear();}

  ChainOb<T, idT> *first() const {return FFirst;}
  ChainOb<T, idT> *store(idT &ident, T &data);
  ChainOb<T, idT> *find(const idT &ident);

  void remove(idT &ident, int del = 1);

  virtual void Clear();

  idT *index(const T &data);

  T *data(const idT &ident);

  UINT count() const {return FCount;}
  UINT Count() const {return FCount;}

  void end() {FCurrent = FLast;}

  void moveto(const T &data);


  T *first() {
     if (FFirst) return &FFirst->Data;
     return NULL;
  }

  T *last() {
     if (FLast) return &FLast->Data;
     return NULL;
  }

  T *home() {
     FCurrent = FFirst;
     if (FCurrent) return &FCurrent->Data;
     return NULL;
  }

  T *current() const {
     if (FCurrent) return &FCurrent->Data;
     return NULL;
  }

  idT *currentID() const {
     if (FCurrent) return &FCurrent->id;
     return NULL;
  }

  T *next() {
     if (FCurrent) FCurrent = FCurrent->next;
     if (FCurrent) return &FCurrent->Data;
     return NULL;
  }

  T& operator[](idT ident);

  // T* operator ++() {return next();} //preincrement

  //T* operator ++(int) { //postincrement:
  //   T* result = current();
  //   next();
  //   return result;
  //}

  // �������� ����������� �������:
  ChainList& operator = (const ChainList &n);
};
//----------------------------------------------------------
// ������� ������ ����� � ������
template <class T, class idT> ChainOb<T, idT>
*ChainList<T, idT>::
store(idT &ident, T &data) {
  ChainOb<T, idT> *p;
  p = new ChainOb<T, idT>;
  // ��� �������� ������ �� ������ ������� ���������
  // ��������� ����� �������� ��� ������ ������������
  // ��������. ��� �������� ������� ������� �������
  // ��������� ���������� FCount
  if (!p) {
     FCount--;
     p = FLast;
  }
  p->Data = data;
  p->id   = ident;
  FCount++;
  if (!FFirst) {
     FLast = FFirst = p;
  }
  else {
     p->prev    = FLast;
     FLast->next = p;
     FLast       = p;
  }
  return p;
}//---------------------------------------------------------
//���������� ��������� �� ����� � ��������� ���������������
template <class T, class idT> ChainOb<T, idT>
*ChainList<T, idT>::
find(const idT &ident) {
  ChainOb<T, idT> *temp;
  temp = FFirst;
  while (temp) {
     if (ident == temp->id) return temp;
     temp = temp->next;
  }
  return NULL;
}//---------------------------------------------------------
// ������� �� ������ ����� � ��������� ���������������
// � ���������� ���. ����� ��������� ����� � ������, ����
// ������ ��������� del ������� ��������
template <class T, class idT> void ChainList<T, idT>::
remove(idT &ident, int del) {
  ChainOb<T, idT> *ob;
  ob = find(ident);
  if (!ob) return;

  if (ob->prev) {
     ob->prev->next = ob->next;
     if (ob->next) ob->next->prev = ob->prev;
     else FLast = ob->prev;
  }
  else { // ob->prev = NULL
     if (ob->next) {
        ob->next->prev = NULL;
        FFirst = ob->next;
     }
     else FFirst = FLast = NULL;
  }
  if (del) delete ob;
  FCount--;
}//---------------------------------------------------------
// ������� ��� ������ � ������, ����������� ������� ���
// ������ � ������������� ���� ������ � ������� ��������
template <class T, class idT> void ChainList<T, idT>::
Clear() {
  ChainOb<T, idT> *p, *p1;
  p = FFirst;
  while(p) {
     p1 = p->next;
     delete p;
     p = p1;
  }
  FFirst = FLast = FCurrent = NULL;
  FCount = 0;
}//---------------------------------------------------------
// ���������� �������� �������������� ��� ������
template <class T, class idT> idT *ChainList<T, idT>::
index(const T &data) {
  ChainOb<T, idT> *temp;
  temp = FFirst;
  while (temp) {
     if (&data == &temp->Data) return &temp->id;
     temp = temp->next;
  }
  return NULL;
}//---------------------------------------------------------
template <class T, class idT> void ChainList<T, idT>::
moveto(const T &data) {
  ChainOb<T, idT> *temp;
  temp = FFirst;
  while (temp) {
     if (&data == &temp->Data) break;
     temp = temp->next;
  }
  FCurrent = temp;
  if (!FCurrent) FCurrent = FFirst;
}//---------------------------------------------------------
//���������� ��������� �� ������ �� ��������� ��������������
template <class T, class idT> T *ChainList<T, idT>::
data(const idT &ident) {
  ChainOb<T, idT> *temp;
  temp = FFirst;
  while (temp) {
     if (ident == temp->id) return &temp->Data;
     temp = temp->next;
  }
  return NULL;
}//---------------------------------------------------------
// ���������� ��������� ���������� � ������������ ������ ��
// ������
template <class T, class idT> T &ChainList<T, idT>::
operator[](idT ident) {
  ChainOb<T, idT> *ob;
  ob = find(ident);
  // ���� ������ �� ������� ident ���, �� ������� �����
  // ����� � ���� �������� � �������������� ��� �����
  if (!ob) {
     T data;
     ob = store(ident, data);
  }
  return ob->Data;
}//---------------------------------------------------------
template <class T, class idT> ChainList<T, idT> &ChainList<T, idT>::
operator = (const ChainList<T, idT> &n) {
  // ���� ������ ����������� �� �� const ������� ��� ��
  // ������������� � const ���������:
  ChainOb<T, idT> *HisFirst = n.first();
  if(this != &n) {
     Clear();
     while (HisFirst) {
        idT id   = HisFirst->id;
        T   Data = HisFirst->Data;
        store(id, Data);
        HisFirst = HisFirst->next;
     }
  }
  return *this;
}//---------------------------------------------------------
#endif