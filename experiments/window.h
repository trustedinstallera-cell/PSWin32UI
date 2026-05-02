#include "control.h"
#include "entry.h"

class Window {
protected:
	HWND hwnd;

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

		this->hwnd = nullptr;
		this->acceptFiles = false;
		this->helpButton = false;
		this->showInTaskbar = true;
		this->clientEdge = false;
		this->dialogFrame = false;
		this->layered = false;
		this->toolWindow = false;
		this->topMost = false;
		this->transparent = false;
	}

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		switch (msg) {
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			// 绘制内容
			EndPaint(hwnd, &ps);
		}
		return 0;
		}
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	virtual LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
		// 子类重写处理消息
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	static bool RegisterWindowClass() {
		static bool registered = false;
		if (registered) return true;

		WNDCLASSEX wc = {};
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.lpfnWndProc = WndProc;
		wc.hInstance = GetModuleHandle(NULL);
		wc.lpszClassName = L"Window";
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

		// 先检查是否已注册
		if (GetClassInfoEx(wc.hInstance, L"Window", &wc)) {
			registered = true;
			return true;
		}

		registered = (RegisterClassEx(&wc) != 0);
		return registered;
	}

public:
	enum InitialState {
		Normal,
		Minimized,
		Maximized
	};

	static Size GetDefaultSize() {
		static Size defaultSize(800, 600);
		return defaultSize;
	}

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

	bool acceptFiles; // Accepts drag-drop files
	bool helpButton;
	bool showInTaskbar;
	bool clientEdge;
	bool dialogFrame;
	bool layered;
	bool toolWindow;
	bool topMost;
	bool transparent;
	std::wstring text;

	HWND parent;
	HMENU hMenu;

	void setSize(Size size_ = Size(800, 600)) {
		this->size = size_; // Temp method. Check value later.
	}

	Window() : hwnd(nullptr) {
		initializeComponent();
		setSize();
	}
	Window(Size size_) :hwnd(nullptr) {
		initializeComponent();
		setSize(size_);
	}
	Window(Size size_, Point position) :hwnd(nullptr) {
		initializeComponent();
		this->position = position;
		if (position.x == -1 && position.y == -1) {
			// Not setted
			this->position.position = CenterScreen;
		}
		setSize(size_);
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
		if (hasSystemMenu) dwStyle |= WS_SYSMENU;
		if (minimizeBox) dwStyle |= WS_MINIMIZEBOX;
		if (maximizeBox) dwStyle |= WS_MAXIMIZEBOX;

		if (visible) dwStyle |= WS_VISIBLE;
		if (disabled) dwStyle |= WS_DISABLED;
		if (minimized) dwStyle |= WS_MINIMIZE;
		if (maximized) dwStyle |= WS_MAXIMIZE;

		// 滚动条
		if (hasHScroll) dwStyle |= WS_HSCROLL;
		if (hasVScroll) dwStyle |= WS_VSCROLL;

		// 控件
		if (isTabStop) dwStyle |= WS_TABSTOP;
		if (isGroupFirst) dwStyle |= WS_GROUP;

		// 性能
		if (clipChildren) dwStyle |= WS_CLIPCHILDREN;
		if (clipSiblings) dwStyle |= WS_CLIPSIBLINGS;

		return dwStyle;
	}

	DWORD getExStyle() {
		DWORD dwExStyle = 0;
		dwExStyle |= (!showInTaskbar) * WS_EX_TOOLWINDOW;
		dwExStyle |= helpButton * WS_EX_CONTEXTHELP;
		dwExStyle |= clientEdge * WS_EX_CLIENTEDGE;
		dwExStyle |= dialogFrame * WS_EX_DLGMODALFRAME;
		dwExStyle |= layered * WS_EX_LAYERED;
		dwExStyle |= toolWindow * WS_EX_TOOLWINDOW;
		dwExStyle |= topMost * WS_EX_TOPMOST;
		dwExStyle |= transparent * WS_EX_TRANSPARENT;
		return dwExStyle;
	}

	void show(HINSTANCE hInstance = GetModuleHandle(NULL)) {
		if (this->hwnd == nullptr) {
			RegisterWindowClass();
			hwnd = CreateWindowEx(getExStyle(), _T("Window"), text.c_str(), getDwStyle(), position.x, position.y, size.x, size.y, parent, hMenu, hInstance, NULL);
			if (hwnd == nullptr) {
				DWORD error = GetLastError();
				char msg[256];
				sprintf_s(msg, "CreateWindowEx failed: %d", error);
				MessageBoxA(nullptr, msg, "Error", MB_OK);
				return;
			}
		}
		else {
		    // 禁止重绘
			SendMessage(hwnd, WM_SETREDRAW, FALSE, 0);

			// 根据需要修改窗口属性（位置、大小、样式、文本等）
			SetWindowPos(hwnd, nullptr, position.x, position.y,
				size.x, size.y, SWP_NOZORDER | SWP_NOACTIVATE);
			SetWindowLongPtr(hwnd, GWL_STYLE, getDwStyle());
			SetWindowLongPtr(hwnd, GWL_EXSTYLE, getExStyle());
			SetWindowText(hwnd, text.c_str());

			// 重新启用重绘
			SendMessage(hwnd, WM_SETREDRAW, TRUE, 0);

			// 触发重绘整个窗口（或指定区域）
			InvalidateRect(hwnd, nullptr, TRUE);
			UpdateWindow(hwnd);  // 立即重绘
		}
	}

};