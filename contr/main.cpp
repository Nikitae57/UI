#include <windows.h>
#include <string>
#include <shlobj.h>

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
  MessageBox(NULL, finalMsg, NULL, NULL);
}

void archive(char *path) {
  char systemCommand[1000];
  sprintf_s(
      systemCommand,
      "7z a \"%s.zip\" -r \"%s\\*\"",
      path,
      path
  );

  PROCESS_INFORMATION pi;
  ZeroMemory(&pi, sizeof(pi));
  STARTUPINFO si;
  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);

  BOOL bResult = CreateProcess(NULL, systemCommand,
      NULL, NULL, FALSE, CREATE_NO_WINDOW,
      NULL, NULL, &si, &pi);

  if(bResult) {
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
  }
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
  archive(path);

  DWORD error = GetLastError();
  if (error != 0) {
    showErrMsg(error);
  }

  return 0;
}