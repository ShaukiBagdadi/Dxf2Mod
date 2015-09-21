//----------------------------------------------------------
// FILE:    LinkList.hpp
// AUTHOR:  Ph.D. Shauki Z. Baghdadi
//----------------------------------------------------------
// �����, ��������� �������� ����� ��������� � ������
// ������ ����� ������������ �������� ������������
// � ����������� �� ���������
//----------------------------------------------------------
#ifndef LinkListHPP
#define LinkListHPP
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
template <class T> class _LINK_AS LinkList;
//----------------------------------------------------------
// ��������� ����� �������� ����� ������
template <class T> class _LINK_AS LinkEl {
public:
  T Data;
  LinkEl *next, *prev;
  LinkEl() {          // �������� ��������� ��
     next = NULL;     // ��������� ������� ������
     prev = NULL;     // � �� ����������
  }
  ~LinkEl() {}        // ��������� ��� ������� Clear()
};
//----------------------------------------------------------
// ����� ��� ������ ������ LinkList. ������� ��� ������-
// ������ ������ ������� ������ LinkList ����� ����� ������
// ������������ �� ��������� �������� ��� ��������� ������.
template <class T> class _LINK_AS LinkIter {
  LinkEl<T> *FCurrent;
  LinkList<T> *FList;
public:
  // ���������������� �� ������� �� ������������ ������:
  LinkIter(LinkList<T> &list) {
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

  UINT currentID() {
     if (FCurrent) return FList->index(FCurrent->Data);
     return 0;
  }

  T *next() {
     if (FCurrent) FCurrent = FCurrent->next;
     if (FCurrent) return &FCurrent->Data;
     return NULL;
  }

  // T* operator ++() {return next();} //preincrement

  //T* operator ++(int) { //postincrement:
  //   T* result = current();
  //   next();
  //   return result;
  //}

};
//----------------------------------------------------------
// ��������������� ������� ������
template <class T> class _LINK_AS LinkList {
  // ������� ������; ��������� �� ������, ��������� �
  // ������� �������� ������
  LinkEl<T> *FFirst, *FLast, *FCurrent;
  // ������� ���������� ��������� � ������
  UINT FCount;
  friend class LinkIter<T>;
public:
  LinkList() {
     FFirst = FLast = FCurrent = NULL;
     FCount = 0;
  }

  ~LinkList() { Clear(); }

  LinkEl<T> *first() const { return FFirst; }
  LinkEl<T> *store(UINT index, T &data);

  T *add(T &data);

  T *addAt(UINT index, T &data);

  LinkEl<T> *find(UINT index);

  void remove(UINT index, int del = 1);

  virtual void Clear();

  UINT index(const T &data);

  T *data(UINT index);

  UINT count() const { return FCount; }
  UINT Count() const { return FCount; }

  T *first() {
     if (FFirst) return &FFirst->Data;
     return NULL;
  }

  T *last() {
     if (FLast) return &FLast->Data;
     return NULL;
  }

  T *end() {
     FCurrent = FLast;
     if (FCurrent) return &FCurrent->Data;
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

  UINT currentID() {
     if (FCurrent) return index(FCurrent->Data);
     return 0;
  }

  T *next() {
     if (FCurrent) FCurrent = FCurrent->next;
     if (FCurrent) return &FCurrent->Data;
     return NULL;
  }

  T* operator[](UINT index);

  //T* operator ++() { return next(); } //preincrement

  //T* operator ++(int) { //postincrement:
  //   T* result = current();
  //   next();
  //   return result;
  //}

  // �������� ����������� �������:
  LinkList& operator = (const LinkList &n);
};
//----------------------------------------------------------
// ������� ������ ����� � ������ �� ��������� �������
template <class T> LinkEl<T> *LinkList<T>::
  store(UINT index, T &data)
{
  if (index - FCount > 1) return NULL;
  LinkEl<T> *p = find(index);
  if (!p) {
     p = new LinkEl<T>;
     FCount++;
  }
  // ��� �������� ������ �� ������ ������� ���������
  // ��������� ����� �������� ��� ������ ������������
  // ��������. ��� �������� ������� ������� �������
  // ��������� ���������� FCount
  if (!p) {
     FCount--;
     return NULL;
  }
  p->Data = data;
  if (!FFirst) {
     FLast = FFirst = p;
  }
  else {
     p->prev     = FLast;
     FLast->next = p;
     FLast       = p;
  }
  return p;
}//---------------------------------------------------------
// ������� ������ �������� � ����� ������
template <class T> T *LinkList<T>::add(T &data)
{
  LinkEl<T> *p = new LinkEl<T>;
  if (!p) return NULL;
  FCount++;
  p->Data = data;
  if (!FFirst) {
     FLast = FFirst = p;
  }
  else {
     p->prev     = FLast;
     FLast->next = p;
     FLast       = p;
  }
  return &p->Data;
}//---------------------------------------------------------
// ������� ������ �������� � ������������ ����� ������
template <class T> T *LinkList<T>::addAt(UINT index, T &data)
{
  if (index - FCount == 1) return add(data);
  LinkEl<T> *p = new LinkEl<T>;
  if (!p) return NULL;
  FCount++;
  p->Data = data;
  LinkEl<T> *l = find(index);
  p->prev = l->prev;
  p->next = l;
  l->prev = p;
  if(p->prev) p->prev->next = p;
  return &p->Data;
}//---------------------------------------------------------
//���������� ��������� �� ����� � ��������� ��������
template <class T> LinkEl<T> *LinkList<T>::
  find(UINT index)
{
  UINT curindex = 1;
  LinkEl<T> *temp;
  temp = FFirst;
  while (temp && (curindex != index)) {
     curindex++;
     temp = temp->next;
  }
  if (temp) return temp;
  return NULL;
}//---------------------------------------------------------
// ������� �� ������ ����� � ��������� ��������
// � ���������� ���. ����� ��������� ����� � ������, ����
// ������ ��������� del ������� ��������
template <class T> void LinkList<T>::
  remove(UINT index, int del)
{
  LinkEl<T> *ob;
  ob = find(index);
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
template <class T> void LinkList<T>::Clear()
{
  LinkEl<T> *p, *p1;
  p = FFirst;
  while(p) {
     p1 = p->next;
     delete p;
     p = p1;
  }
  FFirst = FLast = FCurrent = NULL;
  FCount = 0;
}//---------------------------------------------------------
// ���������� �������� ������� ��� �������� ������
template <class T> UINT LinkList<T>::index(const T &data)
{
  UINT curindex = 1;
  LinkEl<T> *temp;
  temp = FFirst;
  while (temp) {
     if (&data == &temp->Data) return curindex;
     temp = temp->next;
     curindex++;
  }
  return 0;
}//---------------------------------------------------------
// ���������� ��������� �� ������ �� ��������� �������
template <class T> T *LinkList<T>::data(UINT index)
{
  UINT curindex = 1;
  LinkEl<T> *temp;
  temp = FFirst;
  while (temp && (curindex != index)) {
     temp = temp->next;
     curindex++;
  }
  if (temp) return &temp->Data;
  return NULL;
}//---------------------------------------------------------
// ���������� ��������� ���������� � ������������ ���������
// �� ������ �� ��������� �������
template <class T> T *LinkList<T>::operator[](UINT index)
{
  LinkEl<T> *ob;
  ob = find(index);
  if (ob) return &ob->Data;
  return NULL;
}//---------------------------------------------------------
template <class T> LinkList<T> &LinkList<T>::
  operator = (const LinkList<T> &n)
{
  // ���� ������ ����������� �� �� const ������� ��� ��
  // ������������� � const ���������:
  LinkEl<T> *HisFirst = n.first();
  if(this != &n) {
     Clear();
     while (HisFirst) {
        add(HisFirst->Data);
        HisFirst = HisFirst->next;
     }
  }
  return *this;
}//---------------------------------------------------------
#endif