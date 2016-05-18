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
	wcex.hIconSm = NULL;
	wcex.hIcon = (HICON)LoadImage(NULL, L"icon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
	return (::RegisterClassEx(&wcex) != 0);

}

bool COverlappedWindow::Create() {

	handle = CreateWindowEx(windowExStyle, L"OverlappedWindow", L"Sudoku", windowStyle,
		CW_USEDEFAULT, CW_USEDEFAULT, winSize, winSize, 0, 0, GetModuleHandle(0), this);

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

	RECT clientRect = { 0, 0, cellSize * 9 + menuWidth, cellSize * 9 };
	AdjustWindowRectEx(&clientRect, windowStyle, FALSE, windowExStyle);

	SetWindowPos(handle, HWND_TOP, 0, 0, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, SWP_NOMOVE);

	initDigits();


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


	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(handle, &ps);
	
	drawMenu(hdc);
	drawField(hdc);
	
	/*
	for (int x = 0; x < width; x += 75) {
		drawDigit(hdc, x, 0, x % 10, false);
	}
	*/
	

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

void COverlappedWindow::initDigits() {
	digits.resize(11);
	digits[0] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BMP_0));
	digits[1] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BMP_1));
	digits[2] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BMP_2));
	digits[3] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BMP_3));
	digits[4] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BMP_4));
	digits[5] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BMP_5));
	digits[6] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BMP_6));
	digits[7] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BMP_7));
	digits[8] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BMP_8));
	digits[9] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BMP_9));
	digits[10] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BMP_BLANK));

	circledDigits.resize(10);
	circledDigits[1] = (HBITMAP)LoadImage(NULL, L"circled_1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_SHARED);
	circledDigits[2] = (HBITMAP)LoadImage(NULL, L"circled_1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_SHARED);
	circledDigits[3] = (HBITMAP)LoadImage(NULL, L"circled_1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_SHARED);
	circledDigits[4] = (HBITMAP)LoadImage(NULL, L"circled_1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_SHARED);
	circledDigits[5] = (HBITMAP)LoadImage(NULL, L"circled_1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_SHARED);
	circledDigits[6] = (HBITMAP)LoadImage(NULL, L"circled_1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_SHARED);
	circledDigits[7] = (HBITMAP)LoadImage(NULL, L"circled_1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_SHARED);
	circledDigits[8] = (HBITMAP)LoadImage(NULL, L"circled_1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_SHARED);
	circledDigits[9] = (HBITMAP)LoadImage(NULL, L"circled_1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_SHARED);

	
}

void COverlappedWindow::ErrorExit(LPTSTR lpszFunction)
{
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	ExitProcess(dw);
}

void COverlappedWindow::drawField(HDC hdc) {

	long width, height;
	getClientRect(width, height);

	//thick lines
	HPEN pen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
	SelectObject(hdc, pen);

	for (int coord = 0; coord <= cellSize * 9; coord += cellSize * 3) {
		MoveToEx(hdc, coord, 0, NULL);
		LineTo(hdc, coord, height);
		MoveToEx(hdc, 0, coord, NULL);
		LineTo(hdc, width, coord);
	}

	DeleteObject(pen);

	//thin lines
	pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	SelectObject(hdc, pen);

	for (int coord = 0; coord <= cellSize * 9; coord += cellSize) {
		if (coord % (cellSize * 3) == 0) {
			continue;
		}
		MoveToEx(hdc, coord, 0, NULL);
		LineTo(hdc, coord, height);
		MoveToEx(hdc, 0, coord, NULL);
		LineTo(hdc, width, coord);
	}

	DeleteObject(pen);

}

void COverlappedWindow::drawMenu(HDC hdc) {

	HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
	SelectObject(hdc, brush);
	Rectangle(hdc, winSize, 0, winSize + menuWidth, winSize);
	DeleteObject(brush);

	for (int i = 0; i < 9; i++) {
		drawBitmap(hdc, winSize + (menuWidth / 2 - cellSize / 2), i * cellSize, circledDigits[i + 1], false);
	}
}

void COverlappedWindow::drawBitmap(HDC hdc, int x, int y, HBITMAP bitmap, bool redrawField) {

	BITMAP bm;

	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP hbmOld = reinterpret_cast<HBITMAP>(SelectObject(hdcMem, bitmap));

	GetObject(bitmap, sizeof(bm), &bm);

	BitBlt(hdc, x, y, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

	SelectObject(hdcMem, hbmOld);
	DeleteDC(hdcMem);

	if (redrawField) {
		drawField(hdc);
	}
}

void COverlappedWindow::drawDigit(HDC hdc, int x, int y, int digit, bool redrawField) {

	BITMAP bm;

	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP hbmOld = reinterpret_cast<HBITMAP>(SelectObject(hdcMem, (digits[digit])));

	GetObject(digits[digit], sizeof(bm), &bm);

	BitBlt(hdc, x, y, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

	SelectObject(hdcMem, hbmOld);
	DeleteDC(hdcMem);

	if (redrawField) {
		drawField(hdc);
	}
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

