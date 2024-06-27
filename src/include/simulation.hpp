#pragma once
#include "grid.hpp"

extern int OVERPOPULATION_TRESHOLD;		// Any living cell with more neighbors than this treshold dies
extern int UNDERPOPULATION_TRESHOLD;	// Any living cell with less neighbors than this treshold dies
extern int BIRTH_TRESHOLD;				// Any dead cell with exactly this number of neighbors become alive
class Simulation
{
	public:
		Simulation(int width, int height, int cellSize);
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
		Grid grid;
		Grid tmpGrid;
		bool run;
};
