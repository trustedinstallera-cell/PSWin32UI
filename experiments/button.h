#pragma once
#include "control.h"
#include <functional>
#include <windowsx.h>
#include <commctrl.h>
#include <string>
#include <tchar.h>
#include <uxtheme.h>
#pragma comment(lib, "uxtheme.lib")

#pragma comment(lib, "comctl32.lib")

namespace MouseButtons {
    enum Enum {
        None = 0,
        Left = 1,
        Right = 2,
        Middle = 4,
        XButton1 = 8,
        XButton2 = 16
    };
}

namespace ModifierKeys {
    enum Enum {
        None = 0,
        Shift = 1,
        Control = 2,
        Alt = 4
    };
}

class Button : public Control {
public:
    std::function<void(Button&)> onClick;
    std::function<void(Button&)> onMouseUp;
    std::function<void(Button&)> onMouseMove;

    std::wstring text;
    DWORD dwStyle;
    HWND parent;
    Point startPosition;
    Size size;
    bool enabled;   
    bool legacyStyle;
    //Font font;


    HWND getHwnd() {
        return this->hwnd;
    }

    void focus() {
        if (getHwnd() != nullptr) {
            SetFocus(hwnd);
        }
    }

    void show() {
        if (this->hwnd == nullptr) {

            INITCOMMONCONTROLSEX icex = {0};
            icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
            icex.dwICC = ICC_WIN95_CLASSES;  // XP兼容标志
            InitCommonControlsEx(&icex);

            // 直接使用系统 BUTTON 类
            this->hwnd = CreateWindowEx(0, L"BUTTON", this->text.c_str(),
                this->dwStyle,
                this->startPosition.x, this->startPosition.y,
                this->size.x, this->size.y,
                parent, NULL, GetModuleHandle(NULL), NULL);

            if (this->hwnd) {
                if (legacyStyle) {
                    SetWindowTheme(this->hwnd, L"", L"");
                }

                SetWindowLongPtr(this->hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

                // 子类化只是为了拦截消息，但不干扰系统绘制
                originalWndProc = reinterpret_cast<WNDPROC>(
                    SetWindowLongPtr(this->hwnd, GWLP_WNDPROC,
                        reinterpret_cast<LONG_PTR>(Button::SubclassProc))
                    );
            }
        }
        else {
            SetWindowText(hwnd, text.c_str());
            MoveWindow(hwnd, startPosition.x, startPosition.y, size.x, size.y, TRUE);
        }
    }

    void update() {
        show();
    }

    static LRESULT CALLBACK SubclassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        Button* pButton = reinterpret_cast<Button*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

        if (pButton) {
            switch (msg) {
            case WM_LBUTTONUP:
                if (pButton->onClick) {
                    pButton->onClick(*pButton);
                }
                // 重要：不return，让系统继续处理默认行为
                break;
            case WM_MOUSEMOVE:
                if (pButton->onMouseMove) {
                    pButton->onMouseMove(*pButton);
                }
                break;
            case WM_DESTROY:
                // 恢复原始窗口过程
                if (pButton->originalWndProc) {
                    SetWindowLongPtr(hwnd, GWLP_WNDPROC,
                        reinterpret_cast<LONG_PTR>(pButton->originalWndProc));
                }
                break;
            }
        }

        // 调用原始窗口过程，让系统完成所有绘制工作
        if (pButton && pButton->originalWndProc) {
            return CallWindowProc(pButton->originalWndProc, hwnd, msg, wParam, lParam);
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    Button(HWND parent = nullptr) : hwnd(nullptr), originalWndProc(nullptr), enabled(true) {
        this->dwStyle = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON;
        this->parent = parent;
        this->size = Size(75, 23);
        this->startPosition = Point(0, 0);
        this->legacyStyle = false;
        if (this->parent == nullptr && defaultHwnd_ != nullptr) {
            this->parent = defaultHwnd_;
        }
    }

protected:
    WNDPROC originalWndProc;
    HWND hwnd;
};

// MouseEventArgs 定义
struct MouseEventArgs {
    MouseButtons::Enum button;
    Point location;
    int clicks;
    ModifierKeys::Enum modifiers;
    bool handled;

    MouseEventArgs()
        : button(MouseButtons::None)
        , clicks(0)
        , modifiers(ModifierKeys::None)
        , handled(false) {}
};