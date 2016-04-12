#include "Common.h"

class Settings {

public:

	Settings(HWND editControl);
	Settings(const Settings &dialog);

	void setBackgroundBrush(HBRUSH backgroundBrush);
	void setFont(HFONT font);
	void setFontSize(int fontSize);
	void setTransparency(int transparency);

	void copyEditControlSettings();

	void apply();


private:

	HBRUSH backgroundBrush;
	HFONT font;
	int fontSize;
	int transparency;
	
	HWND editControl;

};