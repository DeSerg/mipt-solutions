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
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	return (::RegisterClassEx(&wcex) != 0);

}

bool COverlappedWindow::Create() {

	CreateWindowEx(0, L"OverlappedWindow", L"My Window", WS_OVERLAPPEDWINDOW,
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
                    0, L"EDIT",   // predefined class 
                    NULL,         // no window title 
                    WS_CHILD | WS_VISIBLE | WS_VSCROLL | 
                    ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL, 
                    0, 0, 0, 0,   // set size in WM_SIZE message 
                    handle,         // parent window 
                    (HMENU) ID_EDITCHILD,   // edit control ID 
                    (HINSTANCE) GetWindowLong(handle, GWL_HINSTANCE), 
                    NULL);        // pointer not needed 
}

void COverlappedWindow::OnClose() {
	if (textChanged) {
		int msBoxBtn = MessageBox(handle, L"Would you like save your text to file?", L"My Window", MB_YESNOCANCEL);
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
	MoveWindow(hwndEdit, 
			0, 0,                  // starting x- and y-coordinates 
			LOWORD(lParam),        // width of client area 
			HIWORD(lParam),        // height of client area 
			TRUE);                 // repaint window 
}

int COverlappedWindow::OnCommand(WPARAM wParam) {
	switch (HIWORD(wParam)) { 
	case EN_CHANGE:
		textChanged = true;
		break;
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
	if (textLen == 0) {
		return;
	}

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

    GetSaveFileName(&ofn);
	bool writeResult = writeToFile(ofn.lpstrFile, buffer, bufferLen);

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

