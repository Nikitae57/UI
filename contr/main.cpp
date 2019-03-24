#include <windows.h>
#include <string>
#include <shlobj.h>
#include <iostream>
#include <thread>         // std::this_thread::sleep_for

char path[MAX_PATH];

void showErrMsg() {
  DWORD error = GetLastError();
  LPVOID msgTextRus;
  LPVOID msgTextEng;
  FormatMessage(
      FORMAT_MESSAGE_ALLOCATE_BUFFER |
          FORMAT_MESSAGE_FROM_SYSTEM |
          FORMAT_MESSAGE_IGNORE_INSERTS,
      NULL,
      error,
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
      (LPTSTR) &msgTextRus,
      0, NULL);

  FormatMessage(
      FORMAT_MESSAGE_ALLOCATE_BUFFER |
          FORMAT_MESSAGE_FROM_SYSTEM |
          FORMAT_MESSAGE_IGNORE_INSERTS,
      NULL,
      error,
      MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
      (LPTSTR) &msgTextEng,
      0, NULL);

  char finalMsg[500];
  sprintf_s(finalMsg, "%d\n%s%s", error, msgTextRus, msgTextEng);
  std::cout << finalMsg << '\n';
}

DWORD WINAPI runThread(LPVOID lParam) {
  char systemCommand[1000];
  sprintf_s(
      systemCommand,
      "7z a \"%s.zip\" -r \"%s\\*\"",
      path,
      path
  );

  system(systemCommand);
  return 0;
}

void archiveDir(char* path) {
  char systemCommand[1000];
  sprintf_s(
      systemCommand,
      "7z a \"%s.zip\" -r \"%s\\*\"",
      path,
      path
  );

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

  SHGetPathFromIDList(pidl, path);
  std::thread t(archiveDir, path);
  showErrMsg();

  return 0;
}