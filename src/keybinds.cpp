#include "raylib.h"
#include "includes/keybinds.hpp"
#include <iostream>

using namespace std;
void KeybindChecks(Simulation &simulation, double &renderTreshold, double &cellsize)
{
	if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		Vector2 mousePosition = GetMousePosition();
		int row = mousePosition.y / cellsize;
		int column = mousePosition.x / cellsize;
		simulation.ToggleCell(row, column);
	}
	if (IsKeyPressed(KEY_SPACE))
	{
		if (simulation.IsRunning())
		{
			simulation.Stop();
			SetWindowTitle("Simulation stopped");
		}
		else
		{
			simulation.Start();
			SetWindowTitle("Simulation started");
		}
	}
	if (IsKeyPressed(KEY_F))
	{
		renderTreshold -= 0.1;
	}
	if (IsKeyPressed(KEY_S))
	{
		renderTreshold += 0.1;
	}
	if (IsKeyPressed(KEY_RIGHT_BRACKET))
	{
		cellsize = ComputeNewCellSize(simulation, cellsize, 10);
		cout << "CELLSIZE : " << cellsize << endl;
		
	}
	if (IsKeyPressed(KEY_LEFT_BRACKET))
	{
		cellsize = ComputeNewCellSize(simulation, cellsize, -10);
		cout << "CELLSIZE : " << cellsize << endl;
	}
	if (IsKeyPressed(KEY_R))
	{
		simulation.CreateRandomState();
	}
	if (IsKeyPressed(KEY_C))
	{
		simulation.ClearGrid();
	}
}

int ComputeNewCellSize(Simulation &simulation, int cellsize, int n)
{
	cout << "OUI CELLSIZE : " << cellsize << endl;
	int nCells = (WINDOW_WIDTH / cellsize) + n;
	cout << "NCELSS : " << nCells << endl;
	if (nCells >= 1)
	{
		int newCellSize = (WINDOW_WIDTH / nCells);
		cout << "newCellSize" << newCellSize << endl;
		simulation.Resize(WINDOW_WIDTH, WINDOW_HEIGHT, newCellSize);
		return newCellSize;
	}
	return 1;
}
