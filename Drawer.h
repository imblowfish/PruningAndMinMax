#pragma once
#include <iostream>
#include "Colors.h"
using namespace std;

class Drawer{
private:
	SDL_Renderer *renderer;
public:
	~Drawer(){renderer = nullptr;}
	void LinkRenderer(SDL_Renderer *renderer){this->renderer = renderer;}
	void DrawLine(int x1, int y1, int x2, int y2, SDL_Color lineColor);
	void DrawEmptyCircle(int xPos, int yPos, int radius, SDL_Color lineColor);
	void DrawFilledCircle(int xPos, int yPos, int radius, SDL_Color fillColor);
};