#include "raylib.h"
#include "simulation.hpp"

extern float CELL_SIZE;
extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
extern int BASE_NCELL;
extern const double RENDERING_INCREASE;
float ComputeNewCellSize(Simulation &simulation, float cellsize, int n);
void KeybindChecks(Simulation &simulation, Camera2D &camera, float &renderTreshold, float &CELL_SIZE);
