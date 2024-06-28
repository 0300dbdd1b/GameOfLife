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

int FPS = 120;							// FRAMES PER SECOND
float UPDATE_RATE = 0.5f;				// RATE OF SIMULATION UPDATES ( 1 every UPDATE_RATE seconds)
float REFRESH_RATE = 0.1f;				// RATE OF WINDOW REFRESHING ( 1 every REFRESH_RATE seconds)

const float RENDERING_INCREASE = 0.1f;
const float ADJUSTMENT_INTERVAL = 0.085f;

const float MAX_REFRESH_RATE = 0.0f;	// MAX RATE TO REFRESH THE WINDOW ( 1 time every MAX_REFRESH_RATE seconds)
const float MIN_REFRESH_RATE = 1.0f;	// MIN RATE TO REFRESH THE WINDOW ( 1 time every MIN_REFRESH_RATE seconds)
const float MAX_UPDATE_RATE = 0.0f;		// MAX RATE TO UPDATE THE SIMULATION ( 1 time every MAX_UPDATE_RATE seconds)
const float MIN_UPDATE_RATE =  1;		// MIN RATE TO UPDATE THE SIMULATION ( 1 time every MIN_UPDATE_RATE seconds)

// Constants for drawing the speed bar
const int TRIANGLE_SIZE = 20; 			// Size of each triangle
const int TRIANGLE_SPACING = 2; 		// Spacing between triangles
const int PADDING = 10; 				// Padding from the top right corner
const int MAX_SPEED_LEVELS = 10; 		// Number of speed levels (triangles)

void MainApp::DrawSimulationSpeed(float speed)
{
    int screenWidth = GetScreenWidth();

    int filledTriangles = static_cast<int>(speed * MAX_SPEED_LEVELS);

    for (int i = 0; i < MAX_SPEED_LEVELS; ++i)
    {
        int baseX = screenWidth - (i + 1) * (TRIANGLE_SIZE + TRIANGLE_SPACING) - PADDING;
        int baseY = PADDING;

        Vector2 top = { (float)baseX + TRIANGLE_SIZE / 2.0f, (float)baseY };
        Vector2 left = { (float)baseX, (float)baseY + TRIANGLE_SIZE };
        Vector2 right = { (float)baseX + TRIANGLE_SIZE, (float)baseY + TRIANGLE_SIZE };

        if (i < filledTriangles)
        {
            DrawTriangle(top, left, right, GRAY);
        }
        else
        {
            DrawTriangle(top, left, right, RED);
        }
    }
}

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

		if (simulation.IsRunning() && updateDelta >= UPDATE_RATE)
		{
			simulation.Update();
			updateStartTime = GetTime();
		}
		if (refreshDelta >= REFRESH_RATE)
		{
			BeginDrawing();
				ClearBackground(BACKGROUND_COLOR);
				BeginMode2D(camera);
					simulation.Draw();
				EndMode2D();
				DrawFPS(0, 0);
				DrawSimulationSpeed(UPDATE_RATE);
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
			Vector2 worldPos = GetScreenToWorld2D(GetMousePosition(), camera);
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
				if (camera.zoom < 1.0f) camera.zoom = 1.0f;  // Prevent zooming out too much

				Vector2 newMouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
				Vector2 deltaPos = { newMouseWorldPos.x - mouseWorldPos.x, newMouseWorldPos.y - mouseWorldPos.y };
				camera.target = { camera.target.x - deltaPos.x, camera.target.y - deltaPos.y };
			}
			else
			{
				// Zoom centered on screen
				camera.zoom += mouseWheelMove * 0.1f;
				if (camera.zoom < 1.0f) camera.zoom = 1.0f;  // Prevent zooming out too much
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
			if ((UPDATE_RATE + RENDERING_INCREASE <= MIN_UPDATE_RATE))
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
		cout << UPDATE_RATE << endl;
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
