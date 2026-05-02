#define WIN32_LEAN_AND_MEAN
#define _WIN32_IE    0x0600 

#include "control.h"
#include "button.h"
#include "window.h"
#include "position.h"
#include "font.h"
#include "color.h"


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR lpCmd, int nShow) {
    param.hInstance = hInst;
    param.lpCmdLine = lpCmd;
    param.nCmdShow = nShow;

    Window window;
    window.minimizeBox = false;
    window.maximizeBox = false;
    window.text = L"My Window";
    window.minimized = true;
    window.show();  // 创建并显示窗口


    // 添加消息循环
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}