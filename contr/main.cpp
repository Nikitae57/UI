#include <windows.h>
#include <string>
#include <shlobj.h>
#include <iostream>
#include <sstream>
#include <stdio.h>

void archiveDir(char* path) {
  char systemCommand[1000];
  sprintf_s(systemCommand, "7z a \"%s.zip\" -r \"%s\"", path, path);
  std::cout << "cmd: " << systemCommand << '\n';
//  system("7z a \"D:\\archive.zip\" -r \"D:\\projects\\arch\\labs\"");
  system(systemCommand);
}

int WINAPI WinMain(
    HINSTANCE hInst,
    HINSTANCE hPrevInstance,
    LPSTR lpszCmdLine,
    int nCmdShow
) {
  char currentDir[500];
  GetCurrentDirectory(500, currentDir);

  BROWSEINFO bi = {0};
  bi.lpszTitle = "Select directory to archive";
  bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
  bi.lpfn = NULL;
  bi.lParam = (LPARAM) currentDir;

  LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
  if (pidl == 0) { return 0; }

  char selectedPath[MAX_PATH];
  SHGetPathFromIDList(pidl, selectedPath);

//  std::cout << selectedPath;
  archiveDir(selectedPath);
}