#include "Settings.h"

Settings::Settings(HWND editControl) {
	this->editControl = editControl;
	backgroundBrush = CreateSolidBrush(RGB(255, 255, 255));
	font = reinterpret_cast<HFONT>(SendMessage(editControl, WM_GETFONT, 0, 0));
	transparency = 0;
}

Settings::Settings(const Settings &dialog) {}

void Settings::setBackgroundBrush(HBRUSH backgroundBrush) {
	if (this->backgroundBrush != NULL) {
		DeleteObject(this->backgroundBrush);
	}
	this->backgroundBrush = backgroundBrush;
}

void Settings::setFont(HFONT font) {
	if (this->font != NULL) {
		DeleteObject(this->font);
	}
	this->font = font;
}

void Settings::setFontSize(int fontSize) {
	this->fontSize = fontSize;
}

void Settings::setTransparency(int transparency) {
	this->transparency = transparency;
}

void Settings::copyEditControlSettings() {

	setFont(reinterpret_cast<HFONT>(SendMessage(editControl, WM_GETFONT, 0, 0)));
	

}