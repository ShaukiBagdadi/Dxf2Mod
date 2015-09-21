//----------------------------------------------------------
// T F a c e
//----------------------------------------------------------
TFace& TFace::operator = (const TFace& n) {
  if (this == &n) return *this;
  *((TIndexList*)this) = TIndexList::operator=(n);
  dwUnique = n.dwUnique;
  Color    = n.Color;
  return *this;
}//---------------------------------------------------------
int  operator == (const TFace& a,const TFace& b) {
  if (a.Count()  != b.Count())  return 0;
  if (a.Unique() != b.Unique()) return 0;
  return 1;
}//---------------------------------------------------------
bool TFace::Valid() {
  if (count() < 3)  return false;
  return true;
}//---------------------------------------------------------
DWORD TFace::AddVertex(DWORD vindex) {
  TIndexIter iter(*this);
  DWORD *ptr, idx = 0;
  for (ptr = iter.home(); ptr; ptr = iter.next()) {
     idx++;
     if (*ptr == vindex) return idx;
  }
  add(vindex);
  // TODO: equality of faces is not correct for very big models
  dwUnique += vindex*vindex*vindex;
  return Count();
}//---------------------------------------------------------
AnsiString TFace::IndexesList() {
  AnsiString result;
  TIndexIter IdxIter(*this);
  for (DWORD *idx = IdxIter.home(); idx; idx = IdxIter.next()) {
     result += AnsiString((int)*idx) + AnsiString(",");
  }
  return result;
}//---------------------------------------------------------

