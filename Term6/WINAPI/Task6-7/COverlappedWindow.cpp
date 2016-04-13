#include "COverlappedWindow.h"

COverlappedWindow::COverlappedWindow() {}

COverlappedWindow::~COverlappedWindow() {}

bool COverlappedWindow::RegisterClass() {

	WNDCLASSEX wcex;
	::ZeroMemory(&wcex, sizeof(wcex));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.lpfnWndProc = COverlappedWindow::windowProc;
	wcex.hInstance = GetModuleHandle(0);
	wcex.lpszClassName = L"OverlappedWindow";
	wcex.hCursor = LoadCursor(GetModuleHandle(0), MAKEINTRESOURCE(230));
	wcex.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wcex.hIcon = reinterpret_cast<HICON>(LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(APP_ICON), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR));	
	wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	//wcex.hIcon = LoadIcon(GetModuleHandle(0), APP_ICON);
	return (::RegisterClassEx(&wcex) != 0);

}

bool COverlappedWindow::Create() {

	TCHAR sz[MAX_STRING_LEN];
	LoadString(GetModuleHandle(0), IDS_WND_CAPTION, sz, sizeof(sz) / sizeof(TCHAR));

	CreateWindowEx(0, L"OverlappedWindow", sz, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, GetModuleHandle(0), this);
	return (handle != NULL);

}

void COverlappedWindow::Show(int cmdShow) {

	ShowWindow(handle, cmdShow);
	UpdateWindow(handle);
}

HWND COverlappedWindow::getHandle() {
	return handle;
}

void COverlappedWindow::OnNCCreate(HWND _handle) {
	handle = _handle;
}

void COverlappedWindow::OnCreate() {

	textChanged = false;
	dialogOpen = false;

	hwndEdit = CreateWindowEx(
                    0, L"EDIT",
                    NULL,
                    WS_CHILD | WS_VISIBLE | WS_VSCROLL | 
                    ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL, 
                    0, 0, 0, 0,
                    handle,
                    (HMENU) ID_EDITCHILD,
                    (HINSTANCE) GetWindowLong(handle, GWL_HINSTANCE), 
                    NULL);


	HMODULE module = GetModuleHandle(0);
	HRSRC defTextRec = ::FindResource(module, MAKEINTRESOURCE(IDR_RC_TEXT1), L"RC_TEXT");
	HGLOBAL defTextData = ::LoadResource(module, defTextRec);
	wchar_t *data = reinterpret_cast<wchar_t *>(LockResource(defTextData));

	SetWindowLong(handle, GWL_EXSTYLE, GetWindowLong(handle, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(handle, 0, (255 * 100) / 100, LWA_ALPHA);

	HFONT baseFont = CreateFont(20, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0, L"Arial");
	SendMessage(hwndEdit, WM_SETFONT, reinterpret_cast<WPARAM>(baseFont), true);
	SetWindowText(hwndEdit, data);

	COLORREF backColor = RGB(255, 255, 255);
	COLORREF fontColor = RGB(0, 0, 0);

	settingsNew = Settings(hwndEdit, handle, backColor, fontColor);
	settingsOld = Settings(hwndEdit, handle, backColor, fontColor);

}

void COverlappedWindow::OnClose() {
	if (textChanged) {
		int msBoxBtn = MessageBox(handle, L"Would you like save your changes?", L"My Window", MB_YESNOCANCEL);
		switch (msBoxBtn)
		{
		case IDYES:
			saveText();
			break;
		}
	}
}

void COverlappedWindow::OnDestroy() {
	PostQuitMessage(0);
}

void COverlappedWindow::OnSize(WPARAM wParam, LPARAM lParam) {
	MoveWindow(hwndEdit, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
}

int COverlappedWindow::OnCommand(WPARAM wParam) {

	if (HIWORD(wParam) == EN_CHANGE) {
		textChanged = true;
		return 0;
	}

	switch (LOWORD(wParam)) { 
	case ID_FILE_SAVE:
		saveText();
		break;
	case ID_FILE_EXIT:
		SendMessage(handle, WM_CLOSE, 0, 0);
		break;
	case ID_VIEW_SETTINGS:
		hwndSettingsDialog = CreateDialog(GetModuleHandle(0), 
			MAKEINTRESOURCE(IDD_NOTEPAD_SETTINGS), handle, reinterpret_cast<DLGPROC>(DialogProc));
		ShowWindow(hwndSettingsDialog, SW_SHOW);
		//DialogBox(GetModuleHandle(0), MAKEINTRESOURCE(IDD_NOTEPAD_SETTINGS), handle, reinterpret_cast<DLGPROC>(DialogProc));
		break;

	case ID_QUIT:
		SendMessage(handle, WM_DESTROY, 0, 0);
		break;
	case ID_TEST_SETFONT: {

		settingsOld.apply();

		break;

	}
	default:
		return 1;
	}

	return 0;
}

HBRUSH COverlappedWindow::OnCtlColorEdit(HDC dc) {

	COLORREF backColor;
	COLORREF fontColor;

	if (dialogOpen && preview) {
		backColor = settingsNew.getBackgroundColor();
		fontColor = settingsNew.getFontColor();
	} else {
		backColor = settingsOld.getBackgroundColor();
		fontColor = settingsOld.getFontColor();
	}

	SetBkColor(dc, backColor);
	SetTextColor(dc, fontColor);
	return CreateSolidBrush(backColor);
}

void COverlappedWindow::OnInitDialog(HWND dialogHandle) {
	
	hwndSettingsDialog = dialogHandle;

	preview = true;
	dialogOpen = true;
	SendDlgItemMessage(dialogHandle, IDC_CHECK_PREVIEW, BM_SETCHECK, BST_CHECKED, 0);

	int fontSize = settingsNew.getFontSize();
	SendDlgItemMessage(dialogHandle, IDC_FONT_SLIDER, TBM_SETRANGE, (WPARAM)1, (LPARAM)MAKELONG(8, 72));
	SendDlgItemMessage(dialogHandle, IDC_FONT_SLIDER, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)fontSize);
	
	int transparency = settingsNew.getTransparency();
	SendDlgItemMessage(dialogHandle, IDC_TRANSPARENCY_SLIDER, TBM_SETRANGE, (WPARAM)1, (LPARAM)MAKELONG(0, 255));
	SendDlgItemMessage(dialogHandle, IDC_TRANSPARENCY_SLIDER, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)transparency);

}

void COverlappedWindow::OnHScroll() {
	int positionFont = SendDlgItemMessage(hwndSettingsDialog, IDC_FONT_SLIDER, TBM_GETPOS, 0, 0);
	int positionTransparency = SendDlgItemMessage(hwndSettingsDialog, IDC_TRANSPARENCY_SLIDER, TBM_GETPOS, 0, 0);
	
	settingsNew.setFontSize(positionFont);
	settingsNew.setTransparency(positionTransparency);
	
	if (preview) {
		settingsNew.apply();
	}

}

void COverlappedWindow::OnSettingsBackground(HWND dialogHandle) {

	CHOOSECOLOR cc;
	static COLORREF acrCustClr[16];
	static DWORD rgbCurrent;

	ZeroMemory(&cc, sizeof(cc));
	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = dialogHandle;
	cc.lpCustColors = (LPDWORD)acrCustClr;
	cc.rgbResult = rgbCurrent;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;

	if (ChooseColor(&cc) == TRUE) {
		settingsNew.setBackgroundColor(cc.rgbResult);
	}

	if (preview) {
		settingsNew.apply();
	}

}

void COverlappedWindow::OnSettingsFont(HWND dialogHandle) {

	CHOOSECOLOR cc;
	static COLORREF acrCustClr[16];
	static DWORD rgbCurrent;

	ZeroMemory(&cc, sizeof(cc));
	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = dialogHandle;
	cc.lpCustColors = (LPDWORD)acrCustClr;
	cc.rgbResult = rgbCurrent;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;

	if (ChooseColor(&cc) == TRUE) {
		settingsNew.setFontColor(cc.rgbResult);
	}

	if (preview) {
		settingsNew.apply();
	}

}

void COverlappedWindow::OnSettingsOK(HWND dialogHandle) {
	settingsOld = settingsNew;
	settingsOld.apply();
	dialogOpen = false;
	EndDialog(dialogHandle, 0);
}

void COverlappedWindow::OnSettingsCansel(HWND dialogHandle) {
	settingsOld.apply();
	dialogOpen = false;
	EndDialog(dialogHandle, 0);
}

void COverlappedWindow::OnSettingsPreview(HWND dialogHandle) {

	preview = SendDlgItemMessage(dialogHandle, IDC_CHECK_PREVIEW, BM_GETCHECK, 0, 0);
	if (preview) {
		settingsNew.apply();
	} else {
		settingsOld.apply();
	}

}

void COverlappedWindow::getClientRect(long &width, long &height) {
	
	static RECT rcClient; // client area rectangle
	GetClientRect(handle, &rcClient);
	width = rcClient.right;
	height = rcClient.bottom;
}

void COverlappedWindow::saveText() {
	DWORD textLen = SendMessage(hwndEdit, WM_GETTEXTLENGTH, 0, 0);

	DWORD bufferLen = (textLen + 1) * sizeof(TCHAR);
	TCHAR *buffer = new TCHAR[bufferLen];
	SendMessage(hwndEdit, WM_GETTEXT, (WPARAM)bufferLen, (LPARAM) buffer);

	OPENFILENAME ofn;

    char szFileName[MAX_PATH] = "";

    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn); 
	ofn.hwndOwner = handle;
    ofn.lpstrFilter = (LPCWSTR)L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = (LPWSTR)szFileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = (LPCWSTR)L"txt";

	if (GetSaveFileName(&ofn) == 0) {
		return;
	}

	TCHAR sz[MAX_STRING_LEN];
	LoadString(GetModuleHandle(0), IDS_WND_CAPTION, sz, sizeof(sz) / sizeof(TCHAR));
	if (writeToFile(ofn.lpstrFile, buffer, bufferLen)) {
		MessageBox(handle, L"Saved successfully!", sz, MB_OK);
		textChanged = false;
	} else {
		MessageBox(handle, L"File was not saved...", sz, MB_OK | MB_ICONERROR);
	}

	delete[] buffer;
}

bool COverlappedWindow::writeToFile(LPCTSTR fileName, TCHAR *text, DWORD textLen) {
	
	HANDLE hFile;
    BOOL bSuccess = FALSE;

	hFile = CreateFile(fileName, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if(hFile != INVALID_HANDLE_VALUE) {
        if(sizeof(text) > 0) {
			DWORD writtenBytes;
			if(WriteFile(hFile, text, textLen, &writtenBytes, NULL))
                        bSuccess = TRUE;
        }
		CloseHandle(hFile);
    }
    
    return bSuccess;

}

LRESULT __stdcall COverlappedWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {

	static COverlappedWindow *window;

	switch (message)
	{

	case WM_NCCREATE: {
		window = reinterpret_cast<COverlappedWindow*>((reinterpret_cast<CREATESTRUCT*>(lParam))->lpCreateParams);
		SetLastError(0);
		SetWindowLongPtr(handle, GWLP_USERDATA, reinterpret_cast<LONG>(window));
        if( GetLastError() != 0 ) {
            return GetLastError();
        }
		window->OnNCCreate(handle);
		return DefWindowProc(handle, message, wParam, lParam);
	}
	case WM_CREATE: {
		window->OnCreate();
		return 0;
	}
	case WM_CLOSE: {
		window->OnClose();
	}
	case WM_DESTROY: {
		window->OnDestroy();
		return 0;
	}
	case WM_SIZE: {
		window->OnSize(wParam, lParam);
		return 0;
	}
	case WM_COMMAND: {
		if (window->OnCommand(wParam) == 1) {
			return DefWindowProc(handle, message, wParam, lParam);
		}
	}
	case WM_SETFOCUS:
        SetFocus(window->hwndEdit); 
        return 0; 
	case WM_CTLCOLOREDIT:
		return LRESULT(window->OnCtlColorEdit(reinterpret_cast<HDC>(wParam)));
	default: {
		return DefWindowProc(handle, message, wParam, lParam);
	}
	}
	
}

INT_PTR CALLBACK COverlappedWindow::DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	
	static COverlappedWindow *window;

	switch (uMsg) {
	
	case WM_INITDIALOG:

		window = reinterpret_cast<COverlappedWindow *>(GetWindowLongPtr(GetParent(hwndDlg), GWLP_USERDATA));
		window->OnInitDialog(hwndDlg);
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_CHECK_PREVIEW: {
			window->OnSettingsPreview(hwndDlg);
			return TRUE;
		}
		case IDC_BACKGROUND:
			window->OnSettingsBackground(hwndDlg);
			return TRUE;
		case IDC_FONT:
			window->OnSettingsFont(hwndDlg);
			return TRUE;
		case IDOK:
			window->OnSettingsOK(hwndDlg);
			return TRUE;
		case IDCANCEL:
			window->OnSettingsCansel(hwndDlg);
			return TRUE;
		}
		break;

	case WM_HSCROLL:
		window->OnHScroll();
		return TRUE;

	}

	return FALSE;
}
