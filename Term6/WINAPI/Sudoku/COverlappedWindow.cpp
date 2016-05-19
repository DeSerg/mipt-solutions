#include "COverlappedWindow.h"
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
	wcex.hIcon = (HICON)LoadImage(NULL, L"res\\icon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
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

//window messages
void COverlappedWindow::OnCreate() {

	RECT clientRect = { 0, 0, cellSize * 9 + menuWidth, cellSize * 9 };
	AdjustWindowRectEx(&clientRect, windowStyle, FALSE, windowExStyle);

	SetWindowPos(handle, HWND_TOP, 0, 0, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, SWP_NOMOVE);
	

	initData();

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
	drawGrid(hdc);
	
	EndPaint(handle, &ps);

}

void COverlappedWindow::On_LButtonDown(WPARAM wParam, LPARAM lParam) {

	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);

	if (wParam == MK_LBUTTON) {
		for (int i = 0; i <= 10; ++i) {
			if (x >= menuRects[i].left && x <= menuRects[i].right &&
				y >= menuRects[i].top && y <= menuRects[i].bottom) {
				if (i == 10) {
					menuButtons[10] = menuButtonPressedBMP;
				} else {
					menuButtons[i] = menuDigitsPressed[i];
				}
			}

		}


		for (int i = 0; i < 9; ++i) {
			for (int j = 0; j < 9; ++j) {
				if (x >= i * cellSize && x <= (i + 1) * cellSize &&
					y >= j * cellSize && y <= (j + 1) * cellSize) {
					isDigitPressed = true;
					iPressed = i;
					jPressed = j;
				}
			}
		}

	}

	InvalidateRect(handle, NULL, TRUE);
}

void COverlappedWindow::On_LButtonUp(WPARAM wParam, LPARAM lParam) {

	if (menuButtons[10] == menuButtonPressedBMP) {
		int action = DialogBox(GetModuleHandle(0), MAKEINTRESOURCE(IDD_MENU_DIALOG), handle, reinterpret_cast<DLGPROC>(DialogProc));
		switch (action) {
		case START:
			sudoku.getRandomGrid(grid);
			break;
		case RESTART:
			sudoku.getPreviousGrid(grid);
			break;
		case DEV:
			MessageBox(handle, L"The game is created by Serg Popov", L"Developers", MB_OK);
			break;
		default:
			break;
		}
	}
	menuButtons[10] = menuButtonBMP;

	for (int i = 0; i <= 9; ++i) {
		if (menuButtons[i] == menuDigitsPressed[i] && isDigitPressed) {

			isDigitPressed = false;
			grid[iPressed][jPressed] = i;

		}

		menuButtons[i] = menuDigits[i];
	}

	InvalidateRect(handle, NULL, TRUE);
}


//menu messages
void COverlappedWindow::OnInitMenuDialog(HWND hwndDlg) {

	RECT sudokuRect;
	GetWindowRect(handle, &sudokuRect);

	RECT menuRect;
	GetWindowRect(hwndDlg, &menuRect);

	SetWindowPos(hwndDlg, HWND_TOP, sudokuRect.left + (sudokuRect.right - sudokuRect.left) / 2 - (menuRect.right - menuRect.left) / 2,
					sudokuRect.top + (sudokuRect.bottom - sudokuRect.top) / 6, 0, 0, SWP_NOSIZE);

}

//sudoku methods
void COverlappedWindow::initData() {

	loadResources();


	//grid init
	sudoku.getRandomGrid(grid);

	isDigitPressed = false;
	iPressed = 0;
	jPressed = 0;

	//menu buttons init
	menuButtons.resize(11);
	menuButtons[10] = menuButtonBMP;
	for (int i = 0; i <= 9; ++i) {
		menuButtons[i] = menuDigits[i];
	}

	//menu rectangles init
	menuRects.resize(11);

	RECT rect = {
		winSize + (menuWidth / 2 - menuItemSize / 2),
		(winSize - menuMarginBottom - menuMarginTop - 10 * menuItemSize) / 2 - menuItemSize / 2,
		winSize + (menuWidth / 2 - menuItemSize / 2) + menuItemSize,
		(winSize - menuMarginBottom - menuMarginTop - 10 * menuItemSize) / 2 + menuItemSize / 2 };

	menuRects[10] = rect;

	for (int i = 0; i <= 9; ++i) {
		RECT rect = {
			winSize + (menuWidth / 2 - menuItemSize / 2),
			winSize - menuMarginBottom - 10 * menuItemSize + i * menuItemSize,
			winSize + (menuWidth / 2 - menuItemSize / 2) + menuItemSize,
			winSize - menuMarginBottom - 10 * menuItemSize + i * menuItemSize + menuItemSize };

		menuRects[i] = rect;
	}

}

void COverlappedWindow::getClientRect(long &width, long &height) {
	
	static RECT rcClient; // client area rectangle
	GetClientRect(handle, &rcClient);
	width = rcClient.right;
	height = rcClient.bottom;

}

void COverlappedWindow::loadResources() {

	digits.resize(10);
	digits[0] = (HBITMAP)LoadImage(NULL, L"res\\0.bmp", IMAGE_BITMAP, cellSize, cellSize, LR_LOADFROMFILE | LR_SHARED);
	digits[1] = (HBITMAP)LoadImage(NULL, L"res\\1.bmp", IMAGE_BITMAP, cellSize, cellSize, LR_LOADFROMFILE | LR_SHARED);
	digits[2] = (HBITMAP)LoadImage(NULL, L"res\\2.bmp", IMAGE_BITMAP, cellSize, cellSize, LR_LOADFROMFILE | LR_SHARED);
	digits[3] = (HBITMAP)LoadImage(NULL, L"res\\3.bmp", IMAGE_BITMAP, cellSize, cellSize, LR_LOADFROMFILE | LR_SHARED);
	digits[4] = (HBITMAP)LoadImage(NULL, L"res\\4.bmp", IMAGE_BITMAP, cellSize, cellSize, LR_LOADFROMFILE | LR_SHARED);
	digits[5] = (HBITMAP)LoadImage(NULL, L"res\\5.bmp", IMAGE_BITMAP, cellSize, cellSize, LR_LOADFROMFILE | LR_SHARED);
	digits[6] = (HBITMAP)LoadImage(NULL, L"res\\6.bmp", IMAGE_BITMAP, cellSize, cellSize, LR_LOADFROMFILE | LR_SHARED);
	digits[7] = (HBITMAP)LoadImage(NULL, L"res\\7.bmp", IMAGE_BITMAP, cellSize, cellSize, LR_LOADFROMFILE | LR_SHARED);
	digits[8] = (HBITMAP)LoadImage(NULL, L"res\\8.bmp", IMAGE_BITMAP, cellSize, cellSize, LR_LOADFROMFILE | LR_SHARED);
	digits[9] = (HBITMAP)LoadImage(NULL, L"res\\9.bmp", IMAGE_BITMAP, cellSize, cellSize, LR_LOADFROMFILE | LR_SHARED);

	digitsPressed.resize(10);
	digitsPressed[0] = (HBITMAP)LoadImage(NULL, L"res\\0_pressed.bmp", IMAGE_BITMAP, cellSize, cellSize, LR_LOADFROMFILE | LR_SHARED);
	digitsPressed[1] = (HBITMAP)LoadImage(NULL, L"res\\1_pressed.bmp", IMAGE_BITMAP, cellSize, cellSize, LR_LOADFROMFILE | LR_SHARED);
	digitsPressed[2] = (HBITMAP)LoadImage(NULL, L"res\\2_pressed.bmp", IMAGE_BITMAP, cellSize, cellSize, LR_LOADFROMFILE | LR_SHARED);
	digitsPressed[3] = (HBITMAP)LoadImage(NULL, L"res\\3_pressed.bmp", IMAGE_BITMAP, cellSize, cellSize, LR_LOADFROMFILE | LR_SHARED);
	digitsPressed[4] = (HBITMAP)LoadImage(NULL, L"res\\4_pressed.bmp", IMAGE_BITMAP, cellSize, cellSize, LR_LOADFROMFILE | LR_SHARED);
	digitsPressed[5] = (HBITMAP)LoadImage(NULL, L"res\\5_pressed.bmp", IMAGE_BITMAP, cellSize, cellSize, LR_LOADFROMFILE | LR_SHARED);
	digitsPressed[6] = (HBITMAP)LoadImage(NULL, L"res\\6_pressed.bmp", IMAGE_BITMAP, cellSize, cellSize, LR_LOADFROMFILE | LR_SHARED);
	digitsPressed[7] = (HBITMAP)LoadImage(NULL, L"res\\7_pressed.bmp", IMAGE_BITMAP, cellSize, cellSize, LR_LOADFROMFILE | LR_SHARED);
	digitsPressed[8] = (HBITMAP)LoadImage(NULL, L"res\\8_pressed.bmp", IMAGE_BITMAP, cellSize, cellSize, LR_LOADFROMFILE | LR_SHARED);
	digitsPressed[9] = (HBITMAP)LoadImage(NULL, L"res\\9_pressed.bmp", IMAGE_BITMAP, cellSize, cellSize, LR_LOADFROMFILE | LR_SHARED);

	menuDigits.resize(10);
	menuDigits[0] = (HBITMAP)LoadImage(NULL, L"res\\circled_0.bmp", IMAGE_BITMAP, menuItemSize, menuItemSize, LR_LOADFROMFILE | LR_SHARED);
	menuDigits[1] = (HBITMAP)LoadImage(NULL, L"res\\circled_1.bmp", IMAGE_BITMAP, menuItemSize, menuItemSize, LR_LOADFROMFILE | LR_SHARED);
	menuDigits[2] = (HBITMAP)LoadImage(NULL, L"res\\circled_2.bmp", IMAGE_BITMAP, menuItemSize, menuItemSize, LR_LOADFROMFILE | LR_SHARED);
	menuDigits[3] = (HBITMAP)LoadImage(NULL, L"res\\circled_3.bmp", IMAGE_BITMAP, menuItemSize, menuItemSize, LR_LOADFROMFILE | LR_SHARED);
	menuDigits[4] = (HBITMAP)LoadImage(NULL, L"res\\circled_4.bmp", IMAGE_BITMAP, menuItemSize, menuItemSize, LR_LOADFROMFILE | LR_SHARED);
	menuDigits[5] = (HBITMAP)LoadImage(NULL, L"res\\circled_5.bmp", IMAGE_BITMAP, menuItemSize, menuItemSize, LR_LOADFROMFILE | LR_SHARED);
	menuDigits[6] = (HBITMAP)LoadImage(NULL, L"res\\circled_6.bmp", IMAGE_BITMAP, menuItemSize, menuItemSize, LR_LOADFROMFILE | LR_SHARED);
	menuDigits[7] = (HBITMAP)LoadImage(NULL, L"res\\circled_7.bmp", IMAGE_BITMAP, menuItemSize, menuItemSize, LR_LOADFROMFILE | LR_SHARED);
	menuDigits[8] = (HBITMAP)LoadImage(NULL, L"res\\circled_8.bmp", IMAGE_BITMAP, menuItemSize, menuItemSize, LR_LOADFROMFILE | LR_SHARED);
	menuDigits[9] = (HBITMAP)LoadImage(NULL, L"res\\circled_9.bmp", IMAGE_BITMAP, menuItemSize, menuItemSize, LR_LOADFROMFILE | LR_SHARED);

	menuDigitsPressed.resize(10);
	menuDigitsPressed[0] = (HBITMAP)LoadImage(NULL, L"res\\circled_0_pressed.bmp", IMAGE_BITMAP, menuItemSize, menuItemSize, LR_LOADFROMFILE | LR_SHARED);
	menuDigitsPressed[1] = (HBITMAP)LoadImage(NULL, L"res\\circled_1_pressed.bmp", IMAGE_BITMAP, menuItemSize, menuItemSize, LR_LOADFROMFILE | LR_SHARED);
	menuDigitsPressed[2] = (HBITMAP)LoadImage(NULL, L"res\\circled_2_pressed.bmp", IMAGE_BITMAP, menuItemSize, menuItemSize, LR_LOADFROMFILE | LR_SHARED);
	menuDigitsPressed[3] = (HBITMAP)LoadImage(NULL, L"res\\circled_3_pressed.bmp", IMAGE_BITMAP, menuItemSize, menuItemSize, LR_LOADFROMFILE | LR_SHARED);
	menuDigitsPressed[4] = (HBITMAP)LoadImage(NULL, L"res\\circled_4_pressed.bmp", IMAGE_BITMAP, menuItemSize, menuItemSize, LR_LOADFROMFILE | LR_SHARED);
	menuDigitsPressed[5] = (HBITMAP)LoadImage(NULL, L"res\\circled_5_pressed.bmp", IMAGE_BITMAP, menuItemSize, menuItemSize, LR_LOADFROMFILE | LR_SHARED);
	menuDigitsPressed[6] = (HBITMAP)LoadImage(NULL, L"res\\circled_6_pressed.bmp", IMAGE_BITMAP, menuItemSize, menuItemSize, LR_LOADFROMFILE | LR_SHARED);
	menuDigitsPressed[7] = (HBITMAP)LoadImage(NULL, L"res\\circled_7_pressed.bmp", IMAGE_BITMAP, menuItemSize, menuItemSize, LR_LOADFROMFILE | LR_SHARED);
	menuDigitsPressed[8] = (HBITMAP)LoadImage(NULL, L"res\\circled_8_pressed.bmp", IMAGE_BITMAP, menuItemSize, menuItemSize, LR_LOADFROMFILE | LR_SHARED);
	menuDigitsPressed[9] = (HBITMAP)LoadImage(NULL, L"res\\circled_9_pressed.bmp", IMAGE_BITMAP, menuItemSize, menuItemSize, LR_LOADFROMFILE | LR_SHARED);

	menuButtonBMP = (HBITMAP)LoadImage(NULL, L"res\\menu_button.bmp", IMAGE_BITMAP, menuItemSize, menuItemSize, LR_LOADFROMFILE | LR_SHARED);
	menuButtonPressedBMP = (HBITMAP)LoadImage(NULL, L"res\\menu_button_pressed.bmp", IMAGE_BITMAP, menuItemSize, menuItemSize, LR_LOADFROMFILE | LR_SHARED);

	
}

void COverlappedWindow::drawGrid(HDC hdc) {

	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 9; ++j) {
			int x = i * cellSize;
			int y = j * cellSize;
			drawBitmap(hdc, x, y, digits[grid[i][j]], false);
		}
	}

	if (isDigitPressed) {
		drawBitmap(hdc, iPressed * cellSize, jPressed * cellSize, digitsPressed[grid[iPressed][jPressed]], false);
	}

	drawField(hdc);

}

void COverlappedWindow::drawField(HDC hdc) {

	long width, height;
	getClientRect(width, height);

	//thick lines
	HPEN pen = CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
	SelectObject(hdc, pen);

	for (int coord = 0; coord <= cellSize * 9; coord += cellSize * 3) {
		MoveToEx(hdc, coord, 0, NULL);
		LineTo(hdc, coord, winSize);
		MoveToEx(hdc, 0, coord, NULL);
		LineTo(hdc, winSize, coord);
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
		LineTo(hdc, coord, winSize);
		MoveToEx(hdc, 0, coord, NULL);
		LineTo(hdc, winSize, coord);
	}

	DeleteObject(pen);

}

void COverlappedWindow::drawMenu(HDC hdc) {

	HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
	SelectObject(hdc, brush);
	Rectangle(hdc, winSize, 0, winSize + menuWidth, winSize);
	DeleteObject(brush);

	drawBitmap(hdc, menuRects[10].left, menuRects[10].top, menuButtons[10], false);

	for (int i = 0; i <= 9; i++) {
		drawBitmap(hdc, menuRects[i].left, menuRects[i].top, menuButtons[i], false);
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
	case WM_LBUTTONDOWN: {
		window->On_LButtonDown(wParam, lParam);
		return 0;
	}
	case WM_LBUTTONUP: {
		window->On_LButtonUp(wParam, lParam);
		return 0;
	}
	default: {
		return DefWindowProc(handle, message, wParam, lParam);
	}
	}
	
}

INT_PTR CALLBACK COverlappedWindow::DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	static COverlappedWindow *window;

	switch (uMsg) {

	case WM_INITDIALOG:

		window = reinterpret_cast<COverlappedWindow *>(GetWindowLongPtr(GetParent(hwndDlg), GWLP_USERDATA));
		window->OnInitMenuDialog(hwndDlg);
		
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_BUTTON_START: {
			EndDialog(hwndDlg, START);
			return TRUE;
		}
		case IDC_BUTTON_RESTART:
			EndDialog(hwndDlg, RESTART);
			return TRUE;
		case IDC_BUTTON_DEV:
			EndDialog(hwndDlg, DEV);
			return TRUE;
		case IDC_BUTTON_CLOSE:
			EndDialog(hwndDlg, NONE);
			return TRUE;
			break;

		}
	case WM_CLOSE:
		EndDialog(hwndDlg, NONE);
		return TRUE;
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwndDlg, &ps);

		BITMAP bm;

		int x = 300;
		int y = 270;
		HBITMAP menuBackground = (HBITMAP)LoadImage(NULL, L"res\\menu_background.bmp", IMAGE_BITMAP, x, y, LR_LOADFROMFILE | LR_SHARED);

		HDC hdcMem = CreateCompatibleDC(hdc);
		HBITMAP hbmOld = reinterpret_cast<HBITMAP>(SelectObject(hdcMem, menuBackground));

		GetObject(menuBackground, sizeof(bm), &bm);

		RECT menuRect;
		GetWindowRect(hwndDlg, &menuRect);

		BitBlt(hdc, 0, 0, x, y, hdcMem, 0, 0, SRCCOPY);

		SelectObject(hdcMem, hbmOld);
		DeleteDC(hdcMem);

		EndPaint(hwndDlg, &ps);
	}

	}

	return FALSE;
}