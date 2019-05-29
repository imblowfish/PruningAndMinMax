#pragma once
#ifndef EDITBOX_H
#define EDITBOX_H
#include "ScreenElement.h"

class EditBox: public ScreenElement{
private:
	const int MAX_STR_SIZE = 3;
	string defaultVal = "0";
	bool nowEdit;
public:
	EditBox():ScreenElement(){nowEdit = false;};
	void SetEdit(){nowEdit = !nowEdit;}
	bool GetEdit(){return nowEdit;}
	void FeedbackOnMouse(SDL_Event e){StartEditing();}
	void StartEditing();
	void KeyboardListener(char symbol, int val = -100);
	void Draw() override;
};

#endif