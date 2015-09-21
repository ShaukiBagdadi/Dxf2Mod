//----------------------------------------------------------
// FILE:    ChainLst.hpp
// AUTHOR:  Ph.D. Shauki Z. Baghdadi
//----------------------------------------------------------
// Класс, используемый для идентификаторов должен иметь
// определенные операторы равенства и присваивания,
// и конструктор по умолчанию
// Класс, экземляры которого будут храниться в списке
// должны иметь определенный оператор присваивания
// и конструктор по умолчанию
//----------------------------------------------------------
#ifndef ChainLstHPP
#define ChainLstHPP
//----------------------------------------------------------
#include <stddef.h>
#ifndef UINT
typedef unsigned int UINT;
#endif
//----------------------------------------------------------
// Блок директив, позволяющих использовать класс для
// статических и динамических библиотек
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
// Шаблонный класс "идентифицированного объекта"
template <class T, class idT> class _LINK_AS IdentOb {
public:      // в этом классе все поля и методы общедоступны
  idT id;    // "идентификатор" объекта
  T Data;    // сам объект
  IdentOb(){}// нужен конструктор по умолчанию т.к.
};
//----------------------------------------------------------
// Шаблонный класс элемента звена списка "идентифицированных
// объектов" наследуется от класса
// "идентифицированный объект"
template <class T, class idT>
class _LINK_AS ChainOb: public IdentOb<T, idT> {
public:
  ChainOb *next, *prev;
  ChainOb() {          // содержит указатели на
     next = NULL;      // следующий элемент списка
     prev = NULL;      // и на предыдущий
  }
  ~ChainOb() {}        // необходим для очистки Clear()
};
//----------------------------------------------------------
// Класс для обхода списка ChainList. Следует его исполь-
// зовать вместо методов самого ChainList когда обход списка
// производится во вложенных функциях или вложенных циклах.
template <class T, class idT> class _LINK_AS ChainIter {
  ChainOb<T, idT> *FCurrent;
  ChainList<T, idT> *FList;
public:
  // Инициализируется он ссылкой на существующий список:
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
// Двунаправленный связный список "идентифицированных
// объектов"
template <class T, class idT> class _LINK_AS ChainList {
  // частные данные; указатели на первый, последний и
  // текущий элементы списка
  ChainOb<T, idT> *FFirst, *FLast, *FCurrent;
  // счетчик количества элементов в списке
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

  // Оператор копирования списков:
  ChainList& operator = (const ChainList &n);
};
//----------------------------------------------------------
// Вставка нового звена в список
template <class T, class idT> ChainOb<T, idT>
*ChainList<T, idT>::
store(idT &ident, T &data) {
  ChainOb<T, idT> *p;
  p = new ChainOb<T, idT>;
  // При нехватке памяти не выдаем никаких сообщений
  // поскольку можем работать при разных операционных
  // системах. Для проверки удачной вставки следует
  // проверять приращение FCount
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
//Возвращает указатель на звено с указанным идентификатором
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
// Удаляет из списка звено с указанным идентификатором
// и уничтожает его. Чтобы сохранить звено в памяти, надо
// задать параметру del нулевое значение
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
// Удаляет все звенья в списке, освобождает занятую ими
// память и устанавливает поля списка в нулевые значения
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
// Возвращает значение идентификатора для данных
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
//Возвращает указатель на данные по заданному идентификатору
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
// Перегрузка оператора индексации с возвращением ссылки на
// данные
template <class T, class idT> T &ChainList<T, idT>::
operator[](idT ident) {
  ChainOb<T, idT> *ob;
  ob = find(ident);
  // если данных по индексу ident нет, то создаем новое
  // звено с этим индексом и инициализируем его нулем
  if (!ob) {
     T data;
     ob = store(ident, data);
  }
  return ob->Data;
}//---------------------------------------------------------
template <class T, class idT> ChainList<T, idT> &ChainList<T, idT>::
operator = (const ChainList<T, idT> &n) {
  // Надо обойти ограничение на не const функции при их
  // использовании с const объектами:
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