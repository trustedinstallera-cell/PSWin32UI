#pragma once
#include <vector>
#include <functional>
#include "control.h"
#include "strings.h"
#include "ctrltype.h"

class Dialog :public Control {
public:
	std::wstring text;
	std::wstring message;
	std::vector<int> accepted;

	std::function<int()> run;

	bool model;
	
	Dialog(HWND parent = defaultHwnd_) :parent(this->parent) {
		run = nullptr;
	}
	/*
	void show() {
		switch (type) {
		case CtrlType::Yes:

		default:
			throw std::invalid_argument("Not dialog type.");
		}
	}*/

	

protected:
	HWND parent;
};

class ModalDialog : public Dialog {
protected:
    bool model;
    HWND parent;

    ModalDialog() : model(true) {
        this->parent = defaultHwnd_;
    }

    int show() {
        int result = -1;
        HWND oldFocus = nullptr;

        // 保存并禁用父窗口
        if (parent != nullptr && IsWindow(parent)) {
            oldFocus = GetFocus();
            EnableWindow(parent, FALSE);
            // 不要 ShowWindow 和 UpdateWindow
        }

        // 执行对话框逻辑
        if (this->run) {
            result = this->run();  // 直接调用，不需要 while 循环

            // 处理可能的递归调用
            if (result == -1 && this->run) {
                // 只在明确需要重试时才循环
                result = this->run();
            }
        }

        // 恢复父窗口
        if (parent != nullptr && IsWindow(parent)) {
            EnableWindow(parent, TRUE);
            if (oldFocus && IsWindow(oldFocus)) {
                SetFocus(oldFocus);  // 恢复焦点
            }
            SetForegroundWindow(parent);  // 将父窗口带到前台
        }

        return result;
    }
};

template<UINT dlgType>
class StandardModalDialog : public ModalDialog {
public:
    std::wstring text;

    StandardModalDialog(const std::wstring& message = L"",
        HWND parentWindow = nullptr,
        const std::wstring& caption = L"")
        : text(caption) {
        this->message = message;
        this->parent = parentWindow;

        this->run = [this]() -> int {
            HWND hWndParent = this->parent ? this->parent : GetActiveWindow();
            return MessageBox(hWndParent,
                this->message.c_str(),
                this->text.c_str(),
                dlgType);
        };
    }

    int show() {
        return ModalDialog::show();
    }
};

#define StdModalWindow(ClassName,Text,Prop)\
class ClassName : public StandardModalDialog<Prop> {\
public:\
    ClassName(const std::wstring& message, HWND parentWindow = NULL)\
        : StandardModalDialog(message, parentWindow, Text) {}\
\
    ClassName()\
        : StandardModalDialog(L"", NULL, g_dlgInfo) {}\
};

StdModalWindow(InfoDialog, g_dlgInfo, MB_OK | MB_ICONINFORMATION);
StdModalWindow(ErrorDialog, g_dlgError, MB_OK | MB_ICONERROR);
StdModalWindow(RetryDialog, g_dlgQuestion, MB_RETRYCANCEL | MB_ICONQUESTION);
StdModalWindow(AbortRetryDialog, g_dlgQuestion, MB_ABORTRETRYIGNORE | MB_ICONQUESTION);
StdModalWindow(YesNoDialog, g_dlgQuestion, MB_YESNO | MB_ICONQUESTION);
StdModalWindow(OKCancelDialog, g_dlgQuestion, MB_OKCANCEL | MB_ICONQUESTION);
StdModalWindow(RetryContinueDialog, g_dlgConfirm, MB_CANCELTRYCONTINUE | MB_ICONERROR);
StdModalWindow(WarningDialog, g_dlgWarning, MB_YESNO | MB_ICONWARNING);
