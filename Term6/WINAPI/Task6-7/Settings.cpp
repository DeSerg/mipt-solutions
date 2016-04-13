#include "Settings.h"

Settings::Settings() {}

Settings::Settings(HWND editControl, HWND owner, COLORREF backgroundColor, COLORREF fontColor) {
	this->editControl = editControl;
	this->owner = owner;
	this->backgroundColor = backgroundColor;
	this->fontColor = fontColor;

	BYTE byteTrans;
	GetLayeredWindowAttributes(owner, NULL, &byteTrans, NULL);
	transparency = int(byteTrans);



	HFONT originFont = reinterpret_cast<HFONT>(SendMessage(editControl, WM_GETFONT, 0, 0));
	LOGFONT lf;
	GetObject(originFont, sizeof(LOGFONT), &lf);
	font = CreateFontIndirect(&lf);


	if (font == NULL) {
		font = reinterpret_cast<HFONT>(GetStockObject(SYSTEM_FONT));
	}
	
}

void Settings::setBackgroundColor(COLORREF backgroundColor) {
	this->backgroundColor = backgroundColor;
}

COLORREF Settings::getBackgroundColor() {
	return backgroundColor;
}

void Settings::setFontColor(COLORREF fontColor) {

	this->fontColor = fontColor;

}

COLORREF Settings::getFontColor() {
	return fontColor;
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
	InvalidateRect(owner, NULL, 1);
}
