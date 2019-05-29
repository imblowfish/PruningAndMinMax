#pragma once
#ifndef TREE_BUILDER_H
#define TREE_BUILDER_H
#include <iostream>
#include "Tree.h"
using namespace std;

class TreeBuilder{
private:
	Tree tree;
	unsigned int nowId;
public:
	TreeBuilder(){nowId = 0;}
	bool IsEmpty(){return tree.Empty();}
	Tree& GetTree(){return tree;}
	vector<int>* GetLevelsInfo(){return tree.GetLevelsInfo();}
	void LoadTree(string filepath){
		tree.DeleteNode(0);
		nowId = 0;
		string str;
		ifstream file;
		vector<int> nodeInfo;
		file.open(filepath.c_str());
		if(!file.is_open()) return;
		getline(file, str);
		tree.SetFirstPlayer(atoi(str.c_str()));
		while(!file.eof()){
			nodeInfo.clear();
			for(unsigned int i = 0; i < 5; i++){
				getline(file, str);
				nodeInfo.push_back(atoi(str.c_str()));
			}
			if(file.eof()) continue;
			tree.LoadNode(&nodeInfo, &nowId);
		}
		file.close();
		tree.CalculateLevelsInfo();
	}
	void ClearTree(){
		tree.DeleteNode(0);
		nowId = 0;
	}
	void SetFirstPlayer(){tree.SetFirstPlayer();}
	void SetData(int idx, int data){tree.SetData(idx, data);}
};

#endif