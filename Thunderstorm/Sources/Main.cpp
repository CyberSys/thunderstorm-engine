#include <spdlog/spdlog.h>

#include "fs.h"
#include "s_debug.h"
#include "externs.h"
#include <spdlog/sinks/basic_file_sink.h>

S_DEBUG CDebug;
bool isHold = false;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
  /* Prevent multiple instances */
  if (!CreateEventA(nullptr, false, false, "Global\\FBBD2286-A9F1-4303-B60C-743C3D7AA7BE")
    || GetLastError() == ERROR_ALREADY_EXISTS) {
    MessageBoxA(nullptr, "Another instance is already running!", "Error", MB_ICONERROR);
    return 0;
  }

  /* Init stash */
  create_directories(fs::GetLogsPath());
  create_directories(fs::GetSaveDataPath());

  /* Init system log */
  const auto log_path = fs::GetLogsPath() / "system.log";
  const auto system_log = spdlog::basic_logger_mt("system", log_path.string(), true);
  set_default_logger(system_log);
  system_log->set_level(spdlog::level::trace);

  fio = &File_Service;
  //_VSYSTEM_API = &System_Api;

  /* Read config */
  uint32_t dwMaxFPS = 0;
  auto* ini = File_Service.OpenIniFile(ENGINE_INI_FILE_NAME);
  if (ini) {
    // !~!
    dwMaxFPS = static_cast<uint32_t>(ini->GetLong(nullptr, "max_fps", 0));
    auto bDebugWindow = ini->GetLong(nullptr, "DebugWindow", 0) == 1;
    auto bAcceleration = ini->GetLong(nullptr, "Acceleration", 0) == 1;

    delete ini;
  }

  /* Register and show window */
  const auto* const windowName = "Thunderstorm";

  WNDCLASSEX wndclass;
  wndclass.cbSize = sizeof(wndclass);
  wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
  wndclass.lpfnWndProc = WndProc;
  wndclass.cbClsExtra = 0;
  wndclass.cbWndExtra = sizeof(uint16_t);
  wndclass.hInstance = hInstance;
  wndclass.hIcon = LoadIcon(hInstance, "IDI_ICON1");
  wndclass.hCursor = LoadCursor(hInstance, "NULL_CURSOR"); //LoadCursor(NULL,IDC_ARROW);
  wndclass.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
  wndclass.lpszMenuName = nullptr;
  wndclass.lpszClassName = windowName;
  wndclass.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
  RegisterClassExA(&wndclass);

  auto* const hwnd = CreateWindowA(windowName, windowName, WS_POPUP, 0, 0, 0, 0, NULL, NULL, hInstance, NULL);
  ShowWindow(hwnd, SW_SHOWNORMAL);

  /* Init stuff */
  core.InitBase();

  /* Message loop */
  auto dwOldTime = GetTickCount();
  MSG msg;
  while (true) {
    if (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
      if (msg.message == WM_QUIT)
        break;
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else {
      if (bActive) {
        if (dwMaxFPS) {
          const auto dwMS = 1000u / dwMaxFPS;
          const auto dwNewTime = GetTickCount();
          if (dwNewTime - dwOldTime < dwMS)
            continue;
          dwOldTime = dwNewTime;
        }
        if (!isHold && !core.Run()) {
          core.CleanUp();
          isHold = true;
          SendMessage(hwnd, WM_CLOSE, 0, 0L);
        }
      }
      else {
        Sleep(50);
      }
    }
  }

  /* Release */
  core.ReleaseBase();
  ClipCursor(nullptr);

  return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
  switch (iMsg) {
  case WM_CREATE:
    core.Set_Hwnd(hwnd);
    break;

  case WM_CLOSE:
    DestroyWindow(hwnd);
    break;

  case WM_DESTROY:
    core.Event("DestroyWindow", nullptr);
    core.Event("ExitApplication", nullptr);
    CDebug.Release();
    core.CleanUp();
    File_Service.Close();
    CDebug.CloseDebugWindow();

    InvalidateRect(nullptr, nullptr, 0);
    PostQuitMessage(0);
    break;

  case WM_ACTIVATE:
    bActive = LOWORD(wParam) == WA_CLICKACTIVE || LOWORD(wParam) == WA_ACTIVE;
    core.AppState(bActive);
    break;

  case WM_KEYDOWN:
    if (wParam == VK_F5) // && bDebugWindow
    {
      if (!CDebug.IsDebug())
        CDebug.OpenDebugWindow(core.hInstance);
      else {
        ShowWindow(CDebug.GetWindowHandle(), SW_NORMAL);
        SetFocus(CDebug.SourceView->hOwn);
      }
    }
  case WM_KEYUP:
  case WM_RBUTTONUP:
  case WM_RBUTTONDOWN:
  case WM_LBUTTONUP:
  case WM_LBUTTONDOWN:
  case WM_MBUTTONDOWN:
  case WM_MBUTTONUP:
  //case MM_MCINOTIFY:
  case WM_LBUTTONDBLCLK:
  case WM_CHAR:
  case WM_MOUSEMOVE:
    if (core.Controls)
      core.Controls->EngineMessage(iMsg, wParam, lParam);
    break;

  case WM_MOUSEWHEEL:
    core.Event("evMouseWeel", "l", static_cast<short>(HIWORD(wParam)));
    if (core.Controls)
      core.Controls->EngineMessage(iMsg, wParam, lParam);
    break;

  default: ;
  }

  return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
