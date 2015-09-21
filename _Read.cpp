void TDxfMeshModel::GetPair() {
  char line[255];
  memset(&pair, 0, sizeof pair);
  if (!stream.fail() && !Aborted) {
     stream.getline(line, sizeof line);
     pair.key = atoi(line);
  }
  if (!stream.fail() && !Aborted)
     stream.getline(pair.val, sizeof pair.val);

  // Some progs save differently the insert coords and other coords
  // AnsiString trimmed(pair.val);
  // strcpy(pair.val, trimmed.Trim().c_str());

  if (OnProgress) OnProgress(stream.tellg(), streamsize);
}//---------------------------------------------------------
void TDxfMeshModel::SkipEntity() {
  while (!stream.fail() && !Aborted) {
     GetPair();
     if (pair.key == 0) break;
  }
}//---------------------------------------------------------
void TDxfMeshModel::ReadFromFile(char *fname) {
  // Очистка содержимого от предыдущего выполнения:
  Clear();
  // Извлечение размера DXF файла для использования в
  // процессе отображения степени завершенности:
  stream.open(fname, ios::in);
  stream.seekg(0, ios::end);
  streamsize = stream.tellg();
  stream.seekg(0, ios::beg);
  // Главный цикл разбора DXF файла:
  while (!stream.fail() && !Aborted) {
     GetPair();
     if (pair == _EOF)          break;
     else if (pair == BLOCKS)   ReadBlocks();
     else if (pair == ENTITIES) ReadEntities();
  }
  stream.close();
}//---------------------------------------------------------
