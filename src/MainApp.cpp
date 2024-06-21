#include "includes/MainApp.hpp"
#include "includes/keybinds.hpp"
#include "raylib.h"
#include "simulation.hpp"
#include <string>

int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 800;
int BASE_NCELL = 5;

int BIRTH_TRESHOLD = 3;
int OVERPOPULATION_TRESHOLD = 3;
int UNDERPOPULATION_TRESHOLD = 2;

int FPS = 120;
float UPDATE_TRESHOLD = 0.5f;
float REFRESH_TRESHOLD = 0.01f;
MainApp::MainApp()
	:	simulation(WINDOW_WIDTH, WINDOW_HEIGHT, static_cast<float>(WINDOW_WIDTH) / BASE_NCELL),
		camera({ 
		.offset={ WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f },
		.target={ WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f }, 
		.rotation = 0.0f,
		.zoom = 1.0f})
{


}

void MainApp::MainLoop()
{
	float CELL_SIZE = static_cast<float>(WINDOW_WIDTH) / BASE_NCELL;
	double updateStartTime = GetTime();
	double refreshStartTime = updateStartTime;


	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "GameOfLife");
	SetTargetFPS(FPS);
	while (!WindowShouldClose())
	{
		double currentTime = GetTime();
		double updateDelta = currentTime - updateStartTime;
		double refreshDelta = currentTime - refreshStartTime;

		KeybindChecks(simulation, camera, UPDATE_TRESHOLD, CELL_SIZE);

		if (simulation.IsRunning() && updateDelta >= UPDATE_TRESHOLD)
		{
			simulation.Update();
			updateStartTime = GetTime();
		}
		if (refreshDelta >= REFRESH_TRESHOLD)
		{
			BeginDrawing();
			ClearBackground(GRAY);
			BeginMode2D(camera);
			simulation.Draw();
			EndMode2D();
			DrawFPS(0, 0);
			string displayUpdateTreshold = to_string(UPDATE_TRESHOLD);
			displayUpdateTreshold.resize(4);
			DrawText(displayUpdateTreshold.c_str(), WINDOW_WIDTH - 100, 0, 24, RED);
			EndDrawing();
		}
	}
	CloseWindow();


}
