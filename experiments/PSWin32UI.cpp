#define _WIN32_IE    0x0600 

#include "control.h"
#include "button.h"
#include "window.h"
#include "position.h"
#include "font.h"
#include "color.h"
#include "dialog.h"


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR lpCmd, int nShow) {
    param.hInstance = hInst;
    param.lpCmdLine = lpCmd;
    param.nCmdShow = nShow;

    Window window;
    window.minimizeBox = false;
    window.maximizeBox = false;
    window.text = L"My Window";
    window.minSize = Size(200, 350);
    window.minimized = true;
    window.show();  // 创建并显示窗口

    Button button(window.getHwnd());
    button.text = L"&Test";

    long long counter = 0;
    button.onClick = [&](Button& btn) {
        btn.text = std::to_wstring(counter);
        counter++;
        btn.update();
    };
    //button.legacyStyle = true;
    button.parent = window.getHwnd();
    button.show();

    Button buttonB(window.getHwnd());
    buttonB.text = L"Test123";
    buttonB.onClick = [&](Button& btn) {
        btn.text = std::to_wstring(counter);
        counter++;
        btn.update();
    };
    buttonB.startPosition = Point(100, 20);
    buttonB.update();

    // 添加消息循环
    return window.run();


}