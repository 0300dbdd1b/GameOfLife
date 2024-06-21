#pragma once

#include "raylib.h"
#include "simulation.hpp"
class MainApp
{
	public:
		MainApp();
		void MainLoop();
	private:
		Simulation simulation;
		Camera2D camera;
};

