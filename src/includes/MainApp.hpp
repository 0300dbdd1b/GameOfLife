#pragma once

#include "raylib.h"
#include "simulation.hpp"
class MainApp
{
	public:
		MainApp();
		void MainLoop();
		void KeybindsCheck(int &cellsize);
		int ComputeNewCellSize(int &cellsize, int n);
	private:
		Simulation simulation;
		Camera2D camera;
};

