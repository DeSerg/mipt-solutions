#include "Common.h"

class Settings {

public:

	Settings();
	Settings(HWND editControl, HWND owner, COLORREF color);
	Settings(const Settings &dialog);

	void setBackgroundBrush(HBRUSH backgroundBrush);
	HBRUSH getBackgroundBrush();
	void setFont(HFONT font);
	void setFontSize(int fontSize);
	int getFontSize();
	void setTransparency(int transparency);
	int getTransparency();

	void apply();

private:

	HBRUSH backgroundBrush;
	HFONT font;
	int transparency;
	
	HWND owner;
	HWND editControl;

};