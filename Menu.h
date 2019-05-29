#pragma once
#ifndef MENU_H
#define MENU_H
#include <iostream>
#include <vector>
#include "Colors.h"
#include "Button.h"
using namespace std;

class Menu{
protected:
	enum MenuMode{MAIN_MENU};
	SDL_Renderer *renderer = nullptr;
	string menuName = "";
	bool active;
	vector<Button> buttons;
public:
	Menu(){}
	Menu(string name, bool isRunned){
		menuName = name;
		active = isRunned;
	}
	void LinkRenderer(SDL_Renderer *renderer){this->renderer = renderer;}
	void ClearScreen(){
		SDL_SetRenderDrawColor(renderer, BG_COLOR, 255);
		SDL_RenderClear(renderer);
	}
	virtual void Redraw() = 0;
	virtual void InitMenu() = 0;
};

#endif