#include "Sudoku.h"

Sudoku::Sudoku() {

	readGrids();

}

void Sudoku::getRandomGrid(std::vector<std::vector<int>> &grid) {

	grid = grids[rand() % grids.size()];
	prevGrid = grid;

}	

void Sudoku::getPreviousGrid(std::vector<std::vector<int>> &grid) {

	if (!prevGrid.empty()) {
		grid = prevGrid;
	} else {
		grid = grids[rand() % grids.size()];
	}

}


void Sudoku::readGrids() {

	std::ifstream input("res\\sudoku.txt");
	std::string line;
	if (input.is_open())
	{
		getline(input, line);

		grids.clear();
		grids.push_back(std::vector<std::vector<int>>(9, std::vector<int>(9, 0)));

		int gridId = 0;
		int lineNum = 1;
		while (getline(input, line))
		{
			if (lineNum % 10 == 0) {
				gridId++;
				lineNum++;
				grids.push_back(std::vector<std::vector<int>>(9, std::vector<int>(9, 0)));
				continue;
			} else {
				int gridLine = lineNum % 10 - 1;
				for (int i = 0; i < 9; ++i) {
					grids[gridId][gridLine][i] = line[i] - 48;
				}
				lineNum++;
			}
		}
		input.close();
	}

}