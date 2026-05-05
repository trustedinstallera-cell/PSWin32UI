#pragma once
#include "ctrltype.h"
#include <string>
#include <Windows.h>

class PictureBox {
public:
	std::wstring path;
	Ctrltype type;
	PictureBox() {}

	HICON load(HINSTANCE parent=GetModuleHandle(NULL)) {
		if (!path.empty()) {
			const wchar_t* path_ = path.c_str();
			switch (type) {
			case Ctrltype::FromResource:
				return LoadIcon(parent, MAKEINTRESOURCE(path_));
			case CtrlType::FromFile:
				return (HICON)LoadImage(NULL, path_, IMAGE_ICON, 0, 0, LR_LOADFROMFILE); // ?
			case Ctrltype::FromIcon:

			}
		}
		return nullptr;
	}
};