#include "COverlappedWindow.h";
#include <iostream>

COverlappedWindow::COverlappedWindow() {}

COverlappedWindow::~COverlappedWindow() {}

bool COverlappedWindow::RegisterClass() {

	WNDCLASSEX wcex;
	::ZeroMemory(&wcex, sizeof(wcex));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.lpfnWndProc = COverlappedWindow::windowProc;
	wcex.hInstance = GetModuleHandle(0);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpszClassName = L"OverlappedWindow";
	return (::RegisterClassEx(&wcex) != 0);

}

bool COverlappedWindow::Create() {

	handle = CreateWindowEx(0, L"OverlappedWindow", L"My Window", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, GetModuleHandle(0), this);

	SetWindowLongPtr(handle, GWL_USERDATA, reinterpret_cast<LONG>(this));

	return (handle != NULL);

}

void COverlappedWindow::Show(int cmdShow) {

	// The parameters to ShowWindow explained:
	// hWnd: the value returned from CreateWindow
	// nCmdShow: the fourth parameter from WinMain
	ShowWindow(handle, cmdShow);
	//UpdateWindow(handle);

}

void COverlappedWindow::OnNCCreate(HWND _handle) {
	handle = _handle;
}

void COverlappedWindow::OnCreate() {

	xEllipse = 100;
	yEllipse = 100;
	xEllipseOld = 100;
	yEllipseOld = 100;
	SetTimer(handle, TimerID, 1, reinterpret_cast<TIMERPROC> (COverlappedWindow::windowProc));
}

void COverlappedWindow::OnClose() {
	if (MessageBox(handle, L"Really quit?", L"My application", MB_OKCANCEL) == IDOK) {
		DestroyWindow(handle);
	}
}

void COverlappedWindow::OnDestroy() {

	KillTimer(handle, TimerID);
	PostQuitMessage(0);

}

void COverlappedWindow::OnTimer() {
	
	long width;
	long height;
	getClientRect(width, height);

	xEllipseOld = xEllipse;
	yEllipseOld = yEllipse;
	xEllipse = (xEllipse + Delta) % width;
	yEllipse = (yEllipse + Delta) % height;

	InvalidateRect(handle, NULL, TRUE);
}

void COverlappedWindow::OnPaint() {
	
	static RECT rcClient; // client area rectangle
	GetClientRect(handle, &rcClient);
	long width = rcClient.right;
	long height = rcClient.bottom;

	getClientRect(width, height);

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(handle, &ps);
	HDC memHdc = CreateCompatibleDC(hdc);
	HBITMAP hBitmap = CreateCompatibleBitmap(memHdc, width, height);
	SelectObject(memHdc, hBitmap);

	//drawEllipse(memHdc, xEllipseOld, yEllipseOld, RGB(255, 255, 255), RGB(255, 255, 255));
	FillRect(memHdc, &rcClient, reinterpret_cast<HBRUSH>(GetStockObject(GRAY_BRUSH)));
	drawEllipse(memHdc, xEllipse, yEllipse, RGB(0, 0, 0), RGB(255, 25, 0));
	
	BitBlt(hdc, 0, 0, width, height, memHdc, 0, 0, SRCCOPY);
	DeleteObject(hBitmap);
	DeleteObject(memHdc);
	
	EndPaint(handle, &ps);
}

void COverlappedWindow::drawEllipse(HDC dc, int x, int y, COLORREF fillColor, COLORREF outlineColor) {
	
	HBRUSH brush = CreateSolidBrush(fillColor);
	SelectObject(dc, brush);
	
	HPEN pen = CreatePen(PS_DASH, 2, outlineColor);
	SelectObject(dc, pen);

	Ellipse(dc, x - Radius, y - Radius, x + Radius, y + Radius);

	DeleteObject(brush);
	DeleteObject(pen);
}

void COverlappedWindow::getClientRect(long &width, long &height) {
	
	static RECT rcClient; // client area rectangle
	GetClientRect(handle, &rcClient);
	width = rcClient.right;
	height = rcClient.bottom;

}

LRESULT __stdcall COverlappedWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {

	static COverlappedWindow *window;

	switch (message)
	{
	case WM_NCCREATE: {
		window = reinterpret_cast<COverlappedWindow*>(((CREATESTRUCT*)lParam)->lpCreateParams);
		window->OnNCCreate(handle);
		return DefWindowProc(handle, message, wParam, lParam);
	}
	case WM_CREATE: {
		window->OnCreate();
		return 0;
	}
	/*case WM_CLOSE: {
		window->OnClose();
		return 0;
	}*/
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

