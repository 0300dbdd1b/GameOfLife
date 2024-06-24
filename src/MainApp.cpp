#include "include/MainApp.hpp"
#include "raylib.h"
#include "include/simulation.hpp"
#include <cmath>
#include <string>
#include <iostream>

int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 800;
int BASE_NCELL = 5;

Color BACKGROUND_COLOR = GRAY;

int BIRTH_TRESHOLD = 3;
int OVERPOPULATION_TRESHOLD = 3;
int UNDERPOPULATION_TRESHOLD = 2;

int FPS = 120;
float UPDATE_TRESHOLD = 0.5f;
float REFRESH_TRESHOLD = 0.1f;

const float RENDERING_INCREASE = 0.01;
const float ADJUSTMENT_INTERVAL = 0.085f;

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
	int CELL_SIZE = static_cast<float>(WINDOW_WIDTH) / BASE_NCELL;
	double updateStartTime = GetTime();
	double refreshStartTime = updateStartTime;


	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "GameOfLife");
	SetTargetFPS(FPS);
	while (!WindowShouldClose())
	{
		double currentTime = GetTime();
		float updateDelta = currentTime - updateStartTime;
		float refreshDelta = currentTime - refreshStartTime;

		KeybindsCheck(CELL_SIZE);

		if (simulation.IsRunning() && updateDelta >= UPDATE_TRESHOLD)
		{
			simulation.Update();
			updateStartTime = GetTime();
		}
		if (refreshDelta >= REFRESH_TRESHOLD)
		{
			BeginDrawing();
			ClearBackground(BACKGROUND_COLOR);
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

void MainApp::KeybindsCheck(int &CELL_SIZE)
{

	static Vector2 prevMousePos = { 0.0f, 0.0f };
	static bool isDragging = false;
	static float adjustementTimer = 0.0f;
	
	adjustementTimer += GetFrameTime();
	if (adjustementTimer >= ADJUSTMENT_INTERVAL)
	{
		if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		{
			Vector2 mousePos = GetMousePosition();
			Vector2 worldPos = GetScreenToWorld2D(mousePos, camera);
			simulation.ToggleCell(worldPos.y / CELL_SIZE, worldPos.x / CELL_SIZE);
			adjustementTimer = 0.0f;
		}

		float mouseWheelMove = GetMouseWheelMove();
		bool ctrlPressed = IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL);

		if (mouseWheelMove != 0)
		{
			if (ctrlPressed)
			{
				// Zoom centered on mouse position
				Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
				camera.zoom += mouseWheelMove * 0.1f;
				if (camera.zoom < 0.1f) camera.zoom = 0.1f;  // Prevent zooming out too much

				Vector2 newMouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
				Vector2 deltaPos = { newMouseWorldPos.x - mouseWorldPos.x, newMouseWorldPos.y - mouseWorldPos.y };
				camera.target = { camera.target.x - deltaPos.x, camera.target.y - deltaPos.y };
			}
			else
			{
				// Zoom centered on screen
				camera.zoom += mouseWheelMove * 0.1f;
				if (camera.zoom < 0.1f) camera.zoom = 0.1f;  // Prevent zooming out too much
			}
		}
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
		{
			if (!isDragging)
			{
				prevMousePos = GetMousePosition();
				isDragging = true;
			}

			Vector2 mousePos = GetMousePosition();
			Vector2 delta = { mousePos.x - prevMousePos.x, mousePos.y - prevMousePos.y };

			camera.target.x -= delta.x / camera.zoom;
			camera.target.y -= delta.y / camera.zoom;

			prevMousePos = mousePos;
		}
		else
		{
			isDragging = false;
		}
		// Recenter the camera when 'X' is pressed
		if (IsKeyPressed(KEY_X))
		{
			camera.target = { WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f };
			camera.zoom = 1.0f;
		}

		if (IsKeyPressed(KEY_SPACE))
		{
			if (simulation.IsRunning())
			{
				simulation.Stop();
				SetWindowTitle("Simulation Paused");
			}
			else
			{
				simulation.Start();
				SetWindowTitle("Simulation Running");
			}
		}

		if (IsKeyDown(KEY_F))
		{
			if (( UPDATE_TRESHOLD - RENDERING_INCREASE) >= 0)
			{	
				UPDATE_TRESHOLD -= RENDERING_INCREASE;
			}
			adjustementTimer = 0.0f;
		}
		if (IsKeyDown(KEY_S))
		{
			if ((UPDATE_TRESHOLD + RENDERING_INCREASE) >= 0)
			{
				UPDATE_TRESHOLD += RENDERING_INCREASE;
			}
			adjustementTimer = 0.0f;
		}
		if (IsKeyPressed(KEY_RIGHT_BRACKET))
		{
			CELL_SIZE = ComputeNewCellSize(CELL_SIZE, 5);
			cout << "CELLSIZE : " << CELL_SIZE << endl;

		}
		if (IsKeyPressed(KEY_LEFT_BRACKET))
		{
			CELL_SIZE = ComputeNewCellSize(CELL_SIZE, -5);
			cout << "CELLSIZE : " << CELL_SIZE << endl;
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
}


int MainApp::ComputeNewCellSize(int &cellsize, int n)
{
	cout << "minisell : " << cellsize << endl;
	int nCells = round((WINDOW_WIDTH / cellsize) + n);
	cout << "NCELLS : " << nCells << endl;
	if (nCells >= BASE_NCELL)
	{
		int newCellSize = round(WINDOW_WIDTH / nCells);
		if ((newCellSize >= 2) && (newCellSize != cellsize))
		{

			simulation.Resize(WINDOW_WIDTH, WINDOW_HEIGHT, newCellSize);
			return newCellSize;
		}
	}
	return (round(WINDOW_WIDTH / BASE_NCELL));
}
