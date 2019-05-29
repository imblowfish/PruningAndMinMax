#include "pch.h"
#include "ScreenElement.h"

ScreenElement::ScreenElement(){
	value = "";
	xPos = yPos = 0;
	color = frame = inputColor = fontColor = {BG_COLOR};
	font = nullptr;
	texture = nullptr;
	isVisible = true;
	isSelected = false;
}

void ScreenElement::InitText(bool alphaBackground){
	if(texture){
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}
	SDL_Surface *surface = nullptr;
	/*if(alphaBackground)*/	surface = TTF_RenderText_Blended(font, value.c_str(), fontColor);
	//else surface = TTF_RenderText_Shaded(font, value.c_str(), fontColor, color);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	//подгоняем размер под прямоугольник
	int fontWidth = 0, fontHeight = 0;
	SDL_QueryTexture(texture, NULL, NULL, &fontWidth, &fontHeight);
	//текстовое поле
	textRect = {xPos, yPos, fontWidth, fontHeight};
	textRect.x -= int(textRect.w / 2);
	textRect.y -= int(textRect.h / 2);
	if(!texture) return;
	if(!alphaBackground){
		bgRect.x = textRect.x - 20;
		bgRect.y = textRect.y - 5;
		bgRect.w = textRect.w + 40;
		bgRect.h = textRect.h + 10;
	}
	SDL_FreeSurface(surface);
	isAlpha = alphaBackground ? true : false;
}

void ScreenElement::SetupMe(string val, int x, int y, unsigned int fontSize, 
							SDL_Color color = {BG_COLOR}, SDL_Color frameColor = {BG_COLOR},
							SDL_Color inputColor = {BG_COLOR}, SDL_Color fontColor = {FONT_COLOR},
							bool alphaBackground){
	this->Clear();
	value = val;
	xPos = x; yPos = y;
	this->color = color; this->frame = frameColor;
	this->inputColor = inputColor; this->fontColor = fontColor;
	if(font) fBuilder.CloseFont(font);
	font = fBuilder.CreateFont(font, fontSize);
	isSelected = false;
	isVisible = true;
	this->fontSize = fontSize;
	isAlpha = alphaBackground;
	InitText(alphaBackground);
}

void ScreenElement::Draw(){
	if(!isVisible) return;
	if(!font || !texture) return;
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	if(!isAlpha) SDL_RenderFillRect(renderer, &bgRect);										//вывод фона
	SDL_RenderCopy(renderer, texture, NULL, &textRect);							//вывод текста
	SDL_SetRenderDrawColor(renderer, frame.r, frame.g, frame.b, frame.a);
	if(!isAlpha) SDL_RenderDrawRect(renderer, &bgRect);										//вывод рамки
}

bool ScreenElement::MouseEventListener(SDL_Event e, int x, int y){
	if(!isVisible) return false;
	int left = bgRect.x,
		right = bgRect.x + bgRect.w,
		top = bgRect.y,
		bottom = bgRect.y + bgRect.h;
	if(x < left || x > right || y < top || y > bottom) return false;
	FeedbackOnMouse(e);
	return true;
}