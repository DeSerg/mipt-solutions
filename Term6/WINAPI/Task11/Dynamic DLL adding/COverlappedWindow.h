#pragma once


#include "Settings.h"
#include "Common.h"

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
	HWND getSettingsDialogHandle();

protected:
	void OnNCCreate(HWND _handle);
	void OnCreate();
	void OnClose();
	void OnDestroy();
	void OnSize(WPARAM wParam, LPARAM lParam);
	int OnCommand(WPARAM wParam);
	int OnSysCommand(WPARAM wParam);
	void OnTimer();
	void OnPaint();
	void OnSetCursor();
	HBRUSH OnCtlColorEdit(HDC dc);


	void OnInitDialog(HWND dialogHandle);
	void OnHScroll();
	void OnSettingsBackground(HWND dialogHandle);
	void OnSettingsFont(HWND dialogHandle);
	void OnSettingsOK(HWND dialogHandle);
	void OnSettingsCansel(HWND dialogHandle);
	void OnSettingsPreview(HWND dialogHandle);


private:
	
	HWND handle;
	HWND hwndEdit;
	HWND hwndSettingsDialog;
	bool textChanged;

	bool dialogOpen;
	bool preview;
	Settings settingsOld;
	Settings settingsNew;

	static const int ID_EDITCHILD = 10;
	static const int MAX_STRING_LEN = 200;

	void getClientRect(long &width, long &height);
	void setChildrenPosition();
	void saveText();
	bool writeToFile(LPCTSTR fileName, TCHAR *text, DWORD textLen);
	void countWords();
	
	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

};
