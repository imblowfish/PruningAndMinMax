#pragma once
#ifndef SCREEN_ELEMENT_H
#define SCREEN_ELEMENT_H
#include <iostream>
#include <string>
#include "FontBuilder.h"
#include "Colors.h"
using namespace std;

class ScreenElement{
protected:
	int maxPixelDiff = 5;		//максимальное отклонение в пикселях для определения нажатия мышкой
	int fontSize = 0;
	bool isAlpha = false;
	FontBuilder fBuilder;		//класс создания шрифта
	SDL_Renderer *renderer;		//указатель на рендерер
	SDL_Texture *texture;		//текстура с текстом
	string value;				//текстовое значение элемента(что будет выводить)
	int xPos, yPos;				//положение на экране
	SDL_Color color,			//цвет поля
			  frame,			//цвет рамки
			  inputColor,		//цвет, когда производится ввод в поле
			  fontColor;		//цвет шрифта
	SDL_Rect bgRect, textRect;	//прямоугольник фона и текста
	TTF_Font *font;				//шрифт кнопки
	bool isSelected, isVisible;	//нажат(выбран) элемент, видим ли элемент
	//инициализация шрифта и текста
	void InitText(bool alphaBackground);
public:
	ScreenElement();
	~ScreenElement(){/*Clear();*/}
	void Clear(){
		if(font) fBuilder.CloseFont(font);
		font = nullptr;
		SDL_DestroyTexture(texture);
	}
	//настройка элемента уже после вызова конструктора
	void SetupMe(string val, int x, int y, unsigned int fontSize, SDL_Color color, SDL_Color frameColor, SDL_Color inputColor, SDL_Color fontColor, bool alphaBackground = false);
	void LinkRenderer(SDL_Renderer *renderer){this->renderer = renderer;}
	void SetVisibility(bool val){isVisible = val;}
	bool GetVisibility(){return isVisible;}
	bool MouseEventListener(SDL_Event e, int x, int y);
	int GetIntValue(){
		return atoi(value.c_str());
	}
	void SetNewVal(string val){
		value = val;
		InitText(isAlpha);
	}
	void SetNewValAndPos(string val, int x, int y){
		this->value = val;
		this->xPos = x;
		this->yPos = y;
	}
	string GetStrValue(){
		return value;
	}
	virtual void FeedbackOnMouse(SDL_Event e) = 0;
	virtual void Draw();
};

#endif