#pragma once
#define UNICODE

#include <gl/glew.h>
#include <gl/wglew.h>
#include <Windows.h>

HWND CreateWin();

LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

HGLRC CreateGLContext(HWND hwndparent);

void MSGLoop();

HWND CreateDummyWin(HWND hwndparent);

LRESULT CALLBACK DummyWndProc(HWND dhwnd, UINT dumsg, WPARAM dwparam, LPARAM dlparam);

HGLRC CreateDummyCont(HDC hdc);