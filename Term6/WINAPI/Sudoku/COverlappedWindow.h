#pragma once

#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <strsafe.h>
#include <vector>

#include "resource.h"

class COverlappedWindow {
public:
	COverlappedWindow();
	~COverlappedWindow();

	// Зарегистрировать класс окна
	static bool RegisterClass();

	// Создать экземпляр окна
	bool Create();

	// Показать окно
	void Show(int cmdShow);

protected:
	void OnNCCreate(HWND _handle);
	void OnCreate();
	void OnClose();
	void OnDestroy();
	void OnTimer();
	void OnPaint();

private:
	
	HWND handle;
	int xEllipseOld;
	int yEllipseOld;
	int xEllipse;
	int yEllipse;

	std::vector<HBITMAP> digits;
	std::vector<HBITMAP> circledDigits;

	
	int fieldSize;
	int cellSize = 75;
	int winSize = cellSize * 9;
	int menuWidth = cellSize * 1.5;

	static const DWORD windowExStyle = 0;
	static const DWORD windowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

	static const int Delta = 1;
	static const int Radius = 30;
	static const int TimerID = 10;

	void initDigits();
	void drawField(HDC hdc);
	void drawMenu(HDC hdc);
	void drawDigit(HDC hdc, int x, int y, int digit, bool redrawField = true);
	void drawBitmap(HDC hdc, int x, int y, HBITMAP bitmap, bool redrawField = true);
	void drawEllipse(HDC dc, int x, int y, COLORREF fillColor, COLORREF outlineColor);
	void getClientRect(long &width, long &height);

	void ErrorExit(LPTSTR lpszFunction);

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};
