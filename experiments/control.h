#pragma once
#ifndef UNICODE
#define UNICODE
#endif

#ifndef DISABLE_VISUAL_STYLES
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

#include <Windows.h>
#include <stdexcept>
#include <cassert>
#include "position.h"

struct Point {
	struct {
		int x;
		int y;
	};
	Position position;
	Point() :x(-1), y(-1), position(Manual) {}
	Point(int x, int y) :x(x), y(y) {}
};

struct Size {
	int x;
	int y;
	Size() :x(800), y(600) {}
	Size(int x, int y) { this->x = x; this->y = y; }
};


struct Param {
	HINSTANCE hInstance;
	LPSTR lpCmdLine;
	int nCmdShow;
} param;

/*LRESULT CALLBACK Proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

}*/

bool fixDefaultHwnd = false;
HWND defaultHwnd_ = nullptr;

class Control {
public:	
	HWND hwnd;
	Size size;
	Point startPosition;
	bool isVisible;
	bool enabled;
protected:
	int idCnt_;
	int id;
	virtual ~Control() {
		if (this->hwnd) {
			DestroyWindow(this->hwnd);
		}
	}

	void registerClass() {
		static bool registered = false;
		if (registered) return;

		WNDCLASSEX wc = { 0 };
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

	int generateId() {
		this->id = this->idCnt_++;
		return this->id;
	}

public:
	Control() : hwnd(nullptr) {
		this->id = generateId();
		defaultHwnd_ = nullptr;
	}

	void dispose() {
		this->~Control();
	}

	HWND getHandle() const {
		return this->hwnd;
	}

	void setSize(Size& size_) {
		if (size_.x < 0 || size_.y < 0) {
			assert(false && "Invalid size");
			return;
		}
		this->size = size_;
	}

	Size getSize(void) {
		return size;
	}

	void setStartPosition(Point& pos) {
		this->startPosition = pos;
	}

	Point getStartPosition(void) {
		return this->startPosition;
	}

	void visible(bool isVisible) {
		this->isVisible = isVisible;
	}

	bool visible(void) {
		return this->isVisible;
	}

	void enable(bool enabled_) {
		if (this->hwnd) {
			EnableWindow(hwnd, enabled_);
			this->enabled = enabled_;
		}
	}

	bool enable() {
		return this->enabled;
	}

	void focus() {
		enable(true);
		SetFocus(this->hwnd);
	}

	void text() {

	}

};
