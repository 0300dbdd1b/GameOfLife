#include "simulation.hpp"

extern const double CELL_SIZE;
extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;

int ComputeNewCellSize(Simulation &simulation, int cellsize, int n);
void KeybindChecks(Simulation &simulation, double &renderTreshold, double &CELL_SIZE);
