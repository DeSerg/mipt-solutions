#include "Common.h"

class Sudoku {

public:

	Sudoku();

	void getRandomGrid(std::vector<std::vector<int>> &grid);
	void getPreviousGrid(std::vector<std::vector<int>> &grid);

private:
	
	std::vector<std::vector<std::vector<int>>> grids;
	std::vector<std::vector<int>> prevGrid;

	void readGrids();
};