#include <raylib.h>
#include "includes/simulation.hpp"
int main()
{
	Color GREY = {29, 29, 29, 255};
	int FPS = 1;
	const int WINDOW_WIDTH = 750;
	const int WINDOW_HEIGHT = 750;
	const int CELL_SIZE = 25;

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "raylib");
	SetTargetFPS(FPS);
	
	Simulation simulation(WINDOW_WIDTH, WINDOW_HEIGHT, CELL_SIZE);
	while (WindowShouldClose() == false)
	{
		if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		{
			Vector2 mousePosition = GetMousePosition();
			int row = mousePosition.y / CELL_SIZE;
			int column = mousePosition.x / CELL_SIZE;
			simulation.ToggleCell(row, column);
		}
		if (IsKeyPressed(KEY_SPACE))
		{
			if (simulation.IsRunning())
			{
				simulation.Stop();
			}
			else
			{
				simulation.Start();
			}
		}
		if (IsKeyPressed(KEY_F))
		{
			FPS += 2;
			SetTargetFPS(FPS);
		}
		if (IsKeyPressed(KEY_S))
		{
			FPS -= 2;
			SetTargetFPS(FPS);
		}
		if (IsKeyPressed(KEY_R))
		{
			simulation.CreateRandomState();
		}
		if (IsKeyPressed(KEY_C))
		{
			simulation.ClearGrid();
		}

		simulation.Update();
		BeginDrawing();
		ClearBackground(GREY);
		simulation.Draw();
		EndDrawing();
	}
	CloseWindow();
}
