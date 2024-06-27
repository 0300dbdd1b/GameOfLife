#include "include/MainApp.hpp"
#include "raylib.h"
#include "include/simulation.hpp"
#include <cmath>
#include <string>
#include <iostream>

int BASE_NCELL = 5;

Color BACKGROUND_COLOR = GRAY;

int BIRTH_TRESHOLD = 3;
int OVERPOPULATION_TRESHOLD = 3;
int UNDERPOPULATION_TRESHOLD = 2;

int FPS = 120;							// FRAMES PER SECOND
float UPDATE_RATE = 0.5f;				// RATE OF SIMULATION UPDATES ( 1 every UPDATE_RATE seconds)
float REFRESH_RATE = 0.1f;				// RATE OF WINDOW REFRESHING ( 1 every REFRESH_RATE seconds)

const float RENDERING_INCREASE = 0.01f;
const float ADJUSTMENT_INTERVAL = 0.085f;

const float MAX_REFRESH_RATE = 0.0f;	// MAX RATE TO REFRESH THE WINDOW ( 1 time every MAX_REFRESH_RATE seconds)
const float MIN_REFRESH_RATE = 1.0f;	// MIN RATE TO REFRESH THE WINDOW ( 1 time every MIN_REFRESH_RATE seconds)
const float MAX_UPDATE_RATE = 0.0f;		// MAX RATE TO UPDATE THE SIMULATION ( 1 time every MAX_UPDATE_RATE seconds)
const float MIN_UPDATE_RATE = 1/FPS;	// MIN RATE TO UPDATE THE SIMULATION ( 1 time every MIN_UPDATE_RATE seconds)
const int MAX_ARROWS = 6;
const int DRAWCASE_OFFSET = 1;
const Color ALIVE_CELL_COLOR = GREEN;
const Color DEAD_CELL_COLOR = DARKGRAY;

MainApp::MainApp()
	:	simulation(WINDOW_WIDTH, WINDOW_HEIGHT, static_cast<float>(WINDOW_WIDTH) / BASE_NCELL),
		camera({ 
		.offset={ WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f },
		.target={ WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f }, 
		.rotation = 0.0f,
		.zoom = 1.0f})
{
	renderTexture = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
}

void MainApp::MainLoop()
{
	int CELL_SIZE = static_cast<float>(WINDOW_WIDTH) / BASE_NCELL;
	double updateStartTime = GetTime();
	double refreshStartTime = updateStartTime;

	//SetTargetFPS(FPS);
	while (!WindowShouldClose())
	{
		double currentTime = GetTime();
		float updateDelta = currentTime - updateStartTime;
		float refreshDelta = currentTime - refreshStartTime;

		KeybindsCheck(CELL_SIZE);

		if (simulation.IsRunning() && updateDelta >= UPDATE_RATE)
		{
			simulation.Update();
			updateStartTime = GetTime();
		}
		if (refreshDelta >= REFRESH_RATE)
		{
			Draw(CELL_SIZE);
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
			if (( UPDATE_RATE - RENDERING_INCREASE) >= MAX_UPDATE_RATE)
			{	
				UPDATE_RATE -= RENDERING_INCREASE;
			}
			else
			{
				UPDATE_RATE = MAX_UPDATE_RATE;
			}
			adjustementTimer = 0.0f;
		}
		if (IsKeyDown(KEY_S))
		{
			if ((UPDATE_RATE + RENDERING_INCREASE >= MIN_UPDATE_RATE))
			{
				UPDATE_RATE += RENDERING_INCREASE;
			}
			else
			{
				UPDATE_RATE = MIN_UPDATE_RATE;
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
		if (IsKeyPressed(KEY_F11))
		{
	  		ToggleFullscreen();
		}
	}
}

int MainApp::ComputeNewCellSize(int &cellsize, int n)
{
	int nCells = round((WINDOW_WIDTH / cellsize) + n);
	cout << "NCELLS : " << nCells << endl;
	if (nCells >= BASE_NCELL)
	{
		int newCellSize = round(WINDOW_WIDTH / nCells);
		if ((newCellSize >= 1))
		{

			simulation.Resize(WINDOW_WIDTH, WINDOW_HEIGHT, newCellSize);
			return newCellSize;
		}
	}
	return (round(WINDOW_WIDTH / BASE_NCELL));
}

void MainApp::Draw(int &CELL_SIZE)
{
	BeginTextureMode(renderTexture);
		ClearBackground(BLACK);
		BeginMode2D(camera);
		for (int row = 0; row < simulation.grid.GetRows(); row++)
		{
			for (int column = 0; column < simulation.grid.GetColumns(); column++)
			{
				Color color = simulation.grid.cells[row][column] ? ALIVE_CELL_COLOR : DEAD_CELL_COLOR;
				Vector2 position = {(float)(column * CELL_SIZE),(float)(row * CELL_SIZE)};
				Vector2 size = {(float)(CELL_SIZE - DRAWCASE_OFFSET),(float)(CELL_SIZE - DRAWCASE_OFFSET)};
				DrawRectangleV(position, size, color);
			}
		}
		EndMode2D();
	EndTextureMode();

	BeginDrawing();
		ClearBackground(BLACK);
		BeginMode2D(camera);
			DrawTexture(renderTexture.texture, 0, 0, WHITE);
		EndMode2D();
	DrawFPS(0,0);
	EndDrawing();
}
