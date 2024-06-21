#include <raylib.h>
#include <iostream>
#include "includes/simulation.hpp"
#include "includes/keybinds.hpp"

int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 800;
int BASE_NCELL = 5;

int BIRTH_TRESHOLD = 3;
int OVERPOPULATION_TRESHOLD = 3;
int UNDERPOPULATION_TRESHOLD = 2;
int main()
{
	int FPS = 60;
	float cellsize = static_cast<float>(WINDOW_WIDTH) / BASE_NCELL;
	float renderTreshold = 0.5f;
	float zoom = 1.0f;
    Camera2D camera = { 0 };

    camera.target = { WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f };
    camera.offset = { WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = zoom;
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "raylib");
	SetTargetFPS(FPS);
	
	Simulation simulation(WINDOW_WIDTH, WINDOW_HEIGHT, cellsize);
	double startTime = GetTime();
	while (WindowShouldClose() == false)
	{
		double currentTime = GetTime();
		double delta = currentTime - startTime;
		KeybindChecks(simulation, camera, renderTreshold, cellsize);
		
		if (simulation.IsRunning() && delta >= renderTreshold)
		{
			simulation.Update();
			startTime = GetTime();
		}
		
		BeginDrawing();
		ClearBackground(GRAY);
		
		BeginMode2D(camera);
		simulation.Draw();
		EndMode2D();
		DrawFPS(0, 0);
		string displayStr = to_string(10*RENDERING_INCREASE/renderTreshold);
		displayStr.resize(4);
		DrawText(displayStr.c_str(), WINDOW_WIDTH - 100, 0, 24 , RED);
		EndDrawing();
	}
	CloseWindow();
}
