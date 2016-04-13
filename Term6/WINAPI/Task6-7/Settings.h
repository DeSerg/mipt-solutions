#include "Common.h"

class Settings {

public:

	Settings();
	Settings(HWND editControl, HWND owner);
	Settings(const Settings &dialog);

	void setBackgroundBrush(HBRUSH backgroundBrush);
	void setFont(HFONT font);
	void setFontSize(int fontSize);
	int getFontSize();
	void setTransparency(int transparency);

	void apply();

private:

	HBRUSH backgroundBrush;
	HFONT font;
	int transparency;
	
	HWND owner;
	HWND editControl;

};