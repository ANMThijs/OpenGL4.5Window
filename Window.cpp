#include "Window.h"

HWND CreateWin() {
	HINSTANCE hInst = GetModuleHandleW(NULL);

	WNDCLASS wc = {};

	wc.lpszClassName = L"Main class";
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInst;

	RegisterClassW(&wc);

	HWND hwnd = CreateWindowExW(NULL, wc.lpszClassName, L"Main window",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_MAXIMIZE,
		0, 0, 1920, 1080,
		NULL, NULL, hInst, NULL);

	return hwnd;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
	HDC hdc = NULL;
	HGLRC hrc = NULL;
	switch (umsg) {
	case WM_PAINT:
		hdc = GetDC(hwnd);

		glClear(GL_COLOR_BUFFER_BIT);

		SwapBuffers(hdc);
		break;
	case WM_CREATE:
		hdc = GetDC(hwnd);
		hrc = CreateGLContext(hwnd);
		wglMakeCurrent(hdc, hrc);

		glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProcW(hwnd, umsg, wparam, lparam);
}

HGLRC CreateGLContext(HWND hwndparent) {
	HWND dhwnd = CreateDummyWin(hwndparent);

	HDC hdc = GetDC(hwndparent);

	glewInit();

	PIXELFORMATDESCRIPTOR pfd = {};

	int attribslist[] = {
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB, 32,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		0,
	};

	int iPixFor;
	UINT numFor;

	wglChoosePixelFormatARB(hdc, attribslist, NULL, 1, &iPixFor, &numFor);
	SetPixelFormat(hdc, iPixFor, &pfd);

	int attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 5,
		WGL_CONTEXT_FLAGS_ARB, 0,
		0
	};

	HGLRC hrc = wglCreateContextAttribsARB(hdc, 0, attribs);

	DestroyWindow(dhwnd);

	return hrc;
}

void MSGLoop() {
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
}

HWND CreateDummyWin(HWND hwndparent) {
	WNDCLASS dwc = {};

	dwc.lpszClassName = L"Dummy class";
	dwc.lpfnWndProc = DummyWndProc;

	RegisterClassW(&dwc);

	HWND dhwnd = CreateWindowExW(NULL, dwc.lpszClassName, L"Dummy win",
		WS_CHILD,
		0, 0, 0, 0,
		hwndparent, NULL, NULL, NULL);

	return dhwnd;
}

LRESULT CALLBACK DummyWndProc(HWND dhwnd, UINT dumsg, WPARAM dwparam, LPARAM dlparam) {
	HDC dhdc = NULL;
	HGLRC dhrc = NULL;
	switch (dumsg) {
	case WM_CREATE:
		dhdc = GetDC(dhwnd);
		dhrc = CreateDummyCont(dhdc);
		wglMakeCurrent(dhdc, dhrc);
		break;
	case WM_DESTROY:
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(dhrc);
		ReleaseDC(dhwnd, dhdc);
		break;
	}
	return DefWindowProcW(dhwnd, dumsg, dwparam, dlparam);
}

HGLRC CreateDummyCont(HDC dhdc) {
	PIXELFORMATDESCRIPTOR dpfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,
		8,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	int dPixFor = ChoosePixelFormat(dhdc, &dpfd);
	SetPixelFormat(dhdc, dPixFor, &dpfd);

	HGLRC DummyC = wglCreateContext(dhdc);

	return DummyC;
}