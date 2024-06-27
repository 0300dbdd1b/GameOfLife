#include <raylib.h>
#include "include/MainApp.hpp"

int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 800;

int main()
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "GameOfLife");
	
	MainApp app = MainApp();
	app.MainLoop();
}

