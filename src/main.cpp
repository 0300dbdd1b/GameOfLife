#include <raylib.h>
#include <iostream>
#include "includes/simulation.hpp"
#include "includes/keybinds.hpp"

const int WINDOW_WIDTH = 750;
const int WINDOW_HEIGHT = 750;
int main()
{
	int FPS = 60;
	const int NCells = 100;
	double cellsize = WINDOW_WIDTH / NCells;
	double renderTreshold = 0.5f;

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "raylib");
	SetTargetFPS(FPS);
	
	Simulation simulation(WINDOW_WIDTH, WINDOW_HEIGHT, cellsize);
	double startTime = GetTime();
	while (WindowShouldClose() == false)
	{
		double currentTime = GetTime();
		double delta = currentTime - startTime;
		KeybindChecks(simulation, renderTreshold, cellsize);
		
		if (simulation.IsRunning() && delta >= renderTreshold )
		{
			simulation.Update();
			startTime = GetTime();
			
		}
		BeginDrawing();
		ClearBackground(DARKGRAY);
		simulation.Draw();
		DrawFPS(0, 0);
		string displayStr = to_string(renderTreshold);
		displayStr.resize(4);
		DrawText(displayStr.c_str(), WINDOW_WIDTH - 100, 0, 24 , RED);
		EndDrawing();
	}
	CloseWindow();
}
