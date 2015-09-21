void TDxfMeshModel::ReadEntities() {
  CurBlock = &BlockTable["Entities"];
  while (!stream.fail() && !Aborted) {
     GetPair();
     gotpair:
     if (pair == _EOF)        break;
     else if (pair == ENDSEC) break;
     else if (pair == INSERT)   {
        ReadInsert();
        goto gotpair;
     }
     else if (pair == _3DFACE)  {
        ReadFace();
        goto gotpair;
     }
     else if (pair == POLYLINE) {
        ReadPoly();
        goto gotpair;
     }
  }
}//---------------------------------------------------------
/*
// TODO: get rid of goto??
void TDxfMeshModel::ReadEntities() {
  CurBlock = &BlockTable["Entities"];
  while (!stream.fail() && !Aborted) {
        GetPair();
        if (pair == _EOF)        break;
        else if (pair == ENDSEC) break;
        else if (pair == INSERT)   {
            ReadInsert();
            continue;
         }
         else if (pair == _3DFACE)  {
             ReadFace();
             continue;
         }
         else if (pair == POLYLINE) {
             ReadPoly();
             continue;
     }
  }
}//---------------------------------------------------------
*/