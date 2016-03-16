#pragma once

#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <iostream>
#include <string>
#include <tchar.h>
#include <array>

class CEllipseWindow {
public:
	CEllipseWindow();
	~CEllipseWindow();

	// Зарегистрировать класс окна
	static bool RegisterClass();

	// Создать экземпляр окна
	bool Create(HWND parentHandle);

	// Показать окно
	void Show(int cmdShow);
	HWND getHandle();

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

	static const int DistanceDelta = 1;
	static const int TimeDelta = 20;
	static const int Radius = 30;
	static const int TimerID = 10;

	void drawEllipse(HDC dc, int x, int y, COLORREF fillColor, COLORREF outlineColor);
	void getClientRect(long &width, long &height);

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};
