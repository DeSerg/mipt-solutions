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

	std::array<CEllipseWindow, 4> childWindows;

	void getClientRect(long &width, long &height);
	void setChildrenPosition();

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};
