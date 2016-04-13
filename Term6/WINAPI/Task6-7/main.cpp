#include "COverlappedWindow.h"

#include <exception>

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{

	if (!COverlappedWindow::RegisterClass()) {
		return 1;
	}

	COverlappedWindow window;

	if (!window.Create()) {
		return 1;
	}

	window.Show(nCmdShow);

	HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));

	MSG message;
	BOOL getMessageResult = 0;
	while( (getMessageResult = ::GetMessage( &message, 0, 0, 0 )) != 0 ) {
		if( getMessageResult == -1 ) {
			return -1;
		}
		if (!TranslateAccelerator(window.getHandle(), hAccel, &message) &&
			(!IsWindow(window.getHandle()) || !IsDialogMessage(window.getHandle(), &message))) {
			::TranslateMessage(&message);
			::DispatchMessage(&message);
		}
	}

	return 0;
}
