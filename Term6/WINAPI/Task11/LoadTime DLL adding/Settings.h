#include "Common.h"

class Settings {

public:

	Settings();
	Settings(HWND editControl, HWND owner, COLORREF backgroundColor, COLORREF fontColor);

	void setBackgroundColor(COLORREF backgroundColor);
	COLORREF getBackgroundColor();
	void setFontColor(COLORREF fontColor);
	COLORREF getFontColor();

	void setFont(HFONT font);
	void setFontSize(int fontSize);
	int getFontSize();
	void setTransparency(int transparency);
	int getTransparency();

	void apply();

private:

	COLORREF backgroundColor;
	COLORREF fontColor;
	HFONT font;
	int transparency;
	
	HWND owner;
	HWND editControl;

};