#include "Common.h"
#include "Cell.h"
#include "Sudoku.h"

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

protected:
	void OnNCCreate(HWND _handle);
	void OnCreate();
	void OnClose();
	void OnDestroy();
	void OnTimer();
	void OnPaint();

	void On_LButtonDown(WPARAM wParam, LPARAM lParam);
	void On_LButtonUp(WPARAM wParam, LPARAM lParam);

	void OnInitMenuDialog(HWND hwndDlg);
	void OnMenuStart(HWND hwndDlg);
	void OnMenuRestart(HWND hwndDlg);
	void OnMenuDev(HWND hwndDlg);
	void OnMenuClose(HWND hwndDlg);

private:

	enum Action { START, RESTART, DEV, NONE };

	HWND handle;
	int xEllipseOld;
	int yEllipseOld;
	int xEllipse;
	int yEllipse;

	std::vector<HBITMAP> digits;
	std::vector<HBITMAP> digitsPressed;
	std::vector<HBITMAP> menuDigits;
	std::vector<HBITMAP> menuDigitsPressed;
	HBITMAP menuButtonBMP, menuButtonPressedBMP;


	int iPressed, jPressed;
	bool isDigitPressed;
	Sudoku sudoku = Sudoku();
	std::vector<std::vector<int>> grid;
	std::vector<HBITMAP> menuButtons;
	std::vector<RECT> menuRects;

	int fieldSize;
	int cellSize = 75;
	int menuItemSize = 55;
	int winSize = cellSize * 9;
	int menuWidth = cellSize * 1.5;
	int menuMarginTop = 5;
	int menuMarginBottom = 15;


	static const DWORD windowExStyle = 0;
	static const DWORD windowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

	static const int Delta = 1;
	static const int Radius = 30;
	static const int TimerID = 10;

	void initData();
	void loadResources();
	void drawGrid(HDC hdc);
	void drawField(HDC hdc);
	void drawMenuItem(HDC hdc, int index);
	void drawMenu(HDC hdc);
	void drawDigit(HDC hdc, int x, int y, int digit, bool redrawField = true);
	void drawBitmap(HDC hdc, int x, int y, HBITMAP bitmap, bool redrawField = true);
	void drawEllipse(HDC dc, int x, int y, COLORREF fillColor, COLORREF outlineColor);
	void getClientRect(long &width, long &height);

	void ErrorExit(LPTSTR lpszFunction);

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
