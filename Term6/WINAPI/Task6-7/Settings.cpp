#include "Settings.h"

Settings::Settings() {}

Settings::Settings(HWND editControl, HWND owner) {
	this->editControl = editControl;
	this->owner = owner;
	backgroundBrush = CreateSolidBrush(RGB(255, 255, 255));
	
	font = reinterpret_cast<HFONT>(SendMessage(editControl, WM_GETFONT, 0, 0));
	if (font == NULL) {
		font = reinterpret_cast<HFONT>(GetStockObject(SYSTEM_FONT));
	}
	transparency = 0;
}

Settings::Settings(const Settings &settings) {
	this->editControl = settings.editControl;
	this->backgroundBrush = settings.backgroundBrush;
	this->font = settings.font;
	this->transparency = settings.transparency;
}

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

	LOGFONT lf;
	GetObject(font, sizeof(lf), &lf);
	lf.lfHeight = fontSize;

	DeleteObject(font);
	font = CreateFontIndirect(&lf);

}

int Settings::getFontSize() {

	LOGFONT lf;
	GetObject(font, sizeof(lf), &lf);
	return lf.lfHeight;

}

void Settings::setTransparency(int transparency) {
	this->transparency = transparency;
}

void Settings::apply() {

	SendMessage(editControl, WM_SETFONT, reinterpret_cast<WPARAM>(font), true);
	SendMessage(editControl, EM_SETMODIFY, true, 0);

}
