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
		rgb.r=0;
		rgb.g=0;
		rgb.r=0;
	}

    Color(ColorType color) : isStandard(true), standardColor(color) {}

    Color(RGB rgb) : isStandard(false), rgb(rgb) {
        if(rgb.r>=256||rgb.r<0||
                rgb.g>=256||rgb.g<0||
                rgb.b>=256||rgb.b<0) {
            throw std::out_of_range("RGB parameter out of range.");
        }
    }

    Color(std::string str) : isStandard(true), standardColor(Black) {
        // 参数如 #6A5A68，暂留空实现
    }
    
    //get...
    //set...
};
