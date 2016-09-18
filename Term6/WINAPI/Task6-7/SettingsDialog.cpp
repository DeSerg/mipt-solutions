#include "SettingsDialog.h"

SettingsDialog::SettingsDialog(HWND parentHandle) :
	parentHandle(parentHandle)
{

	DialogBox(GetModuleHandle(0), MAKEINTRESOURCE(IDD_NOTEPAD_SETTINGS), parentHandle, reinterpret_cast<DLGPROC>(DialogProc));

}

INT_PTR CALLBACK SettingsDialog::DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	switch (uMsg) {

	case WM_INITDIALOG:

		SendDlgItemMessage(hwndDlg, IDC_FONT_SLIDER, TBM_SETRANGE, (WPARAM)1, (LPARAM)MAKELONG(8, 72));
		SendDlgItemMessage(hwndDlg, IDC_TRANSPARENCY_SLIDER, TBM_SETRANGE, (WPARAM)1, (LPARAM)MAKELONG(0, 100));

		return TRUE;


	case WM_COMMAND:
		switch (wParam) {
		case IDC_BACKGROUND:
			return TRUE;
		case IDC_FONT:
			return TRUE;
		case IDOK:

			EndDialog(hwndDlg, 0);
			return TRUE;
		case IDCANCEL:
			EndDialog(hwndDlg, 0);
			return TRUE;
		}
		break;

	case WM_HSCROLL:
		int positionFont = SendDlgItemMessage(hwndDlg, IDC_FONT_SLIDER, TBM_GETPOS, 0, 0);
		int positionTransparency = SendDlgItemMessage(hwndDlg, IDC_TRANSPARENCY_SLIDER, TBM_GETPOS, 0, 0);
		return TRUE;

	}

	return FALSE;
}
