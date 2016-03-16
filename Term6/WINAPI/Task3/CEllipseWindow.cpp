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

	HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
	wcex.hbrBackground = brush;
	DeleteObject(brush);

	return (::RegisterClassEx(&wcex) != 0);

}

bool CEllipseWindow::Create(int x, int y, int width, int height, HWND parentHandle) {

	CreateWindowEx(0, L"EllipseWindow", L"ChildWindow", WS_CHILD | WS_BORDER,
		x, y, width, height, parentHandle, 0, GetModuleHandle(0), this);
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

	//ellipse position setting
	long width;
	long height;
	getClientRect(width, height);
	xEllipseOld = xEllipse = (rand() % 100) * width / 100;
	yEllipseOld = yEllipse = (rand() % 100) * height / 100;
	yEllipseOld = yEllipse = 0;
	
	//direction setting
	direction = rand() % 4;
	int dx = rand() % 10 + 3;
	int dy = rand() % 10 + 3;
	XDelta[0] = XDelta[1] = dx;
	XDelta[2] = XDelta[3] = -dx;
	YDelta[0] = YDelta[3] = dy;
	YDelta[1] = YDelta[2] = -dy;

	//timer setting
	TimeDelta = rand() % 5 + 3;
	TimerID = 1;
	SetTimer(handle, TimerID, TimeDelta, 0);
}

void CEllipseWindow::OnDestroy() {

	KillTimer(handle, TimerID);
	PostQuitMessage(0);

}

void CEllipseWindow::OnTimer() {
	
	chooseDirection();

	xEllipseOld = xEllipse;
	yEllipseOld = yEllipse;
	xEllipse = xEllipse + XDelta[direction];
	yEllipse = yEllipse + YDelta[direction];

	InvalidateRect(handle, NULL, TRUE);
}

void CEllipseWindow::OnPaint() {
	
	static RECT rcClient; // client area rectangle
	GetClientRect(handle, &rcClient);
	long width = rcClient.right;
	long height = rcClient.bottom;

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(handle, &ps);
	/*HDC memHdc = CreateCompatibleDC(hdc);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, width, height);
	SelectObject(memHdc, hBitmap);*/

	drawEllipse(hdc, xEllipseOld, yEllipseOld, Radius, RGB(255, 255, 255), RGB(255, 255, 255));
	//FillRect(hdc, &rcClient, reinterpret_cast<HBRUSH>(GetStockObject(GRAY_BRUSH)));

	COLORREF fillColor, outlineColor;
	if (GetFocus() == handle) {
		fillColor = RGB(255, 25, 0);
		outlineColor = RGB(0, 0, 0);
	} else {
		fillColor = RGB(0, 0, 0);
		outlineColor = RGB(255, 25, 0);
	}

	drawEllipse(hdc, xEllipse, yEllipse, Radius, fillColor, outlineColor);
	
	/*BitBlt(hdc, 0, 0, width, height, memHdc, 0, 0, SRCCOPY);
	DeleteObject(hBitmap);
	DeleteObject(memHdc);*/
	
	EndPaint(handle, &ps);
}

void CEllipseWindow::OnLButtonDown() {
	if (GetFocus() == handle) {
		SetFocus(GetParent(handle));
	} else {
		SetFocus(handle);
	}
}

void CEllipseWindow::drawEllipse(HDC dc, int x, int y, int radius, COLORREF fillColor, COLORREF outlineColor) {
	
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

void CEllipseWindow::chooseDirection() {

	long width;
	long height;
	getClientRect(width, height);

	int left = 0;
	int right = width;
	int top = 0;
	int bottom = height;

	bool leftClose = (xEllipse - Radius) < epsilon;
	bool rightClose = (xEllipse + Radius) > (right - epsilon);
	bool topClose = (yEllipse - Radius) < epsilon;
	bool bottomClose = (yEllipse + Radius) > (bottom - epsilon);

	if (topClose) {
		if (leftClose) {
			direction = 0;
		} else if (rightClose) {
			direction = 3;
		} else if (direction == 2) {
			direction = 3;
		} else if (direction == 1) {
			direction = 0;
		}
	}

	if (bottomClose) {
		if (leftClose) {
			direction = 1;
		} else if (rightClose) {
			direction = 2;
		} else if (direction == 3) {
			direction = 2;
		} else if (direction == 0) {
			direction = 1;
		}
	}

	if (rightClose) {
		if (topClose) {
			direction = 3;
		} else if (bottomClose) {
			direction = 2;
		} else if (direction == 1) {
			direction = 2;
		} else if (direction == 0) {
			direction = 3;
		}
	}

	if (leftClose) {
		if (topClose) {
			direction = 0;
		} else if (bottomClose) {
			direction = 1;
		} else if (direction == 2) {
			direction = 1;
		} else if (direction == 3) {
			direction = 0;
		}
	}
}

LRESULT __stdcall CEllipseWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message)
	{
	case WM_NCCREATE: {
		CEllipseWindow *window = reinterpret_cast<CEllipseWindow*>((reinterpret_cast<CREATESTRUCT*>(lParam))->lpCreateParams);
		SetLastError(0);
        SetWindowLongPtr(handle, GWLP_USERDATA, reinterpret_cast<LONG>(window));
        if( GetLastError() != 0 ) {
            return GetLastError();
        }
		window->OnNCCreate(handle);
		return DefWindowProc(handle, message, wParam, lParam);
	}
	case WM_CREATE: {
		CEllipseWindow* window = reinterpret_cast<CEllipseWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));
		window->OnCreate();
		return 0;
	}
	case WM_DESTROY: {
		CEllipseWindow* window = reinterpret_cast<CEllipseWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));
		window->OnDestroy();
		return 0;
	}
	case WM_TIMER: {
		CEllipseWindow* window = reinterpret_cast<CEllipseWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));
		window->OnTimer();
		return 0;
	}
	case WM_PAINT: {
		CEllipseWindow* window = reinterpret_cast<CEllipseWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));
		window->OnPaint();
		return DefWindowProc(handle, message, wParam, lParam);
	}
	case WM_LBUTTONDOWN:
        {
            CEllipseWindow* window = reinterpret_cast<CEllipseWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));
            window->OnLButtonDown();
            return 0;
        }
	default: {
		return DefWindowProc(handle, message, wParam, lParam);
	}
	}
	
}

