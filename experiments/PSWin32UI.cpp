#define WIN32_LEAN_AND_MEAN
//#define _WIN32_IE    0x0600 
//#define DISABLE_VISUAL_STYLES

#include "control.h"
#include "button.h"
#include "window.h"
#include "position.h"
#include "font.h"
#include "color.h"
#include "dialog.h"

void OnClick() {

}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR lpCmd, int nShow) {
    param.hInstance = hInst;
    param.lpCmdLine = lpCmd;
    param.nCmdShow = nShow;

    Window window;
    window.maximizeBox = false;
    window.closeBox = false;
    window.text = L"My Window";   

    Button button(window.getHwnd());
    button.text = L"&Test";
   
    long long counter = 0;
    button.onClick = [&](Button& btn) {
        RetryContinueDialog dlg(L"Test message");
        
        dlg.show();
        window.close();
        
    };
    //button.legacyStyle = true;
    window.show();
    button.parent = window.getHwnd();
    button.show();

    // 添加消息循环
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}