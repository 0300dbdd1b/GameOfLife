#pragma once
#include "raylib.h"
# include <vector>

#define RANDOM_ALIVE_TRESHOLD 3
#define DEAD 0
#define ALIVE 1

using namespace std;
class Grid
{
	public:
		Grid();
		Grid(int width, int height, int cellsize);
		~Grid();
		void SetValue(int row, int column, int value);
		int GetValue(int row, int column);
		int GetRows() {return rows;}
		int GetColumns() {return columns;}
		float GetCellSize() {return cellSize;}
		bool IsWithinBounds(int row, int column);
		void FillRandom();
		void Clear();
		void ToggleCell(int row, int column);
		int rows;
		int columns;
		int cellSize;
		vector<vector<int>> cells;
};
