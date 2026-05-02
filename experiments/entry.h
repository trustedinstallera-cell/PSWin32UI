#pragma once
#include <Windows.h>

#define Main() { \
struct Param {\
    HINSTANCE hInstance;\
    LPSTR lpCmdLine;\
    int nCmdShow;\
} param;\
\
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR lpCmd, int nShow)\
{\
    param.hInstance = hInst;\
    param.lpCmdLine = lpCmd;\
    param.nCmdShow = nShow;
