#pragma once
#ifndef TREE_VIEWER_H
#define TREE_VIEWER_H
#include <iostream>
#include "Colors.h"
#include "SDL.h"
#include "Tree.h"
#include "Drawer.h"
#include "EditBox.h"
#include "Textfield.h"
using namespace std;

enum ViewerMode{EDIT_MODE, MINMAX_MODE, PRUNING_MODE};

class TreeViewer{
private:
	SDL_Renderer *renderer;
	EditBox editBox;
	Drawer drawer;
	int nodeRadius = 10;
	int SCREEN_WIDTH, SCREEN_HEIGHT;
	int offsetY, verticalMax, nowSelected;
	bool nowEdit;
	bool showId = false;
	int editorMode;
	vector<int> *levelInfo, count;
	vector<Node> nodes, bottomNodes;
	TextField textfield;
	void ConnectNodes(Node *parent, Node *child, int pruningType = NONE){
		SDL_Color lineColor;
		if(editorMode == PRUNING_MODE && pruningType != NONE){
			if(pruningType == ALPHA) lineColor = {RED_COLOR};
			else if(pruningType == BETTA) lineColor = {GREEN_COLOR};
			else if(pruningType == DEEP_BETTA) lineColor = {YELLOW_COLOR};
			else lineColor = {PURPLE_COLOR};
		} else lineColor = {BLUE_COLOR};
		drawer.DrawLine(parent->posX, parent->posY, child->posX, child->posY, lineColor);
	}
	void DrawNode(int x, int y, bool selected = false){
		SDL_Color ellipseColor;
		if(!selected) ellipseColor = {BLUE_COLOR};
		else ellipseColor = {RED_COLOR};
		drawer.DrawFilledCircle(x, y, nodeRadius, ellipseColor);
	}
	void DrawTree(Node &n){
		if(!n.childs.empty()){
			nodes.push_back(n);
			int pruningType = NONE;
			for(unsigned int i = 0; i < n.childs.size(); i++){
				ConnectNodes(&n, n.childs[i], pruningType);
				if(n.childs[i]->pruningType != NONE) pruningType = n.childs[i]->pruningType;
				DrawTree(*n.childs[i]);
			}
		}else bottomNodes.push_back(n);
		if(n.id == nowSelected) DrawNode(n.posX, n.posY, true);
		else DrawNode(n.posX, n.posY);
	}
	void DrawLevels(int firstPlayer, int levels){
		textfield.Clear();
		int x = 15, y;
		for(unsigned int i = 0; i < levels; i++){
			if(i > 0) y = int(offsetY * (i + 1));
			else y = int(offsetY);
			drawer.DrawLine(0, y, SCREEN_WIDTH, y, {GREY_COLOR});
			string val;
			if(firstPlayer >= 0){
				if(firstPlayer == MAX_PLAYER) val = "max";
				else if(firstPlayer == MIN_PLAYER) val = "min";
				else val = "Nan";
				firstPlayer = !firstPlayer;
			}
			textfield.LinkRenderer(renderer);
			textfield.SetupMe(val, x, y, 15, {BG_COLOR}, {BG_COLOR}, {BG_COLOR}, {FONT_COLOR});
			textfield.Draw();
		}
	}
	void DrawID(int posX, int posY, int val){
		textfield.Clear();
		textfield.LinkRenderer(renderer);
		textfield.SetupMe(to_string(val), posX, posY-20, 10,
						  {RED_COLOR}, {BG_COLOR}, {BG_COLOR}, {BLACK_COLOR}, true);
		textfield.Draw();
		textfield.Clear();
	}
	void DrawNodesValues(vector<Node> *nodeArr = nullptr){
		if(!nodeArr) return;
		int x, y;
		for(unsigned int i = 0; i < (*nodeArr).size(); i++){
			if(showId) DrawID((*nodeArr)[i].posX, (*nodeArr)[i].posY, (*nodeArr)[i].id);
			x = (*nodeArr)[i].posX;
			y = (*nodeArr)[i].posY;
			textfield.LinkRenderer(renderer);
			if((*nodeArr)[i].id == nowSelected)
				textfield.SetupMe(to_string((*nodeArr)[i].data), x, y, 12,
								  {RED_COLOR}, {BG_COLOR}, {BG_COLOR}, {WHITE_COLOR}, true);
			else
				textfield.SetupMe(to_string((*nodeArr)[i].data), x, y, 12,
								  {BLUE_COLOR}, {BG_COLOR}, {BG_COLOR}, {WHITE_COLOR}, true);
			textfield.Draw();
		}
		textfield.Clear();
	}
	void DrawLegend(){
		textfield.Clear();
		textfield.LinkRenderer(renderer);
		drawer.DrawLine(SCREEN_WIDTH - 100, 20, SCREEN_WIDTH - 10, 20, {RED_COLOR});
		textfield.SetupMe("alpha", SCREEN_WIDTH - 150, 20, 10,
						  {RED_COLOR}, {BG_COLOR}, {BG_COLOR}, {BLACK_COLOR}, true);
		textfield.Draw();
		drawer.DrawLine(SCREEN_WIDTH - 100, 40, SCREEN_WIDTH - 10, 40, {GREEN_COLOR});
		textfield.SetupMe("betta", SCREEN_WIDTH - 150, 40, 10,
						  {RED_COLOR}, {BG_COLOR}, {BG_COLOR}, {BLACK_COLOR}, true);
		textfield.Draw();
		drawer.DrawLine(SCREEN_WIDTH - 100, 60, SCREEN_WIDTH - 10, 60, {PURPLE_COLOR});
		textfield.SetupMe("deep alpha", SCREEN_WIDTH - 150, 60, 10,
						  {RED_COLOR}, {BG_COLOR}, {BG_COLOR}, {BLACK_COLOR}, true);
		textfield.Draw();
		textfield.Clear();
	}
public:
	TreeViewer(){
		editorMode = EDIT_MODE;
	}
	~TreeViewer(){
		nodes.clear();
		bottomNodes.clear();
	}
	void LinkRenderer(SDL_Renderer *renderer){
		this->renderer = renderer; 
		drawer.LinkRenderer(renderer);
	}
	void Setup(int width, int height){
		SCREEN_WIDTH = width;
		SCREEN_HEIGHT = height;
		nowSelected = -1;
	}
	void Draw(Tree &tree, vector<int> *levelInfo){
		if(tree.Empty()) return;
		this->levelInfo = levelInfo;
		count.clear();
		verticalMax = (*levelInfo).size();
		for(unsigned int i = 0; i < (*levelInfo).size(); i++) count.push_back(1);
		bottomNodes.clear();
		nodes.clear();
		offsetY = int(SCREEN_HEIGHT / (verticalMax + 1));
		DrawLevels(tree.GetFirstPlayer(), (*levelInfo).size());
		DrawTree(*tree.root);
		if(editorMode != EDIT_MODE){
			if(editorMode == PRUNING_MODE) DrawLegend();
			DrawNodesValues(&nodes);
		}
		DrawNodesValues(&bottomNodes);
		if(nowEdit) editBox.Draw();
	}
	int ClickedOnBottomNode(int mouseX, int mouseY){
		int maxPixelDiffX = 10, maxPixelDiffY = 10;
		for(unsigned int i = 0; i < bottomNodes.size(); i++)
			if(fabs(bottomNodes[i].posX - mouseX) < maxPixelDiffX && fabs(bottomNodes[i].posY - mouseY) < maxPixelDiffY)
				return nowSelected = bottomNodes[i].id;
		return -1;
	}
	int GetNowSelected(){return nowSelected;}
	int GetVal(){return editBox.GetIntValue();}
	void EndInput(){
		editBox.Clear();
		nowEdit = false;
	}
	void InputInBox(char symbol, int val = -100){
		if(!nowEdit) return;
		editBox.KeyboardListener(symbol, val);
	}
	void StartEdit(){
		if(nowSelected < 0) return;
		int id = -1;
		editorMode = EDIT_MODE;
		for(unsigned int i = 0; i < bottomNodes.size(); i++)
			if(nowSelected == bottomNodes[i].id) id = i;
		if(id < 0) return;
		editBox.Clear();
		editBox.LinkRenderer(renderer);
		editBox.SetupMe(to_string(bottomNodes[id].data), 
						bottomNodes[id].posX, bottomNodes[id].posY + 30, 10, 
					   {EDITBOX_COLOR}, {EDITBOX_FRAME_COLOR}, {EDITBOX_INPUT_COLOR}, {EDITBOX_FONT_COLOR});
		nowEdit = true;
		editBox.StartEditing();
	}
	void SetEditorMode(int val){
		if(val > PRUNING_MODE) return;
		editorMode = val;
	}
};

#endif