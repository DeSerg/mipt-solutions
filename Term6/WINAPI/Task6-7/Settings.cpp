#include "Settings.h"

Settings::Settings() {}

Settings::Settings(HWND editControl, HWND owner, COLORREF color) {
	this->editControl = editControl;
	this->owner = owner;

	BYTE byteTrans;
	GetLayeredWindowAttributes(owner, NULL, &byteTrans, NULL);
	transparency = int(byteTrans);

	backgroundBrush = CreateSolidBrush(color);

	HFONT originFont = reinterpret_cast<HFONT>(SendMessage(editControl, WM_GETFONT, 0, 0));
	LOGFONT lf;
	GetObject(originFont, sizeof(LOGFONT), &lf);
	font = CreateFontIndirect(&lf);


	if (font == NULL) {
		font = reinterpret_cast<HFONT>(GetStockObject(SYSTEM_FONT));
	}
	
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

HBRUSH Settings::getBackgroundBrush() {
	return backgroundBrush;
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

int Settings::getTransparency() {
	return transparency;
}


void Settings::apply() {

	int fontSize = getFontSize();
	SendMessage(editControl, WM_SETFONT, reinterpret_cast<WPARAM>(font), true);
	SetLayeredWindowAttributes(owner, 0, transparency, LWA_ALPHA);

}
