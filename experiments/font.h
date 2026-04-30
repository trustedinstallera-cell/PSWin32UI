#include "Color.h"
#pragma once
#include <windows.h>

enum FontUnit {
    Points,    // 点（1/72 英寸）
    Pixels,    // 屏幕像素
    Number     // 中文号数
};

class Font{
public:
    Color color;
    std::wstring name;
    bool isBold;
    bool isItalic;
    bool hasUnderline;
    bool hasStrikethrough;
    int size; //Windows 转换有精度损失，无法避免
    
    // 获取系统默认字体大小（点值）
    static double getSystemDefaultPoints() {
        static double cached = []() -> double {
            NONCLIENTMETRICS ncm = {0};
            ncm.cbSize = sizeof(NONCLIENTMETRICS);
            
            if (SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0)) {
                // lfHeight 是逻辑像素值（负数）
                int pixelHeight = -ncm.lfMessageFont.lfHeight;
                // 获取屏幕 DPI
                HDC hdc = GetDC(NULL);
                int dpi = GetDeviceCaps(hdc, LOGPIXELSY);
                ReleaseDC(NULL, hdc);
                // 像素转点：points = pixels * 72 / DPI
                return (double)pixelHeight * 72.0 / dpi;
            }
            
            // 回退：硬编码 9pt（Windows 经典默认值）
            return 9.0;
        }();
        return cached;
    }
    
    static double fromChineseNumber(const wchar_t* chineseNumber) {
            static const struct { const wchar_t* name; double points; } map[] = {
                {L"初号", 42.0},
                {L"小初", 36.0},
                {L"一号", 26.0},
                {L"二号", 22.0},
                {L"三号", 16.0},
                {L"四号", 14.0},
                {L"小四", 12.0},
                {L"五号", 10.5},    // 关键：半整数
                {L"小五", 9.0},
                {L"六号", 7.5},
                {L"七号", 5.5},
                {L"八号", 5.0},
                // 可继续扩展
            }; // 警告：初始化列表
            
            /*需要别名时改用
            static const std::pair<const wchar_t*, double> fontMap[]
            */
            
            for (size_t i = 0; i < sizeof(map)/sizeof(map[0]); ++i) {
                if (wcscmp(chineseNumber, map[i].name) == 0)
                    return map[i].points;
            }
            return 10.5; // 默认五号
        }
    };
        
    static int convertSize(int value, FontUnit fromUnit, HDC hdc = GetDC(NULL)) {
        switch (fromUnit) {
            case FontUnit::Points:
                return -MulDiv(value, GetDeviceCaps(hdc, LOGPIXELSY), 72);
            case FontUnit::Pixels:
                return -value;
            case FontUnit::Number:
                // 号数转点再转像素
                // TODO
				//return -MulDiv(numberToPoints(value), GetDeviceCaps(hdc, LOGPIXELSY), 72);
        }
        return -value;
    }


// 字体单位转换宏定义
// 像素单位
#define _px(val) UI::Font::convertSize(val, UI::FontUnit::Pixels)
// 点数单位
#define _pt(val) UI::Font::convertSize(val, UI::FontUnit::Points)
// 字号单位
#define _n(val)  UI::Font::convertSize(val, UI::FontUnit::Number)

/* 使用示例
Font font;
font.size = _px(16);   // 16 像素
font.size = _pt(14);   // 14 点
font.size = _n("五号");     // 五号字
*/

/* 参考函数
    HFONT CreateFontA(
      int     cHeight,        // 字体高度（逻辑单位）
      int     cWidth,         // 字符平均宽度
      int     cEscapement,    // 角度
      int     cOrientation,
      int     cWeight,        // 粗细（FW_NORMAL = 400）
      DWORD   bItalic,
      DWORD   bUnderline,
      DWORD   bStrikeOut,
      DWORD   iCharSet,
      DWORD   iOutPrecision,
      DWORD   iClipPrecision,
      DWORD   iQuality,
      DWORD   iPitchAndFamily,
      LPCSTR  lpszFace
    );
*/
