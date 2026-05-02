#pragma once
#include <string>
#include <stdexcept>

// 1. 枚举改名（避免下划线开头）
enum ColorType {  // 原 _color
    Black, Blue, Green, Cyan, Red, Purple, Yellow, White,
    Gray, LightBlue, LightGreen, LightCyan, LightRed,
    LightPurple, LightYellow, BrightWhite
};

struct RGB {
    int r;
    int g;
    int b;
};

class Color {
protected:
    ColorType standardColor;
    RGB rgb;
    bool isStandard;  // 新增：区分是标准色还是RGB
public:
    // 默认构造函数
    Color() : isStandard(true), standardColor(Black) {
        rgb.r = 0;
        rgb.g = 0;
        rgb.r = 0;
    }

    Color(ColorType color) : isStandard(true), standardColor(color) {}

    Color(RGB rgb) : isStandard(false), rgb(rgb) {
        if (rgb.r >= 256 || rgb.r < 0 ||
            rgb.g >= 256 || rgb.g < 0 ||
            rgb.b >= 256 || rgb.b < 0) {
            throw std::out_of_range("RGB parameter out of range.");
        }
    }

    Color(std::string str) : isStandard(true), standardColor(Black) {
        try {
            if (str.length() != std::char_traits<char>::length("#RRGGBB")) {
                throw std::length_error("RGB string must be exactly 7 characters (including '#').");
            }

            RGB rgb_;
            std::string subStrR(str.begin() + 1, str.begin() + 3);
            rgb_.r = std::stoi(subStrR, nullptr, 16);
            std::string subStrG(str.begin() + 3, str.begin() + 5);
            rgb_.g = std::stoi(subStrG, nullptr, 16);
            std::string subStrB(str.begin() + 5, str.begin() + 7);
            rgb_.b = std::stoi(subStrB, nullptr, 16);
        }
        catch (std::invalid_argument) {
            throw std::invalid_argument("Format must be '#RRGGBB'");
        }
        catch (...) {
            throw;
        }
        // 参数如 #6A5A68
    }

    //get...
    //set...
};
