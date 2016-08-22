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
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	return (::RegisterClassEx(&wcex) != 0);

}

bool COverlappedWindow::Create() {

	handle = CreateWindowEx(windowExStyle, L"OverlappedWindow", L"Sudoku", windowStyle,
		CW_USEDEFAULT, CW_USEDEFAULT, win_size_, win_size_, 0, 0, GetModuleHandle(0), this);

	SetWindowLongPtr(handle, GWL_USERDATA, reinterpret_cast<LONG>(this));

	return (handle != NULL);

}

void COverlappedWindow::Show(int cmdShow) {

	// The parameters to ShowWindow explained:
	// hWnd: the value returned from CreateWindow
	// nCmdShow: the fourth parameter from WinMain

	RECT window_rect;
	GetWindowRect(handle, &window_rect);
	int window_width = window_rect.right - window_rect.left;
	int window_height = window_rect.bottom- window_rect.top;

	int screen_width = GetSystemMetrics(SM_CXSCREEN);
	int screen_height = GetSystemMetrics(SM_CYSCREEN);

	SetWindowPos(handle, HWND_TOP, (screen_width - window_width) / 4, (screen_height - window_height) / 2, window_width, window_height, SWP_SHOWWINDOW);

	//ShowWindow(handle, cmdShow);

	//UpdateWindow(handle);

}

void COverlappedWindow::OnNCCreate(HWND _handle) {
	handle = _handle;
}

//window messages
void COverlappedWindow::OnCreate() {

	RECT clientRect = { 0, 0, cell_size_ * 9 + menu_width_, cell_size_ * 9 };
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

	PostQuitMessage(0);

}

void COverlappedWindow::OnTimer() {
	
	long width;
	long height;
	getClientRect(width, height);
	InvalidateRect(handle, NULL, TRUE);
}

void COverlappedWindow::OnPaint() {
	
	static RECT rcClient; // client area rectangle
	GetClientRect(handle, &rcClient);
	long width = rcClient.right;
	long height = rcClient.bottom;


	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(handle, &ps);
	HDC memHdc = CreateCompatibleDC(hdc);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, width, height);
	SelectObject(memHdc, hBitmap);

	drawMenu(memHdc);
	drawGrid(memHdc);

	BitBlt(hdc, 0, 0, width, height, memHdc, 0, 0, SRCCOPY);
	DeleteObject(hBitmap);
	DeleteObject(memHdc);

	EndPaint(handle, &ps);

}

void COverlappedWindow::On_LButtonDown(WPARAM wParam, LPARAM lParam) {

	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);

	if (wParam == MK_LBUTTON) {
		for (int i = 0; i <= 10; ++i) {
			if (x >= menu_rects_[i].left && x <= menu_rects_[i].right &&
				y >= menu_rects_[i].top && y <= menu_rects_[i].bottom) {
				if (i == 10) {
					menu_buttons_[10] = menu_button_pressed_BMP_;
				} else {
					menu_buttons_[i] = menu_digits_pressed_[i];
				}
				updateRect(menu_rects_[i]);
			}

		}


		for (int i = 0; i < 9; ++i) {
			for (int j = 0; j < 9; ++j) {
				if (x >= i * cell_size_ && x <= (i + 1) * cell_size_ &&
					y >= j * cell_size_ && y <= (j + 1) * cell_size_) {

					if (is_digit_pressed_) {
						updateRect(field_rects_[i_pressed_][j_pressed_]);
					}

					is_digit_pressed_ = true;
					i_pressed_ = i;
					j_pressed_ = j;
					updateRect(field_rects_[i][j]);
				}
			}
		}

	}

	//InvalidateRect(handle, NULL, TRUE);
}

void COverlappedWindow::On_LButtonUp(WPARAM wParam, LPARAM lParam) {

	if (menu_buttons_[10] == menu_button_pressed_BMP_) {
		int action = DialogBox(GetModuleHandle(0), MAKEINTRESOURCE(IDD_MENU_DIALOG), handle, reinterpret_cast<DLGPROC>(DialogProc));
		switch (action) {
		case START:
			sudoku.getRandomGrid(grid);
			InvalidateRect(handle, NULL, TRUE);
			break;
		case RESTART:
			sudoku.getPreviousGrid(grid);
			InvalidateRect(handle, NULL, TRUE);
			break;
		case DEV:
			MessageBox(handle, L"The game is created by Serg Popov", L"Developers", MB_OK);
			break;
		default:
			break;
		}

		menu_buttons_[10] = menu_button_BMP_;
		updateRect(menu_rects_[10]);
	}


	for (int i = 0; i <= 9; ++i) {
		if (menu_buttons_[i] == menu_digits_pressed_[i] && is_digit_pressed_) {

			//is_digit_pressed_ = false;
			grid[i_pressed_][j_pressed_] = i;
		}

		menu_buttons_[i] = menu_digits_[i];
		updateRect(menu_rects_[i]);
		updateRect(field_rects_[i_pressed_][j_pressed_]);
	}

}

void COverlappedWindow::On_WM_CHAR(WPARAM wParam, LPARAM lParam) {

	if (is_digit_pressed_) {
		if (wParam >= '0' && wParam <= '9') {
			grid[i_pressed_][j_pressed_] = wParam - '0';
		}
	}

	updateRect(field_rects_[i_pressed_][j_pressed_]);
}

void COverlappedWindow::On_WM_KEYDOWN(WPARAM wParam, LPARAM lParam) {

	int i_pressed_prev = i_pressed_;
	int j_pressed_prev = j_pressed_;



	if (is_digit_pressed_) {
		if (wParam == VK_RIGHT) {
			i_pressed_ = (i_pressed_ + 1) % 9;
		} else if (wParam == VK_DOWN) {
			j_pressed_ = (j_pressed_ + 1) % 9;
		} else if (wParam == VK_LEFT) {
			i_pressed_ = (i_pressed_ - 1 + 9) % 9;
		} else if (wParam == VK_UP) {
			j_pressed_ = (j_pressed_ - 1 + 9) % 9;
		} else if (wParam == VK_DELETE || wParam == VK_BACK) {
			grid[i_pressed_][j_pressed_] = 0;
		} 

		updateRect(field_rects_[i_pressed_prev][j_pressed_prev]);

	} else if (wParam == VK_RIGHT || wParam == VK_DOWN || wParam == VK_LEFT || wParam == VK_UP) {
		i_pressed_ = 0;
		j_pressed_ = 0;
		is_digit_pressed_ = true;
	}

	updateRect(field_rects_[i_pressed_][j_pressed_]);

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

	is_digit_pressed_ = false;
	i_pressed_ = 0;
	j_pressed_ = 0;

	//menu buttons init
	menu_buttons_.resize(11);
	menu_buttons_[10] = menu_button_BMP_;
	for (int i = 0; i <= 9; ++i) {
		menu_buttons_[i] = menu_digits_[i];
	}

	//menu rectangles init
	menu_rects_.resize(11);

	RECT rect = {
		win_size_ + (menu_width_ / 2 - menu_item_size_ / 2),
		(win_size_ - menu_margin_bottom_ - menu_margin_top_ - 10 * menu_item_size_) / 2 - menu_item_size_ / 2,
		win_size_ + (menu_width_ / 2 - menu_item_size_ / 2) + menu_item_size_,
		(win_size_ - menu_margin_bottom_ - menu_margin_top_ - 10 * menu_item_size_) / 2 + menu_item_size_ / 2 };

	menu_rects_[10] = rect;

	for (int i = 0; i <= 9; ++i) {
		RECT rect = {
			win_size_ + (menu_width_ / 2 - menu_item_size_ / 2),
			win_size_ - menu_margin_bottom_ - 10 * menu_item_size_ + i * menu_item_size_,
			win_size_ + (menu_width_ / 2 - menu_item_size_ / 2) + menu_item_size_,
			win_size_ - menu_margin_bottom_ - 10 * menu_item_size_ + i * menu_item_size_ + menu_item_size_ };

		menu_rects_[i] = rect;
	}


	field_rects_ = std::vector<std::vector<RECT> >(9, std::vector<RECT>(9, RECT()));

	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 9; ++j) {
			field_rects_[i][j].top = j * cell_size_;
			field_rects_[i][j].bottom = (j + 1) * cell_size_;
			field_rects_[i][j].left = i * cell_size_;
			field_rects_[i][j].right = (i + 1) * cell_size_;
		}
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
	digits[0] = (HBITMAP)LoadImage(NULL, L"res\\0.bmp", IMAGE_BITMAP, cell_size_, cell_size_, LR_LOADFROMFILE | LR_SHARED);
	digits[1] = (HBITMAP)LoadImage(NULL, L"res\\1.bmp", IMAGE_BITMAP, cell_size_, cell_size_, LR_LOADFROMFILE | LR_SHARED);
	digits[2] = (HBITMAP)LoadImage(NULL, L"res\\2.bmp", IMAGE_BITMAP, cell_size_, cell_size_, LR_LOADFROMFILE | LR_SHARED);
	digits[3] = (HBITMAP)LoadImage(NULL, L"res\\3.bmp", IMAGE_BITMAP, cell_size_, cell_size_, LR_LOADFROMFILE | LR_SHARED);
	digits[4] = (HBITMAP)LoadImage(NULL, L"res\\4.bmp", IMAGE_BITMAP, cell_size_, cell_size_, LR_LOADFROMFILE | LR_SHARED);
	digits[5] = (HBITMAP)LoadImage(NULL, L"res\\5.bmp", IMAGE_BITMAP, cell_size_, cell_size_, LR_LOADFROMFILE | LR_SHARED);
	digits[6] = (HBITMAP)LoadImage(NULL, L"res\\6.bmp", IMAGE_BITMAP, cell_size_, cell_size_, LR_LOADFROMFILE | LR_SHARED);
	digits[7] = (HBITMAP)LoadImage(NULL, L"res\\7.bmp", IMAGE_BITMAP, cell_size_, cell_size_, LR_LOADFROMFILE | LR_SHARED);
	digits[8] = (HBITMAP)LoadImage(NULL, L"res\\8.bmp", IMAGE_BITMAP, cell_size_, cell_size_, LR_LOADFROMFILE | LR_SHARED);
	digits[9] = (HBITMAP)LoadImage(NULL, L"res\\9.bmp", IMAGE_BITMAP, cell_size_, cell_size_, LR_LOADFROMFILE | LR_SHARED);

	digits_pressed_.resize(10);
	digits_pressed_[0] = (HBITMAP)LoadImage(NULL, L"res\\0_pressed.bmp", IMAGE_BITMAP, cell_size_, cell_size_, LR_LOADFROMFILE | LR_SHARED);
	digits_pressed_[1] = (HBITMAP)LoadImage(NULL, L"res\\1_pressed.bmp", IMAGE_BITMAP, cell_size_, cell_size_, LR_LOADFROMFILE | LR_SHARED);
	digits_pressed_[2] = (HBITMAP)LoadImage(NULL, L"res\\2_pressed.bmp", IMAGE_BITMAP, cell_size_, cell_size_, LR_LOADFROMFILE | LR_SHARED);
	digits_pressed_[3] = (HBITMAP)LoadImage(NULL, L"res\\3_pressed.bmp", IMAGE_BITMAP, cell_size_, cell_size_, LR_LOADFROMFILE | LR_SHARED);
	digits_pressed_[4] = (HBITMAP)LoadImage(NULL, L"res\\4_pressed.bmp", IMAGE_BITMAP, cell_size_, cell_size_, LR_LOADFROMFILE | LR_SHARED);
	digits_pressed_[5] = (HBITMAP)LoadImage(NULL, L"res\\5_pressed.bmp", IMAGE_BITMAP, cell_size_, cell_size_, LR_LOADFROMFILE | LR_SHARED);
	digits_pressed_[6] = (HBITMAP)LoadImage(NULL, L"res\\6_pressed.bmp", IMAGE_BITMAP, cell_size_, cell_size_, LR_LOADFROMFILE | LR_SHARED);
	digits_pressed_[7] = (HBITMAP)LoadImage(NULL, L"res\\7_pressed.bmp", IMAGE_BITMAP, cell_size_, cell_size_, LR_LOADFROMFILE | LR_SHARED);
	digits_pressed_[8] = (HBITMAP)LoadImage(NULL, L"res\\8_pressed.bmp", IMAGE_BITMAP, cell_size_, cell_size_, LR_LOADFROMFILE | LR_SHARED);
	digits_pressed_[9] = (HBITMAP)LoadImage(NULL, L"res\\9_pressed.bmp", IMAGE_BITMAP, cell_size_, cell_size_, LR_LOADFROMFILE | LR_SHARED);

	menu_digits_.resize(10);
	menu_digits_[0] = (HBITMAP)LoadImage(NULL, L"res\\circled_0.bmp", IMAGE_BITMAP, menu_item_size_, menu_item_size_, LR_LOADFROMFILE | LR_SHARED);
	menu_digits_[1] = (HBITMAP)LoadImage(NULL, L"res\\circled_1.bmp", IMAGE_BITMAP, menu_item_size_, menu_item_size_, LR_LOADFROMFILE | LR_SHARED);
	menu_digits_[2] = (HBITMAP)LoadImage(NULL, L"res\\circled_2.bmp", IMAGE_BITMAP, menu_item_size_, menu_item_size_, LR_LOADFROMFILE | LR_SHARED);
	menu_digits_[3] = (HBITMAP)LoadImage(NULL, L"res\\circled_3.bmp", IMAGE_BITMAP, menu_item_size_, menu_item_size_, LR_LOADFROMFILE | LR_SHARED);
	menu_digits_[4] = (HBITMAP)LoadImage(NULL, L"res\\circled_4.bmp", IMAGE_BITMAP, menu_item_size_, menu_item_size_, LR_LOADFROMFILE | LR_SHARED);
	menu_digits_[5] = (HBITMAP)LoadImage(NULL, L"res\\circled_5.bmp", IMAGE_BITMAP, menu_item_size_, menu_item_size_, LR_LOADFROMFILE | LR_SHARED);
	menu_digits_[6] = (HBITMAP)LoadImage(NULL, L"res\\circled_6.bmp", IMAGE_BITMAP, menu_item_size_, menu_item_size_, LR_LOADFROMFILE | LR_SHARED);
	menu_digits_[7] = (HBITMAP)LoadImage(NULL, L"res\\circled_7.bmp", IMAGE_BITMAP, menu_item_size_, menu_item_size_, LR_LOADFROMFILE | LR_SHARED);
	menu_digits_[8] = (HBITMAP)LoadImage(NULL, L"res\\circled_8.bmp", IMAGE_BITMAP, menu_item_size_, menu_item_size_, LR_LOADFROMFILE | LR_SHARED);
	menu_digits_[9] = (HBITMAP)LoadImage(NULL, L"res\\circled_9.bmp", IMAGE_BITMAP, menu_item_size_, menu_item_size_, LR_LOADFROMFILE | LR_SHARED);

	menu_digits_pressed_.resize(10);
	menu_digits_pressed_[0] = (HBITMAP)LoadImage(NULL, L"res\\circled_0_pressed.bmp", IMAGE_BITMAP, menu_item_size_, menu_item_size_, LR_LOADFROMFILE | LR_SHARED);
	menu_digits_pressed_[1] = (HBITMAP)LoadImage(NULL, L"res\\circled_1_pressed.bmp", IMAGE_BITMAP, menu_item_size_, menu_item_size_, LR_LOADFROMFILE | LR_SHARED);
	menu_digits_pressed_[2] = (HBITMAP)LoadImage(NULL, L"res\\circled_2_pressed.bmp", IMAGE_BITMAP, menu_item_size_, menu_item_size_, LR_LOADFROMFILE | LR_SHARED);
	menu_digits_pressed_[3] = (HBITMAP)LoadImage(NULL, L"res\\circled_3_pressed.bmp", IMAGE_BITMAP, menu_item_size_, menu_item_size_, LR_LOADFROMFILE | LR_SHARED);
	menu_digits_pressed_[4] = (HBITMAP)LoadImage(NULL, L"res\\circled_4_pressed.bmp", IMAGE_BITMAP, menu_item_size_, menu_item_size_, LR_LOADFROMFILE | LR_SHARED);
	menu_digits_pressed_[5] = (HBITMAP)LoadImage(NULL, L"res\\circled_5_pressed.bmp", IMAGE_BITMAP, menu_item_size_, menu_item_size_, LR_LOADFROMFILE | LR_SHARED);
	menu_digits_pressed_[6] = (HBITMAP)LoadImage(NULL, L"res\\circled_6_pressed.bmp", IMAGE_BITMAP, menu_item_size_, menu_item_size_, LR_LOADFROMFILE | LR_SHARED);
	menu_digits_pressed_[7] = (HBITMAP)LoadImage(NULL, L"res\\circled_7_pressed.bmp", IMAGE_BITMAP, menu_item_size_, menu_item_size_, LR_LOADFROMFILE | LR_SHARED);
	menu_digits_pressed_[8] = (HBITMAP)LoadImage(NULL, L"res\\circled_8_pressed.bmp", IMAGE_BITMAP, menu_item_size_, menu_item_size_, LR_LOADFROMFILE | LR_SHARED);
	menu_digits_pressed_[9] = (HBITMAP)LoadImage(NULL, L"res\\circled_9_pressed.bmp", IMAGE_BITMAP, menu_item_size_, menu_item_size_, LR_LOADFROMFILE | LR_SHARED);

	menu_button_BMP_ = (HBITMAP)LoadImage(NULL, L"res\\menu_button.bmp", IMAGE_BITMAP, menu_item_size_, menu_item_size_, LR_LOADFROMFILE | LR_SHARED);
	menu_button_pressed_BMP_ = (HBITMAP)LoadImage(NULL, L"res\\menu_button_pressed.bmp", IMAGE_BITMAP, menu_item_size_, menu_item_size_, LR_LOADFROMFILE | LR_SHARED);

	
}

void COverlappedWindow::drawGrid(HDC hdc) {

	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 9; ++j) {
			int x = i * cell_size_;
			int y = j * cell_size_;
			drawBitmap(hdc, x, y, digits[grid[i][j]], false);
		}
	}

	if (is_digit_pressed_) {
		drawBitmap(hdc, i_pressed_ * cell_size_, j_pressed_ * cell_size_, digits_pressed_[grid[i_pressed_][j_pressed_]], false);
	}

	drawField(hdc);

}

void COverlappedWindow::drawField(HDC hdc) {

	long width, height;
	getClientRect(width, height);

	//thick lines
	HPEN pen = CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
	SelectObject(hdc, pen);

	for (int coord = 0; coord <= cell_size_ * 9; coord += cell_size_ * 3) {
		MoveToEx(hdc, coord, 0, NULL);
		LineTo(hdc, coord, win_size_);
		MoveToEx(hdc, 0, coord, NULL);
		LineTo(hdc, win_size_, coord);
	}

	DeleteObject(pen);

	//thin lines
	pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	SelectObject(hdc, pen);

	for (int coord = 0; coord <= cell_size_ * 9; coord += cell_size_) {
		if (coord % (cell_size_ * 3) == 0) {
			continue;
		}
		MoveToEx(hdc, coord, 0, NULL);
		LineTo(hdc, coord, win_size_);
		MoveToEx(hdc, 0, coord, NULL);
		LineTo(hdc, win_size_, coord);
	}

	DeleteObject(pen);

}

void COverlappedWindow::drawMenu(HDC hdc) {

	HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
	SelectObject(hdc, brush);
	Rectangle(hdc, win_size_, 0, win_size_ + menu_width_, win_size_);
	DeleteObject(brush);

	drawBitmap(hdc, menu_rects_[10].left, menu_rects_[10].top, menu_buttons_[10], false);

	for (int i = 0; i <= 9; i++) {
		drawBitmap(hdc, menu_rects_[i].left, menu_rects_[i].top, menu_buttons_[i], false);
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

void COverlappedWindow::updateRect(RECT &rect) {

	InvalidateRect(handle, &rect, TRUE);

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
	case WM_CHAR: {
		window->On_WM_CHAR(wParam, lParam);
		return 0;
	}
	case WM_KEYDOWN: {
		window->On_WM_KEYDOWN(wParam, lParam);
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
		/*
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

		EndPaint(hwndDlg, &ps);*/
	}

	}

	return FALSE;
}