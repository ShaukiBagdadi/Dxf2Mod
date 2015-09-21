void TDxfMeshModel::ReadAttrib(TInsert &ins) {
  string att;
  while (!stream.fail() && !Aborted) {
     GetPair();
     if (pair.key == 1) att = pair.val;
     else if (pair.key == 2) ins.AttribList[pair.val] = att;
     else if (pair == SEQEND) break;
  }
}//---------------------------------------------------------
void TDxfMeshModel::ReadInsert() {
  TInsert ins;
  while (!stream.fail() && !Aborted) {
     GetPair();
          if (pair.key == 2)  ins.bname  = pair.val;
     else if (pair.key == 8)  ins.lname  = pair.val;
     else if (pair.key == 10) ins.x = pair.val;
     else if (pair.key == 20) ins.y = pair.val;
     else if (pair.key == 30) ins.z = pair.val;
     else if (pair.key == 41) ins.scalex = atof(pair.val);
     else if (pair.key == 42) ins.scaley = atof(pair.val);
     else if (pair.key == 43) ins.scalez = atof(pair.val);
     else if (pair.key == 50) ins.angle  = atof(pair.val);
     else if (pair == ATTRIB) {ReadAttrib(ins); break;}
     else if (pair.key == 0 ) break;
  }
  CurBlock->InsList.add(ins);
}//---------------------------------------------------------