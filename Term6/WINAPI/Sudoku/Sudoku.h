#include "Common.h"

class Sudoku {

public:

	Sudoku();

	Sudoku(int hardness);

private:
	
	std::vector<std::vector<int>> grid;



	void initRandomGrid(int hardness);


};