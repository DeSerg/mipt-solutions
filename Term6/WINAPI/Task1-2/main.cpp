#include "COverlappedWindow.h"
#include <exception>

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{

	if (!COverlappedWindow::RegisterClassW()) {
		return 1;
	}

	COverlappedWindow window;

	if (!window.Create()) {
		return 1;
	}

	window.Show(nCmdShow);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}

	return 0;
}
