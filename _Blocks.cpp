void TDxfMeshModel::ReadBlock() {
  while (!stream.fail() && !Aborted) {
     GetPair();
     gotpair:
     if (pair.key == 2) CurBlock = &BlockTable[pair.val];
     else if (pair.key == 10) CurBlock->x = atof(pair.val);
     else if (pair.key == 20) CurBlock->y = atof(pair.val);
     else if (pair.key == 30) CurBlock->z = atof(pair.val);
     else if (pair == _3DFACE) {
        ReadFace();
        goto gotpair;
     }
     else if (pair == POLYLINE) {
        ReadPoly();
        goto gotpair;
     }
     else if (pair == INSERT) {
        ReadInsert();
        goto gotpair;
     }
     else if (pair == ENDSEC) break;
     else if (pair == ENDBLK) break;
     else if (pair.key == 0)  {SkipEntity(); goto gotpair;}
  }
}//---------------------------------------------------------
void TDxfMeshModel::ReadBlocks() {
  while (!stream.fail() && !Aborted) {
     GetPair();
     if (pair == _EOF)        break;
     else if (pair == ENDSEC) break;
     else if (!DoSkipBlocks && (pair == BLOCK)) ReadBlock();
  }
}//---------------------------------------------------------
