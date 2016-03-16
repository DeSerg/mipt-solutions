#pragma once

#include "CEllipseWindow.h"
#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>

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
	void OnSize();
	void OnTimer();
	void OnPaint();

private:
	
	HWND handle;
	int xEllipseOld;
	int yEllipseOld;
	int xEllipse;
	int yEllipse;

	std::array<CEllipseWindow, 4> childWindows;

	static const int Delta = 1;
	static const int Radius = 30;
	static const int TimerID = 10;

	void getClientRect(long &width, long &height);

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};
