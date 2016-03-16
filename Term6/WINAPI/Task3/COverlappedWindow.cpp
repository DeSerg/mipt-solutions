#include "COverlappedWindow.h"
#include "COverlappedWindow.h"

COverlappedWindow::COverlappedWindow() {}

COverlappedWindow::~COverlappedWindow() {}

bool COverlappedWindow::RegisterClass() {

	WNDCLASSEX wcex;
	::ZeroMemory(&wcex, sizeof(wcex));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.lpfnWndProc = COverlappedWindow::windowProc;
	wcex.hInstance = GetModuleHandle(0);
	wcex.lpszClassName = L"OverlappedWindow";
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	return (::RegisterClassEx(&wcex) != 0);

}

bool COverlappedWindow::Create() {

	CreateWindowEx(0, L"OverlappedWindow", L"My Window", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, GetModuleHandle(0), this);
	return (handle != NULL);

}

void COverlappedWindow::Show(int cmdShow) {

	ShowWindow(handle, cmdShow);
	UpdateWindow(handle);

	for (int i = 0; i < childWindows.size(); i++) {
		childWindows[i].Show(cmdShow);
	}

}

void COverlappedWindow::OnNCCreate(HWND _handle) {
	handle = _handle;
}

void COverlappedWindow::OnCreate() {

	CEllipseWindow::RegisterClass();
	for (int i = 0; i < childWindows.size(); i++) {
		childWindows[i].Create(handle);
	}

}

void COverlappedWindow::OnDestroy() {
	PostQuitMessage(0);
}

void COverlappedWindow::OnSize() {

	RECT clientRect;
	::GetClientRect(handle, &clientRect);	
	long clientWidth, clientHeight;
	
	int left = clientRect.left;
	int top = clientRect.top;

	int childWidth = (clientRect.right - clientRect.left) / 2;
	int childHeight = (clientRect.bottom - clientRect.top) / 2;
	int childMidX = (clientRect.left + clientRect.right) / 2;
	int childMidY = (clientRect.top + clientRect.bottom) / 2;

	SetWindowPos(childWindows[0].getHandle(), HWND_TOP, clientRect.left, clientRect.top, childWidth, childHeight, 0);
	SetWindowPos(childWindows[1].getHandle(), HWND_TOP, childMidX, clientRect.top, childWidth, childHeight, 0);
	SetWindowPos(childWindows[2].getHandle(), HWND_TOP, clientRect.left, childMidY, childWidth, childHeight, 0);
	SetWindowPos(childWindows[3].getHandle(), HWND_TOP, childMidX, childMidY, childWidth, childHeight, 0);

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
		return DefWindowProc(handle, message, wParam, lParam);
	}
	case WM_DESTROY: {
		window->OnDestroy();
		return 0;
	}
	case WM_SIZE: {
		window->OnSize();
		return DefWindowProc(handle, message, wParam, lParam);;
	}
	default: {
		return DefWindowProc(handle, message, wParam, lParam);
	}
	}
	
}

