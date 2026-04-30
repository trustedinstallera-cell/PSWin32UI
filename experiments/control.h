#include <Windows.h>
#include <stdexcept>

struct Point{
    int x;
    int y;
};

struct Size{
    int x;
    int y;
};

LRESULT CALLBACK Proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
    
}



class Control {
protected:
	  int id_;
	  HWND hwnd;

	 ~Control() {
		if (this->hwnd) {
			DestroyWindow(this->hwnd);
		}
	 }

	 void registerClass() {
		static bool registered = false;
		if (registered) return;
    
		WNDCLASSEX wc = {0};
		wc.cbSize = sizeof(WNDCLASSEX);
		//wc.lpfnWndProc = &Control::staticWndProc;
		wc.hInstance = GetModuleHandle(NULL);
		wc.lpszClassName = L"PSWin32UIControl";
		wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    
		RegisterClassEx(&wc);
		registered = true;
	}

	 LRESULT wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		
	}

	 int generateId(){
		  return this->id_++;
	 }

public:
	 Control() : hwnd(nullptr) {
		this->id_=1000;
	 }

};
