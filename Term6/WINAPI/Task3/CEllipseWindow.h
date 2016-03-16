#pragma once

#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <iostream>
#include <string>
#include <tchar.h>
#include <array>
#include <cmath>

class CEllipseWindow {
public:
	CEllipseWindow();
	~CEllipseWindow();

	// Зарегистрировать класс окна
	static bool RegisterClass();

	// Создать экземпляр окна
	bool Create(int x, int y, int width, int height, HWND parentHandle);

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
	void OnLButtonDown();

private:
	
	HWND handle;
	int xEllipseOld;
	int yEllipseOld;
	int xEllipse;
	int yEllipse;
	int TimerID;
	int XDelta[4];
	int YDelta[4];
	int TimeDelta;
	int direction;

	static const int Radius = 30;
	static const int epsilon = 1;
	

	void drawEllipse(HDC dc, int x, int y, int radius, COLORREF fillColor, COLORREF outlineColor);
	void getClientRect(long &width, long &height);
	void chooseDirection();

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};
