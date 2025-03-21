#ifndef CONWAY_H
#define CONWAY_H

#include <windows.h>
#include <stdio.h>

#define WIN_HEIGHT				600
#define WIN_WIDTH				800
#define CELL_SIZE				15
#define BOTTOM_STRIP_HEIGHT		(2 * CELL_SIZE)
#define GRID_WIDTH				(WIN_WIDTH / CELL_SIZE)
#define GRID_HEIGHT				(WIN_HEIGHT / CELL_SIZE)

const int WIN_CENTER				= (WIN_WIDTH / CELL_SIZE) / 2;
const COLORREF BG_COLOR				= RGB(0, 0, 0);			// Black
const COLORREF ALIVE_CELL_COLOR		= RGB(255, 255, 255);	// White
const COLORREF BOTTOM_STRIP_COLOR	= RGB(240, 240, 240);	// Very Light Grey
const COLORREF TEXT_COLOR			= RGB(0, 0, 0);			// Black
const char* TEXT_FONT				= "Cascadia Mono";

void drawAtLocation(HDC hdc, int x, int y);
void aliveDeadRulesDecision(int neighbors[GRID_WIDTH][GRID_HEIGHT], int i, int j);
void countAliveNeighbors(int neighbors[GRID_WIDTH][GRID_HEIGHT]);
void displayBottomStrip(HDC hdc);
void displayText(HDC hdc);
void handleDrawing(HWND hwnd);
void handleTimeTick();
void handleMouseLeftClick(long long lParam);
void displayAliveCells(HDC hdc);

#endif