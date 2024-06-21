#include "raylib.h"
#include "includes/keybinds.hpp"
#include <iostream>

extern const double RENDERING_INCREASE = 0.05;
using namespace std;
void KeybindChecks(Simulation &simulation, Camera2D &camera, float &renderTreshold, float &cellsize)
{   

	static Vector2 prevMousePos = { 0.0f, 0.0f };
    static bool isDragging = false;
	if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		Vector2 mousePos = GetMousePosition();
        Vector2 worldPos = GetScreenToWorld2D(mousePos, camera);
		simulation.ToggleCell(worldPos.y / cellsize, worldPos.x / cellsize);
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
	if (IsKeyPressed(KEY_F))
	{
		if ((renderTreshold - RENDERING_INCREASE) >= 0)
		{	
			renderTreshold -= RENDERING_INCREASE;
		}
	}
	if (IsKeyPressed(KEY_S))
	{
		if ((renderTreshold + RENDERING_INCREASE) >= 0)
		{
			renderTreshold += RENDERING_INCREASE;
		}
	}
	if (IsKeyPressed(KEY_RIGHT_BRACKET))
	{
		cellsize = ComputeNewCellSize(simulation, cellsize, 5);
		cout << "CELLSIZE : " << cellsize << endl;

	}
	if (IsKeyPressed(KEY_LEFT_BRACKET))
	{
		cellsize = ComputeNewCellSize(simulation, cellsize, -5);
		cout << "CELLSIZE : " << cellsize << endl;
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

float ComputeNewCellSize(Simulation &simulation, float cellsize, int n)
{
	int nCells = ((static_cast<float>(WINDOW_WIDTH) / cellsize) + n);
	cout << "NCELSS : " << nCells << endl;
	if (nCells >= 1)
	{
		float newCellSize = static_cast<float>(WINDOW_WIDTH) / nCells;
		if ((newCellSize >= 1) && (newCellSize != cellsize))
		{

			cout << "newCellSize" << newCellSize << endl;
			simulation.Resize(WINDOW_WIDTH, WINDOW_HEIGHT, newCellSize);
			return newCellSize;
		}
	}
	return (static_cast<float>(WINDOW_WIDTH) / BASE_NCELL);
}
