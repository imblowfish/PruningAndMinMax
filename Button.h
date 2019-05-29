#pragma once
#ifndef BUTTON_H
#define BUTTON_H
#include "ScreenElement.h"
//класс кнопки

class Button: public ScreenElement{
public:
	Button():ScreenElement(){};
	void FeedbackOnMouse(SDL_Event e){
		if(!isVisible) return;
		if(e.button.button != SDL_BUTTON_LEFT) return;
		if(e.type == SDL_MOUSEBUTTONDOWN){
			SDL_Color save = color;
			color = inputColor;
			isSelected = true;
			//InitText(isAlpha);
			color = save;
		} else{
			//InitText(isAlpha);
			isSelected = false;
		}
	}
	void Draw() override{
		if(!isVisible) return;
		if(!font || !texture) return;
		if(!isSelected) SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		else SDL_SetRenderDrawColor(renderer, inputColor.r, inputColor.g, inputColor.b, inputColor.a);
		SDL_RenderFillRect(renderer, &bgRect);										//вывод фона
		SDL_RenderCopy(renderer, texture, NULL, &textRect);							//вывод текста
		SDL_SetRenderDrawColor(renderer, frame.r, frame.g, frame.b, frame.a);
		SDL_RenderDrawRect(renderer, &bgRect);										//вывод рамки
	}
};
#endif