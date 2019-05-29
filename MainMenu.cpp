#include "pch.h"
#include "MainMenu.h"

void MainMenu::InitMenu(){
	tViewer.LinkRenderer(renderer);
	tViewer.Setup(SCREEN_WIDTH, SCREEN_HEIGHT);
	tBuilder.LoadTree(pathToSave);
	//tBuilder.DeleteNode(1);
	//getchar();
	int shiftY = 40, shiftX = 100;			//сдвиг каждой кнопки относительно предыдущей по Y
	int scrElX = 60, scrElY = 30;			//начальная пощиция по x и y
	int fontSize = 12;
	for(unsigned int i = 0; i < buttonNames.size(); i++){
		buttons.push_back(Button());
		buttons[i].LinkRenderer(renderer);
		buttons[i].SetupMe(buttonNames[i], scrElX + i *shiftX, scrElY, fontSize, 
						   {BTN_COLOR}, {BTN_FRAME_COLOR}, {BTN_INPUT_COLOR}, {FONT_COLOR});
	}
	Redraw();
}

void MainMenu::MainLoop(){
	SDL_Event event;
	SDL_MouseButtonEvent bEvent;
	int mouseX, mouseY;
	bool running = true;
	while(running){
		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_KEYDOWN:
					keys[event.key.keysym.sym] = 1;
					if(keys[SDLK_ESCAPE]) running = false;
					InputInEditBox(event);
					if(keys[SDLK_RETURN]){
						int nowSelIdx = tViewer.GetNowSelected();
						if(nowSelIdx < 0) continue;
						tBuilder.SetData(tViewer.GetNowSelected(), tViewer.GetVal());
						tViewer.EndInput();
					}
					if(keys[SDLK_e]) tViewer.StartEdit();
					Redraw();
				break;
				case SDL_KEYUP:
					keys[event.key.keysym.sym] = 0;
				break;
				case SDL_MOUSEBUTTONUP:
					SDL_GetMouseState(&mouseX, &mouseY);
					dragAndDrop = false;
					bEvent = event.button;
					if(bEvent.button != SDL_BUTTON_LEFT) continue;
					for(unsigned int i = 0; i < buttons.size(); i++)
						if(buttons[i].MouseEventListener(event, mouseX, mouseY)){
							ActionOnButton(buttons[i].GetStrValue());
							Redraw();
							continue;
						}
					if(tViewer.ClickedOnBottomNode(mouseX, mouseY) < 0) continue;
					Redraw();
				break;
				case SDL_MOUSEBUTTONDOWN:
					SDL_GetMouseState(&mouseX, &mouseY);
					bEvent = event.button;
					if(bEvent.button == SDL_BUTTON_LEFT){
						for(unsigned int i = 0; i < buttons.size(); i++)
							if(buttons[i].MouseEventListener(event, mouseX, mouseY)) Redraw();
					}
					if(bEvent.button != SDL_BUTTON_RIGHT) continue;
					if(tViewer.ClickedOnBottomNode(mouseX, mouseY) < 0) continue;
					Redraw();
				break;
				case SDL_QUIT:
					running = false;
				break;
			}
		}
	}
}

void MainMenu::Redraw(){
	ClearScreen();
	for(unsigned int i = 0; i < buttons.size(); i++) buttons[i].Draw();
	tViewer.Draw(tBuilder.GetTree(), tBuilder.GetLevelsInfo());
	SDL_RenderPresent(renderer);
}

void MainMenu::ActionOnButton(string val){
	if(val == buttonNames[SET_PLAYER]){
		if(tBuilder.IsEmpty()) return;
		tViewer.SetEditorMode(EDIT_MODE);
		tBuilder.SetFirstPlayer();
	} else if(val == buttonNames[MIN_MAX]){
		tViewer.SetEditorMode(MINMAX_MODE);
		minMax.MinMaxProcedure(tBuilder.GetTree());
	} else if(val == buttonNames[PRUNING]){
		tViewer.SetEditorMode(PRUNING_MODE);
		pruning.PruningProcedure(tBuilder.GetTree());
	}
}