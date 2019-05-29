#include "pch.h"
#include <iostream>
#include "Main.h"


int main(int argc, char *argv[]){
	if(!initSDL("Lab3")){
		getchar();
		return -1;
	} else cout << "initSDL() success" << endl;
	MainMenu menu("MainMenu", true);
	menu.LinkRenderer(renderer);
	menu.InitMenu();
	menu.MainLoop();
	return 0;
}

