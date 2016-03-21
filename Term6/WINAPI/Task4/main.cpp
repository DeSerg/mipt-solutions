#include "CEllipseWindow.h"
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

	MSG message;
	BOOL getMessageResult = 0;
	while( (getMessageResult = ::GetMessage( &message, 0, 0, 0 )) != 0 ) {
		if( getMessageResult == -1 ) {
			return -1;
		}
		::TranslateMessage(&message);
		::DispatchMessage(&message);
	}

	return 0;
}
