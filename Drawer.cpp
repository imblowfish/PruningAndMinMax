#include "pch.h"
#include "Drawer.h"

void Drawer::DrawLine(int x1, int y1, int x2, int y2, SDL_Color lineColor){
	SDL_SetRenderDrawColor(renderer, lineColor.r, lineColor.g, lineColor.b, lineColor.a);
	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}
void Drawer::DrawEmptyCircle(int xPos, int yPos, int radius, SDL_Color lineColor){
	SDL_SetRenderDrawColor(renderer, lineColor.r, lineColor.g, lineColor.b, 255);
	int x = radius - 1;
	int y = 0;
	int dx = 1;
	int dy = 1;
	int err = dx - (radius << 1);
	while(x >= y){
		SDL_RenderDrawPoint(renderer, xPos + x, yPos + y);
		SDL_RenderDrawPoint(renderer, xPos + y, yPos + x);
		SDL_RenderDrawPoint(renderer, xPos - y, yPos + x);
		SDL_RenderDrawPoint(renderer, xPos - x, yPos + y);
		SDL_RenderDrawPoint(renderer, xPos - x, yPos - y);
		SDL_RenderDrawPoint(renderer, xPos - y, yPos - x);
		SDL_RenderDrawPoint(renderer, xPos + y, yPos - x);
		SDL_RenderDrawPoint(renderer, xPos + x, yPos - y);
		if(err <= 0){
			y++;
			err += dy;
			dy += 2;
		}
		if(err > 0){
			x--;
			dx += 2;
			err += dx - (radius << 1);
		}
	}
}
void Drawer::DrawFilledCircle(int xPos, int yPos, int radius, SDL_Color fillColor){
	SDL_SetRenderDrawColor(renderer, fillColor.r, fillColor.g, fillColor.b, 255);
	int xOffset = 0;
	int yOffset = radius;
	int balance = -radius;
	while(xOffset <= yOffset){
		int p0 = xPos - xOffset;
		int p1 = xPos - yOffset;

		int w0 = xOffset + xOffset;
		int w1 = yOffset + yOffset;

		for(unsigned int i = 0; i < w0; i++){
			SDL_RenderDrawPoint(renderer, p0 + i, yPos + yOffset);
			SDL_RenderDrawPoint(renderer, p0 + i, yPos - yOffset);
		}
		for(unsigned int i = 0; i < w1; i++){
			SDL_RenderDrawPoint(renderer, p1 + i, yPos + xOffset);
			SDL_RenderDrawPoint(renderer, p1 + i, yPos - xOffset);
		}
		if((balance += xOffset++ + xOffset) >= 0) balance -= --yOffset + yOffset;
	}
}