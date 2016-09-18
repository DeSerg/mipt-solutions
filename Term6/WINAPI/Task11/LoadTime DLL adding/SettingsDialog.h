#include "Common.h"

class SettingsDialog {

public:

	SettingsDialog(HWND parentHandle);

private:

	HWND parentHandle;

	static INT_PTR CALLBACK SettingsDialog::DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

};