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
	SetWindowText(hwndEdit, data);
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
		settingsNew = Settings(hwndEdit);
		settingsOld = Settings(hwndEdit);
		DialogBox(GetModuleHandle(0), MAKEINTRESOURCE(IDD_NOTEPAD_SETTINGS), handle, reinterpret_cast<DLGPROC>(DialogProc));
		return 0;
	case ID_QUIT:
		SendMessage(handle, WM_DESTROY, 0, 0);
	default:
		return 1;
	}

	return 0;
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
	case WM_GETTEXTLENGTH:
		
	default: {
		return DefWindowProc(handle, message, wParam, lParam);
	}
	}
	
}

INT_PTR CALLBACK COverlappedWindow::DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	
	static COverlappedWindow *window;

	switch (uMsg) {
	
	case WM_INITDIALOG:

		window = reinterpret_cast<COverlappedWindow *>(GetWindowLongPtr(GetParent(hwndDlg), 0));

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
			window->settingsOld = window->settingsNew;
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
