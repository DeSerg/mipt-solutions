#include "Common.h"

struct Cell {

	Cell() : value(0), pressed(false) {}

	Cell(int value, bool pressed) : value(value), pressed(pressed) {}

	Cell(const Cell &cell) {
		this->value = cell.value;
		this->pressed = cell.pressed;
	}

	int value;
	bool pressed;

};