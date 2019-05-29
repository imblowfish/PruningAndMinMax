#include "pch.h"
#include "EditBox.h"

void EditBox::StartEditing(){
	if(!isVisible) return;
	//if(nowEdit) return;
	nowEdit = true;
	SDL_Color save = color;
	color = inputColor;
	value.insert(value.end(), '|');
	InitText(isAlpha);
	color = save;
	SDL_StartTextInput();
}

void EditBox::KeyboardListener(char symbol, int val){
	if(!isVisible) return;
	if(!nowEdit) return;
	int pos = value.find('|');
	if(pos >= 0) value.erase(pos, 1);
	else return;
	if(val == SDLK_RETURN){
		nowEdit = false;
		if(value.empty() || value[0] == '0') value = defaultVal;
		InitText(isAlpha);
		SDL_StopTextInput();
		return;
	}
	if(val == SDLK_LEFT){
		if(pos > 0) pos--;
	}
	else if(val == SDLK_RIGHT){
		if(pos < value.size()) pos++;
	}
	else if(val == SDLK_BACKSPACE){
		if(pos > 0) value.erase(--pos, 1);
	}
	else if(symbol >= '0' && symbol <= '9' && value.size() < MAX_STR_SIZE){
		value.insert(value.begin() + pos++, symbol);
	}	
	else if(symbol == '-' && pos == 0 && value.find('-') == string::npos && value.size() < MAX_STR_SIZE)
		value.insert(value.begin() + pos++, symbol);
	value.insert(value.begin() + pos, '|');
	SDL_Color save = color;
	color = inputColor;
	InitText(isAlpha);
	color = save;
}

void EditBox::Draw(){
	if(!isVisible) return;
	if(!font || !texture) return;
	if(!nowEdit) SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	else SDL_SetRenderDrawColor(renderer, inputColor.r, inputColor.g, inputColor.b, inputColor.a);
	SDL_RenderFillRect(renderer, &bgRect);										//вывод фона
	SDL_RenderCopy(renderer, texture, NULL, &textRect);							//вывод текста
	SDL_SetRenderDrawColor(renderer, frame.r, frame.g, frame.b, frame.a);
	SDL_RenderDrawRect(renderer, &bgRect);										//вывод рамки
}