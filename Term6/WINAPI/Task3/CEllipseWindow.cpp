#include "CEllipseWindow.h"
#include "COverlappedWindow.h"

CEllipseWindow::CEllipseWindow() {}

CEllipseWindow::~CEllipseWindow() {}

bool CEllipseWindow::RegisterClass() {

	WNDCLASSEX wcex;
	::ZeroMemory(&wcex, sizeof(wcex));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.lpfnWndProc = CEllipseWindow::windowProc;
	wcex.hInstance = GetModuleHandle(0);
	wcex.lpszClassName = L"EllipseWindow";
	wcex.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	return (::RegisterClassEx(&wcex) != 0);

}

bool CEllipseWindow::Create(HWND parentHandle) {

	CreateWindowEx(0, L"EllipseWindow", L"ChildWindow", WS_CHILD | WS_BORDER,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, parentHandle, 0, GetModuleHandle(0), this);
	return (handle != NULL);

}

void CEllipseWindow::Show(int cmdShow) {
	ShowWindow(handle, cmdShow);
	UpdateWindow(handle);
}

HWND CEllipseWindow::getHandle() {
	return handle;
}

void CEllipseWindow::OnNCCreate(HWND _handle) {
	handle = _handle;
}

void CEllipseWindow::OnCreate() {

	long width;
	long height;
	getClientRect(width, height);
	xEllipseOld = xEllipse = 0;
	yEllipseOld = yEllipse = 0;
	//int result = SetTimer(handle, TimerID, TimeDelta, reinterpret_cast<TIMERPROC> (CEllipseWindow::windowProc));
}

void CEllipseWindow::OnDestroy() {

	//KillTimer(handle, TimerID);
	PostQuitMessage(0);

}

void CEllipseWindow::OnTimer() {
	
	long width;
	long height;
	getClientRect(width, height);

	xEllipseOld = xEllipse;
	yEllipseOld = yEllipse;
	xEllipse = (xEllipse + DistanceDelta) % width;
	yEllipse = (yEllipse + DistanceDelta) % height;

	InvalidateRect(handle, NULL, TRUE);
}

void CEllipseWindow::OnPaint() {
	
	static RECT rcClient; // client area rectangle
	GetClientRect(handle, &rcClient);
	long width = rcClient.right;
	long height = rcClient.bottom;

	getClientRect(width, height);

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(handle, &ps);
	HDC memHdc = CreateCompatibleDC(hdc);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, width, height);
	SelectObject(memHdc, hBitmap);

	//drawEllipse(memHdc, xEllipseOld, yEllipseOld, RGB(255, 255, 255), RGB(255, 255, 255));
	FillRect(memHdc, &rcClient, reinterpret_cast<HBRUSH>(GetStockObject(GRAY_BRUSH)));
	drawEllipse(memHdc, xEllipse, yEllipse, RGB(0, 0, 0), RGB(255, 25, 0));
	
	BitBlt(hdc, 0, 0, width, height, memHdc, 0, 0, SRCCOPY);
	DeleteObject(hBitmap);
	DeleteObject(memHdc);
	
	EndPaint(handle, &ps);
}

void CEllipseWindow::drawEllipse(HDC dc, int x, int y, COLORREF fillColor, COLORREF outlineColor) {
	
	HBRUSH brush = CreateSolidBrush(fillColor);
	SelectObject(dc, brush);
	
	HPEN pen = CreatePen(PS_DASH, 2, outlineColor);
	SelectObject(dc, pen);

	Ellipse(dc, x - Radius, y - Radius, x + Radius, y + Radius);

	DeleteObject(brush);
	DeleteObject(pen);
}

void CEllipseWindow::getClientRect(long &width, long &height) {
	
	static RECT rcClient; // client area rectangle
	GetClientRect(handle, &rcClient);
	width = rcClient.right;
	height = rcClient.bottom;

}

LRESULT __stdcall CEllipseWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {

	static CEllipseWindow *window;

	switch (message)
	{
	case WM_NCCREATE: {
		window = reinterpret_cast<CEllipseWindow*>(((CREATESTRUCT*)lParam)->lpCreateParams);
		SetLastError(0);
        SetWindowLongPtr(handle, GWLP_USERDATA, reinterpret_cast<LONG>(window));
        if( GetLastError() != 0 ) {
            return GetLastError();
        }
		window->OnNCCreate(handle);
		return DefWindowProc(handle, message, wParam, lParam);
	}
	case WM_CREATE: {
		window->OnCreate();
		return 0;
	}
	case WM_DESTROY: {
		window->OnDestroy();
		return 0;
	}
	case WM_TIMER: {
		window->OnTimer();
		return 0;
	}
	case WM_PAINT: {
		window->OnPaint();
		return 0;
	}
	default: {
		return DefWindowProc(handle, message, wParam, lParam);
	}
	}
	
}

