#include "control.h"
#include <functional>
#include <windowsx.h>
#include <string>
#include <tchar.h>

enum MouseButtons {
    None   = 0,
    Left   = 1,
    Right  = 2,
    Middle = 4,
    XButton1 = 8,
    XButton2 = 16
};

enum ModifierKeys {
    None    = 0,
    Shift   = 1,
    Control = 2,
    Alt     = 4
};

class Button : public Control {
public:
    std::function<void(Button&)> onClick;
	std::function<void(Button&)> onMouseUp;
	std::function<void(Button&)> onMouseMove;
	
	void show(){
		
	}

	Button(){
		
	}

protected:
    
	/*
	LRESULT OnMouseMove(WPARAM wParam, LPARAM lParam) {
		MouseEventArgs e;
		e.button = MouseButtons::None;  // WM_MOUSEMOVE 不表示按键
		e.location.x = GET_X_LPARAM(lParam);
		e.location.y = GET_Y_LPARAM(lParam);
		e.modifiers = GetModifierKeys();  // 检查 Ctrl/Shift 状态
    
		if (onMouseMove) onMouseMove(*this, e);
		return 0;
	}
	*/

	LRESULT handleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
		switch (msg) {
			case WM_LBUTTONUP:
				if (onClick) onClick(*this);
				return 0;
			case WM_PAINT: {
				// 默认按钮绘制（或用系统默认）
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hwnd, &ps);
				// 让系统绘制
				DefWindowProc(hwnd, msg, wParam, lParam);
				EndPaint(hwnd, &ps);
				return 0;
			}
			default:
				return DefWindowProc(hwnd, msg, wParam, lParam);
		}
	}

	static LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		Button* pButton = reinterpret_cast<Button*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        
        if (pButton) {
            // 调用实例方法，访问该按钮特有的数据
            return pButton->handleMessage(hwnd, msg, wParam, lParam);
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);		
	}

	bool registerClass() {
		if (s_registered) return true;
    
		TCHAR className[32];
		_stprintf_s(className, _T("%d"), Control::generateId());

		WNDCLASSEX wc = {0};
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.lpfnWndProc = &Button::wndProc;
		wc.hInstance = GetModuleHandle(NULL);
		wc.lpszClassName = className;
		wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
		wc.style = CS_HREDRAW | CS_VREDRAW;
    
		if (!RegisterClassEx(&wc)) return false;
		s_registered = true;
		return true;
	}
    static bool s_registered;
    //static LRESULT CALLBACK staticButtonWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

};

// MouseEventArgs 定义
struct MouseEventArgs {
    MouseButtons button;        // 哪个按键触发
    Point location;             // 相对于控件客户区的坐标
    int clicks;                 // 1=单击, 2=双击
    ModifierKeys modifiers;     // Ctrl/Shift/Alt 状态
    bool handled;               // 可选：是否已处理（事件冒泡用）
    
    MouseEventArgs()
        : button(MouseButtons::None)
        , clicks(0)
        , modifiers(ModifierKeys::None)
        , handled(false) {}
};