#include <windows.h>
#include <string>
#include <shlobj.h>
#include <iostream>

void showErrMsg(DWORD error) {
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

DWORD WINAPI ThreadProc(CONST LPVOID lpParam) {
  char *path = (char*) lpParam;

  char systemCommand[1000];
  sprintf_s(
      systemCommand,
      "7z a \"%s.zip\" -r \"%s\\*\"",
      path,
      path
  );

  system(systemCommand);
  ExitThread(0);
}

int WINAPI WinMain(
    HINSTANCE hInst,
    HINSTANCE hPrevInstance,
    LPSTR lpszCmdLine,
    int nCmdShow
) {
  char currentDir[500];
  GetCurrentDirectory(500, currentDir);
  char path[MAX_PATH];

  HWND hWnd = GetConsoleWindow();
  ShowWindow(hWnd, SW_HIDE);

  BROWSEINFO bi = {0};
  bi.lpszTitle = "Select directory to archive";
  bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
  bi.lpfn = NULL;
  bi.lParam = (LPARAM) currentDir;

  LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
  if (pidl == 0) { return 0; }

  SHGetPathFromIDList(pidl, path);
  HANDLE threadHandle = CreateThread(NULL, 0, &ThreadProc, (LPVOID) path, 0, NULL);
  WaitForSingleObject(threadHandle, INFINITE);

  DWORD error = GetLastError();
  if (error != 0) {
    showErrMsg(error);
  }

  return 0;
}