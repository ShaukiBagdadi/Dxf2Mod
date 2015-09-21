void TDxfMeshModel::ReadFace() {
  int color = 7;
  TextVector vlist[4];
  memset(&vlist, 0, sizeof vlist);
  while (!stream.fail() && !Aborted) {
     GetPair();
     if (pair.key == 0) break;
     else if (pair.key==8)
               CurMaker = &CurBlock->LayTable[pair.val];
     else if (pair.key==10) vlist[0].x = pair.val;
     else if (pair.key==20) vlist[0].y = pair.val;
     else if (pair.key==30) vlist[0].z = pair.val;
     else if (pair.key==11) vlist[1].x = pair.val;
     else if (pair.key==21) vlist[1].y = pair.val;
     else if (pair.key==31) vlist[1].z = pair.val;
     else if (pair.key==12) vlist[2].x = pair.val;
     else if (pair.key==22) vlist[2].y = pair.val;
     else if (pair.key==32) vlist[2].z = pair.val;
     else if (pair.key==13) vlist[3].x = pair.val;
     else if (pair.key==23) vlist[3].y = pair.val;
     else if (pair.key==33) vlist[3].z = pair.val;
     else if (pair.key==62)      color = atoi(pair.val);
  }
  TFace Face;
  Face.AddVertex( CurMaker->AddVertex(vlist[0]) );
  Face.AddVertex( CurMaker->AddVertex(vlist[1]) );
  Face.AddVertex( CurMaker->AddVertex(vlist[2]) );
  Face.AddVertex( CurMaker->AddVertex(vlist[3]) );
  Face.Color = color;
  CurMaker->AddFace(Face);
  DxfFaceCount++;
}//---------------------------------------------------------
