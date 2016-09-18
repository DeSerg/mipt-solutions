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
	void On_WM_CHAR(WPARAM wParam, LPARAM lParam);
	void On_WM_KEYDOWN(WPARAM wParam, LPARAM lParam);
	void On_WM_KEYUP(WPARAM wParam, LPARAM lParam);



	void OnInitMenuDialog(HWND hwndDlg);
	void OnMenuStart(HWND hwndDlg);
	void OnMenuRestart(HWND hwndDlg);
	void OnMenuDev(HWND hwndDlg);
	void OnMenuClose(HWND hwndDlg);

private:

	enum Action { START, RESTART, DEV, NONE };

	HWND handle;

	std::vector<HBITMAP> digits;
	std::vector<HBITMAP> digits_pressed_;
	std::vector<HBITMAP> menu_digits_;
	std::vector<HBITMAP> menu_digits_pressed_;
	HBITMAP menu_button_BMP_, menu_button_pressed_BMP_;

	int i_pressed_, j_pressed_;
	bool is_digit_pressed_;
	Sudoku sudoku = Sudoku();
	std::vector<std::vector<int>> grid;
	std::vector<HBITMAP> menu_buttons_;

	//Rectangles for easyer calculations
	std::vector<RECT> menu_rects_;
	std::vector< std::vector<RECT> > field_rects_;

	int field_size_;
	int cell_size_ = 75;
	int menu_item_size_ = 55;
	int win_size_ = cell_size_ * 9;
	int menu_width_ = cell_size_ * 1.5;
	int menu_margin_top_ = 5;
	int menu_margin_bottom_ = 15;


	static const DWORD windowExStyle = 0;
	static const DWORD windowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

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
	void updateRect(RECT &rect);

	void ErrorExit(LPTSTR lpszFunction);

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
