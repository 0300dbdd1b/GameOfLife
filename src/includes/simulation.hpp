#pragma once
#include "grid.hpp"


class Simulation
{
	public:
		Simulation(int width, int height, int cellSize);
		void Draw();
		void SetCellValue(int row, int column, int value);
		int CountLiveNeighbors(int row, int column);
		void Update();
		bool IsRunning();
		void Start() {run = true;}
		void Stop() {run = false;}
		void ClearGrid();
		void ToggleCell(int row, int column);
		void CreateRandomState();
		void Resize(int width, int height, int cellsize);
		int GetWidth();
		int GetHeight();
	private:
		Grid grid;
		Grid tmpGrid;
		bool run;
};
