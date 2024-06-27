#pragma once

#include "raylib.h"
#include "simulation.hpp"

extern int WINDOW_HEIGHT;
extern int WINDOW_WIDTH;
class MainApp
{
	public:
		MainApp();
		void MainLoop();
		void KeybindsCheck(int &cellsize);
		int ComputeNewCellSize(int &cellsize, int n);
		void Draw(int &cellsize);
	private:
		Simulation simulation;
		Camera2D camera;
		RenderTexture2D renderTexture;
};

