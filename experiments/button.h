#pragma once
#include "control.h"
#include <functional>
#include <commctrl.h>
#include <string>
#include <uxtheme.h>

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "uxtheme.lib")

class Button : public Control {
public:
    std::function<void(Button&)> onClick;

    std::wstring text;
    DWORD dwStyle;
    HWND parent;

    bool legacyStyle;

    Button(HWND parent = nullptr) {
        this->parent = parent ? parent : defaultHwnd_;
        this->size = Size(75, 23);
        this->startPosition = Point(0, 0);
        this->dwStyle = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP;
        this->legacyStyle = false;
    }

    void show() {
        if (!hwnd) {
            INITCOMMONCONTROLSEX icex = { sizeof(icex), ICC_WIN95_CLASSES };
            InitCommonControlsEx(&icex);

            hwnd = CreateWindowEx(
                0,
                L"BUTTON",
                text.c_str(),
                dwStyle,
                startPosition.x, startPosition.y,
                size.x, size.y,
                parent,
                (HMENU)(INT_PTR)id,   // * 关键：用 id
                GetModuleHandle(NULL),
                NULL
            );

            if (hwnd) {
                // * 关键：绑定 this
                SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);

                if (legacyStyle)
                    SetWindowTheme(hwnd, L"", L"");
            }
        }
        else {
            SetWindowText(hwnd, text.c_str());
            MoveWindow(hwnd,
                startPosition.x, startPosition.y,
                size.x, size.y, TRUE);
        }
    }

    void update() {
        show();
    }
};