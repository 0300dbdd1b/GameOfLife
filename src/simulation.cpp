#include <vector>
#include <utility>
#include "include/simulation.hpp"
#include <iostream>
using namespace std;


Simulation::Simulation(int width, int height, int cellsize)
{
	grid = Grid(width, height, cellsize);
	tmpGrid = Grid(width, height, cellsize);
	grid.FillRandom();
	run = false;
}

void Simulation::Draw()
{
	grid.Draw();
}


void Simulation::SetCellValue(int row, int column, int value)
{
	grid.SetValue(row, column, value);
}

int Simulation::CountLiveNeighbors(int row, int column)
{
    int liveNeighbors = 0;
    const int neighborOffsets[8][2] = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1},
        {-1, -1}, {1, -1}, {1, 1}, {-1, 1}
    };

    for (int i = 0; i < 8; ++i)
    {
        int neighborRow = (row + neighborOffsets[i][0] + grid.GetRows()) % grid.GetRows();
        int neighborColumn = (column + neighborOffsets[i][1] + grid.GetColumns()) % grid.GetColumns();
        liveNeighbors += grid.GetValue(neighborRow, neighborColumn);
    }
    return liveNeighbors;
}


void Simulation::Update()
{
	for (int row = 0; row < grid.GetRows(); row++)
	{
		for (int column = 0; column < grid.GetColumns(); column++)
		{
			int liveNeighbors = CountLiveNeighbors(row, column);
			int cellValue = grid.GetValue(row, column);

			if (cellValue == ALIVE)
			{
				if (liveNeighbors > OVERPOPULATION_TRESHOLD || liveNeighbors < UNDERPOPULATION_TRESHOLD)
				{
					tmpGrid.SetValue(row, column, DEAD);
				}
				else
				{
					tmpGrid.SetValue(row, column, ALIVE);
				}
			}
			else
			{
				if (liveNeighbors == BIRTH_TRESHOLD)
 				{
					tmpGrid.SetValue(row, column, ALIVE);
				}
				else
				{
					tmpGrid.SetValue(row, column, DEAD);
				}
			}
		}
	}
	grid = tmpGrid;
}


bool Simulation::IsRunning()
{
	return run;
} 

void Simulation::ClearGrid()
{
	grid.Clear();
}

void Simulation::CreateRandomState()
{
	grid.FillRandom();
}

void Simulation::ToggleCell(int row, int column)
{
	if (!IsRunning())
	{
		grid.ToggleCell(row, column);
	}
}

void Simulation::Resize(int width, int height, int cellsize)
{
	if (cellsize <= 2)
	{cellsize= 2;}
	Grid newGrid (width, height, cellsize);
	newGrid.FillRandom();
	grid = newGrid;
	tmpGrid = newGrid;
}

int Simulation::GetWidth()
{
	int width = grid.GetRows() * grid.GetCellSize();
	return width;
}

int Simulation::GetHeight()
{
	int height = grid.GetColumns() * grid.GetCellSize();
	return height;
}
