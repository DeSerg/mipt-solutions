#pragma once

#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include "windef.h"
#include "shlwapi.h"
#include <Winuser.h>
#include <Winbase.h>
#include <iostream>
#include <Richedit.h>
#include <windef.h>

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
	HWND getHandle();

protected:
	void OnNCCreate(HWND _handle);
	void OnCreate();
	void OnClose();
	void OnDestroy();
	void OnSize(WPARAM wParam, LPARAM lParam);
	int OnCommand(WPARAM wParam);
	void OnTimer();
	void OnPaint();
	void OnSetCursor();

private:
	
	HWND handle;
	HWND hwndEdit;
	bool textChanged;

	static const int ID_EDITCHILD = 10;

	void getClientRect(long &width, long &height);
	void setChildrenPosition();

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};
