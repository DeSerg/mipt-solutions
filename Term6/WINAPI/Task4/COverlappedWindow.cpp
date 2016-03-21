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
	wcex.hCursor = LoadCursor(GetModuleHandle(0), MAKEINTRESOURCE(230));
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
}

HWND COverlappedWindow::getHandle() {
	return handle;
}

void COverlappedWindow::OnNCCreate(HWND _handle) {
	handle = _handle;
}

void COverlappedWindow::OnCreate() {

	textChanged = false;

	hwndEdit = CreateWindowEx(
                    0, L"EDIT",   // predefined class 
                    NULL,         // no window title 
                    WS_CHILD | WS_VISIBLE | WS_VSCROLL | 
                    ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL, 
                    0, 0, 0, 0,   // set size in WM_SIZE message 
                    handle,         // parent window 
                    (HMENU) ID_EDITCHILD,   // edit control ID 
                    (HINSTANCE) GetWindowLong(handle, GWL_HINSTANCE), 
                    NULL);        // pointer not needed 
}

void COverlappedWindow::OnClose() {
	if (textChanged) {
		MessageBox(handle, L"Would you like save your text to file?", L"My Window", MB_YESNOCANCEL);
	}
}

void COverlappedWindow::OnDestroy() {
	PostQuitMessage(0);
}

void COverlappedWindow::OnSize(WPARAM wParam, LPARAM lParam) {
	MoveWindow(hwndEdit, 
			0, 0,                  // starting x- and y-coordinates 
			LOWORD(lParam),        // width of client area 
			HIWORD(lParam),        // height of client area 
			TRUE);                 // repaint window 
}

int COverlappedWindow::OnCommand(WPARAM wParam) {
	switch (HIWORD(wParam)) { 
	case EN_CHANGE:
		textChanged = true;
		break;
	default:
			return 1;
	}

	return 0;
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
		window = reinterpret_cast<COverlappedWindow*>((reinterpret_cast<CREATESTRUCT*>(lParam))->lpCreateParams);
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
	case WM_CLOSE: {
		window->OnClose();
	}
	case WM_DESTROY: {
		window->OnDestroy();
		return 0;
	}
	case WM_SIZE: {
		window->OnSize(wParam, lParam);
		return 0;
	}
	case WM_COMMAND: {
		if (window->OnCommand(wParam) == 1) {
			return DefWindowProc(handle, message, wParam, lParam);
		}
	}
	case WM_SETFOCUS: 
            SetFocus(window->hwndEdit); 
            return 0; 

	default: {
		return DefWindowProc(handle, message, wParam, lParam);
	}
	}
	
}

