#define MAKER_ADD_MESH_FACE \
  Face.AddVertex( CurMaker->AddVertex(vlist[v1-1]) );  \
  Face.AddVertex( CurMaker->AddVertex(vlist[v2-1]) );  \
  Face.AddVertex( CurMaker->AddVertex(vlist[v3-1]) );  \
  Face.AddVertex( CurMaker->AddVertex(vlist[v4-1]) );  \
  Face.Color = color;                                  \
  CurMaker->AddFace(Face);
//----------------------------------------------------------
void TDxfMeshModel::ReadPoly() {
  bool MClosed = false, NClosed = false;
  int M = 0, N = 0, vertex = -1, color = 7;
  TextVector *vlist = NULL;
  while (!stream.fail() && !Aborted) {
     GetPair();
     if (pair == SEQEND) break;
     else
     if (pair == VERTEX) vertex++;
     else if (pair.key==8)
                CurMaker = &CurBlock->LayTable[pair.val];
     else
     if (pair.key == 70) {
        // Игнорируем этот маркер для вершин
        if (vertex > -1) continue;
        // Но проверяем его для полилинии
        int flags = atoi(pair.val);
        if (!(flags & 16)) return; // Это не сеть
        if (flags & 1)  MClosed = true;
        if (flags & 32) NClosed = true;
     }
     else if (pair.key == 71) M = atoi(pair.val);
     else if (pair.key == 72) {
        N = atoi(pair.val);
        vlist = new TextVector[M*N];
        memset(vlist, 0, sizeof vlist);
     }
     else if (pair.key == 10) {
        if (M && N && (vertex > -1))
           vlist[vertex].x = pair.val;
     }
     else if (pair.key == 20) {
        if (M && N && (vertex > -1))
           vlist[vertex].y = pair.val;
     }
     else if (pair.key == 30) {
        if (M && N && (vertex > -1))
           vlist[vertex].z = pair.val;
     }
     else if (pair.key == 62)
        color = atoi(pair.val);
  }

  // Если сеть открыта, то достаточно следующего
  // вложенного цикла:
  for (int m = 1; m < M; m++)
     for (int n = 1; n < N; n++) {
        TFace Face;
        int v1, v2, v3, v4;
        v1 = N*(m-1)+n;
        v2 = N*(m)+n;
        v3 = N*(m)+n+1;
        v4 = N*(m-1)+n+1;
        MAKER_ADD_MESH_FACE
     }
  if (MClosed) // например закрыть трубу тора:
     for (int n = 1; n < N; n++) {
        TFace Face;
        int v1, v2, v3, v4;
        v1 = N*(M-1)+n;
        v2 = n;
        v3 = n+1;
        v4 = N*(M-1)+n+1;
        MAKER_ADD_MESH_FACE
     }
  if (NClosed)
     for (int m = 1; m < M; m++) {
        TFace Face;
        int v1, v2, v3, v4;
        v1 = N*(m-1)+N;
        v2 = N*(m)+N;
        v3 = N*(m)+1;
        v4 = N*(m-1)+1;
        MAKER_ADD_MESH_FACE
     }
  if (MClosed && NClosed) {
     TFace Face;
     int v1, v2, v3, v4;
     v1 = N*(M-1)+N;
     v4 = N*(M-1)+1;
     v3 = 1;
     v2 = N;
     MAKER_ADD_MESH_FACE
  }
  delete[] vlist;
  DxfMeshCount++;
}//---------------------------------------------------------
