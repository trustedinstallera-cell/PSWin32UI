#pragma once
#include <windows.h>
#include <string>
#include "control.h"

// 全局默认窗口句柄（用于Control类）
extern HWND defaultHwnd_;
extern bool fixDefaultHwnd;

class Window {
protected:
    HWND hwnd;
    HWND parent;
    HMENU hMenu;

    void initializeComponent() {
        this->hwnd = nullptr;
        this->parent = nullptr;
        this->hMenu = nullptr;
        this->text = L"";

        // WS_* 样式成员变量
        this->isChild = false;
        this->isPopup = false;
        this->isOverlapped = true;
        this->hasBorder = true;
        this->hasThickFrame = true;
        this->hasDialogFrame = false;
        this->hasCaption = true;
        this->hasSystemMenu = true;
        this->minimizeBox = true;
        this->maximizeBox = true;
        this->closeBox = true;
        this->visible = true;
        this->disabled = false;
        this->minimized = false;
        this->maximized = false;
        this->hasHScroll = false;
        this->hasVScroll = false;
        this->isTabStop = false;
        this->isGroupFirst = false;
        this->clipChildren = true;
        this->clipSiblings = false;

        this->acceptFiles = false;
        this->helpButton = false;
        this->showInTaskbar = true;
        this->clientEdge = false;
        this->dialogFrame = false;
        this->layered = false;
        this->toolWindow = false;
        this->topMost = false;
        this->transparent = false;

        this->position = Point(CW_USEDEFAULT, CW_USEDEFAULT);
        this->size = Size(800, 600);
        this->state = Normal;
    }

    // 注册窗口类（XP兼容版本）
    bool RegisterWindowClass() {
        WNDCLASSEX wc = {};
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.lpfnWndProc = Window::StaticWndProc;
        wc.hInstance = GetModuleHandle(NULL);
        wc.lpszClassName = L"WindowClass4827";  // 唯一类名避免冲突
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);  // XP必须设置光标
        wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

        // XP兼容：先尝试注销已存在的类（避免重复注册错误）
        UnregisterClass(L"WindowClass4827", wc.hInstance);

        // 注册新类
        ATOM result = RegisterClassEx(&wc);
        return (result != 0);
    }

    // 静态窗口过程
    static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        Window* pWindow = nullptr;

        if (msg == WM_NCCREATE) {
            // 获取窗口参数
            CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
            pWindow = reinterpret_cast<Window*>(pCreate->lpCreateParams);
            if (pWindow) {
                SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
                pWindow->hwnd = hwnd;
            }
        }
        else {
            pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        }

        if (pWindow) {
            return pWindow->HandleMessage(msg, wParam, lParam);
        }

        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    // 虚函数，子类可重写处理消息
    virtual LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
        switch (msg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_ERASEBKGND:
            // XP优化：返回1表示已处理擦除背景
            return 1;
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

public:
    enum InitialState {
        Normal,
        Minimized,
        Maximized
    };

    Point position;
    Size size;
    InitialState state;

    // WS_* 样式成员变量
    bool isChild;
    bool isPopup;
    bool isOverlapped;
    bool hasBorder;
    bool hasThickFrame;
    bool hasDialogFrame;
    bool hasCaption;
    bool hasSystemMenu;
    bool minimizeBox;
    bool maximizeBox;
    bool visible;
    bool disabled;
    bool minimized;
    bool maximized;
    bool hasHScroll;
    bool hasVScroll;
    bool isTabStop;
    bool isGroupFirst;
    bool clipChildren;
    bool clipSiblings;

    bool acceptFiles;
    bool helpButton;
    bool showInTaskbar;
    bool clientEdge;
    bool dialogFrame;
    bool layered;
    bool toolWindow;
    bool topMost;
    bool transparent;
    bool closeBox;
    std::wstring text;

    static Size GetDefaultSize() {
        return Size(800, 600);
    }

    void setSize(Size size_ = Size(800, 600)) {
        this->size = size_;
    }

    Window() : hwnd(nullptr), parent(nullptr), hMenu(nullptr) {
        initializeComponent();
    }

    Window(Size size_) : hwnd(nullptr), parent(nullptr), hMenu(nullptr) {
        initializeComponent();
        setSize(size_);
    }

    Window(Size size_, Point position) : hwnd(nullptr), parent(nullptr), hMenu(nullptr) {
        initializeComponent();
        this->position = position;
        if (position.x == -1 && position.y == -1) {
            this->position = Point(CW_USEDEFAULT, CW_USEDEFAULT);
        }
        setSize(size_);
    }

    virtual ~Window() {
        if (hwnd) {
            DestroyWindow(hwnd);
        }
    }

    HWND getHwnd() {
        return this->hwnd;
    }

    DWORD getDwStyle() {
        DWORD dwStyle = 0;

        if (isChild) dwStyle |= WS_CHILD;
        else if (isPopup) dwStyle |= WS_POPUP;
        else if (isOverlapped) dwStyle |= WS_OVERLAPPED;

        if (hasBorder) dwStyle |= WS_BORDER;
        if (hasThickFrame) dwStyle |= WS_THICKFRAME;
        if (hasDialogFrame) dwStyle |= WS_DLGFRAME;

        if (hasCaption) dwStyle |= WS_CAPTION;
        if (hasSystemMenu) {
            dwStyle |= WS_SYSMENU;
            if (minimizeBox && !helpButton) dwStyle |= WS_MINIMIZEBOX;
            if (maximizeBox && !helpButton) dwStyle |= WS_MAXIMIZEBOX;
        }

        if (visible) dwStyle |= WS_VISIBLE;
        if (disabled) dwStyle |= WS_DISABLED;
        if (minimized) dwStyle |= WS_MINIMIZE;
        if (maximized) dwStyle |= WS_MAXIMIZE;

        if (hasHScroll) dwStyle |= WS_HSCROLL;
        if (hasVScroll) dwStyle |= WS_VSCROLL;
        if (isTabStop) dwStyle |= WS_TABSTOP;
        if (isGroupFirst) dwStyle |= WS_GROUP;
        if (clipChildren) dwStyle |= WS_CLIPCHILDREN;
        if (clipSiblings) dwStyle |= WS_CLIPSIBLINGS;

        return dwStyle;
    }

    DWORD getExStyle() {
        DWORD dwExStyle = 0;

        if (!showInTaskbar) dwExStyle |= WS_EX_TOOLWINDOW;
        if (hasSystemMenu && helpButton) dwExStyle |= WS_EX_CONTEXTHELP;
        if (clientEdge) dwExStyle |= WS_EX_CLIENTEDGE;
        if (dialogFrame) dwExStyle |= WS_EX_DLGMODALFRAME;
        if (layered) dwExStyle |= WS_EX_LAYERED;
        if (toolWindow) dwExStyle |= WS_EX_TOOLWINDOW;
        if (topMost) dwExStyle |= WS_EX_TOPMOST;
        if (transparent) dwExStyle |= WS_EX_TRANSPARENT;
        if (acceptFiles) dwExStyle |= WS_EX_ACCEPTFILES;

        return dwExStyle;
    }

    void show(HINSTANCE hInstance = GetModuleHandle(NULL)) {
        if (this->hwnd == nullptr) {
            // 注册窗口类
            if (!RegisterWindowClass()) {
                DWORD error = GetLastError();
                char msg[256];
                sprintf_s(msg, "RegisterClassEx failed: %d", error);
                MessageBoxA(nullptr, msg, "Error", MB_OK);
                return;
            }

            // 计算初始显示状态
            int nCmdShow = SW_SHOWNORMAL;
            if (state == Minimized) nCmdShow = SW_MINIMIZE;
            else if (state == Maximized) nCmdShow = SW_SHOWMAXIMIZED;

            // 创建窗口
            hwnd = CreateWindowEx(
                getExStyle(),
                L"WindowClass4827",
                text.c_str(),
                getDwStyle(),
                position.x, position.y,
                size.x, size.y,
                parent,
                hMenu,
                hInstance,
                this  // 传递this指针
            );

            if (hwnd == nullptr) {
                DWORD error = GetLastError();
                char msg[256];
                sprintf_s(msg, "CreateWindowEx failed: %d\nClass may not be registered properly", error);
                MessageBoxA(nullptr, msg, "Error", MB_OK);
                return;
            }

            // 设置默认窗口句柄（用于Control类）
            if (defaultHwnd_ == nullptr && !fixDefaultHwnd) {
                defaultHwnd_ = getHwnd();
            }

            // 显示窗口
            if (visible) {
                ShowWindow(hwnd, nCmdShow);
                UpdateWindow(hwnd);
            }
        }
        else {
            // 更新已有窗口
            SendMessage(hwnd, WM_SETREDRAW, FALSE, 0);

            // 更新窗口样式
            SetWindowLong(hwnd, GWL_STYLE, getDwStyle());
            SetWindowLong(hwnd, GWL_EXSTYLE, getExStyle());

            // 更新位置和大小
            SetWindowPos(hwnd, nullptr, position.x, position.y,
                size.x, size.y, SWP_NOZORDER | SWP_NOACTIVATE);

            // 更新标题
            SetWindowText(hwnd, text.c_str());

            // 重新启用重绘
            SendMessage(hwnd, WM_SETREDRAW, TRUE, 0);
            InvalidateRect(hwnd, nullptr, TRUE);
            UpdateWindow(hwnd);
        }

        
        HMENU hSysCloseMenu = GetSystemMenu(hwnd, this->closeBox);
        if (hSysCloseMenu && !this->closeBox) {
            RemoveMenu(hSysCloseMenu, SC_CLOSE, MF_BYCOMMAND);
        }

    }

    void close() {
        if (hwnd) {
            DestroyWindow(hwnd);
            hwnd = nullptr;
        }
    }

    void update() {
        if (hwnd) {
            UpdateWindow(hwnd);
        }
    }
};