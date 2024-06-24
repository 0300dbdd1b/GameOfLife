#include "raylib.h"
#include "include/grid.hpp"

const int DRAWCASE_OFFSET = 1;
const Color ALIVE_CELL_COLOR = GREEN;
const Color DEAD_CELL_COLOR = DARKGRAY;

Grid::Grid()
{
	Grid(100,100,10);
}

Grid::Grid(int width, int height, int cellsize)
{
	if (cellsize <= 0)
	{cellsize = 1;}
	rows = (height + cellsize - 1) / cellsize;
	columns = (width + cellsize - 1) / cellsize;
	cellSize = cellsize;
	cells = vector<vector<int>>(rows, vector<int>(columns,0));
}

void Grid::Draw()
{
	for (int row = 0; row < rows; row++)
	{
		for (int column = 0; column < columns; column++)
		{
			Color color = cells[row][column] ? ALIVE_CELL_COLOR : DEAD_CELL_COLOR;
			DrawRectangle(column * cellSize, row * cellSize, cellSize - DRAWCASE_OFFSET, cellSize - DRAWCASE_OFFSET, color);
		}
	}
}

void Grid::FillRandom()
{
	for (int row = 0; row < rows; row++)
	{
		for (int column = 0; column < columns; column++)
		{
			int randomValue = GetRandomValue(0, RANDOM_ALIVE_TRESHOLD);
			cells[row][column] = (randomValue == RANDOM_ALIVE_TRESHOLD) ? ALIVE : DEAD;
		}
	}
}
bool Grid::IsWithinBounds(int row, int column)
{
	if ((row >= 0) && (row < rows) && (column >= 0) && (column < columns))
	{
		return true;
	}
	return false;

}

void Grid::SetValue(int row, int column, int value)
{
	if (IsWithinBounds(row, column))
	{
		cells[row][column] = value;
	}
}

int Grid::GetValue(int row, int column)
{
	if (IsWithinBounds(row, column))
	{
		return cells[row][column];
	}
	return 0;
}

void Grid::Clear()
{
	for (int row = 0; row < rows; row++)
	{
		for (int column = 0; column < columns; column++)
		{
			cells[row][column] = DEAD;
		}
	}
}

void Grid::ToggleCell(int row, int column)
{
	if (IsWithinBounds(row, column))
	{
		cells[row][column] = !cells[row][column];
	}
}
