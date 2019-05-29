#pragma once
#ifndef TREE_H
#define TREE_H
#include <iostream>
#include "FileWriter.h"
using namespace std;
enum playerType{
	MAX_PLAYER, MIN_PLAYER
};
enum pruningTypes{
	NONE, ALPHA, BETTA, DEEP_ALPHA, DEEP_BETTA
};
struct Node{
	int data,
		id,
		lvl,
		posX,
		posY,
		pruningType,
		alpha, betta, deepAlpha, deepBetta;
	bool edited;
	vector<Node*> childs;
	Node *parent;
	Node(){
		parent = nullptr;
		id = lvl = -1;
		data = posX = posY = 0;
		alpha = deepAlpha = INT_MIN;
		betta = deepBetta = INT_MAX;
		edited = false;
		pruningType = NONE;
	}
};
class Tree: public FileWriter{
private:
	int whoFirst = -1;
	vector<int> levelsInfo;
	//очистка дерева
	void ClearTree(){
		ClearNode(root);
	}
	//рекурсивная очистка нода, соседей и child
	void ClearNode(Node *n){
		if(!n) return;
		if(!n->childs.empty()){
			for(unsigned int i=0;i<n->childs.size();i++) ClearNode(n->childs[i]);
			n->childs.clear();
		}
		delete n;
		n = nullptr;
	}
	void ClearNodeById(int id){
		Node *tmp = FindNodeById(id);
		if(!tmp) return;
		if(!tmp->childs.empty()){
			for(unsigned int i = 0; i < tmp->childs.size(); i++) ClearNode(tmp->childs[i]);
		}
		delete tmp;
		tmp = nullptr;
	}
	//поиск ноды по идентификатору
	Node* FindNodeById(Node *n, unsigned int id){
		if(!n) return nullptr;
		if(n->id == id) return n;
		if(!n->childs.empty()){
			for(unsigned int i=0;i<n->childs.size(); i++){
				Node *foundNode = FindNodeById(n->childs[i], id);
				if(foundNode) return foundNode;
			}
		}
		return nullptr;
	}
	void CalculateLevels(Node *n, vector<int> *arr){
		if(!n) return;
		if(n->lvl >= (*arr).size()) (*arr).push_back(0);
		if(!n->childs.empty()){
			for(unsigned int i = 0; i < n->childs.size(); i++){
				CalculateLevels(n->childs[i], arr);
			}
			(*arr)[(*n).lvl]++;
		}
	}
	void PruningToDefault(Node *n){
		if(!n) return;
		if(!n->childs.empty()) n->data = 0;
		n->pruningType = NONE;
		n->alpha = n->deepAlpha = INT_MIN;
		n->betta = n->deepBetta = INT_MAX;
		if(!n->childs.empty()){
			for(unsigned int i = 0; i < n->childs.size(); i++){
				PruningToDefault(n->childs[i]);
			}
		}
	}
public:
	Node *root;
	Tree(){root = nullptr;}
	~Tree(){ClearTree();}
	bool Empty(){return root == nullptr;}
	void AddRoot(unsigned int id, int data=0){
		if(!root){
			root = new Node();
			root->data = data;
			root->id = id;
			root->lvl = 0;
			return;
		}
	}
	bool AddChildNode(unsigned int parentId, unsigned int id, int data=0){
		Node *tmp = FindNodeById(root, parentId);
		if(!tmp){
			return false;
		}
		if(FindNodeById(tmp, id)) return true;
		Node *child = new Node();
		child->id = id;
		child->data = data;
		child->lvl = tmp->lvl + 1;
		child->parent = tmp;
		tmp->childs.push_back(child);
		return true;
	}
	Node* FindNodeById(unsigned int id){return FindNodeById(root, id);}
	void CalculateLevelsInfo(){
		levelsInfo.clear();
		CalculateLevels(root, &levelsInfo);
	}
	//возвращает вектор размеров с глубину дерева и количество нодов на каждом уровне
	vector<int>* GetLevelsInfo(){
		if(levelsInfo.empty()) return nullptr;
		return &levelsInfo;
		return nullptr;
	}
	Node* GetRoot(){return root;}
	void LoadNode(vector<int> *nodeInfo, unsigned int *maxId){
		enum infoSelector {PARENT, ID, DATA, POSX, POSY};
		if(*maxId <= (*nodeInfo)[ID])
			*maxId = (*nodeInfo)[ID]+1;
		Node *tmp;
		if(Empty()){
			AddRoot((*nodeInfo)[ID]);
			root->posX = (*nodeInfo)[POSX];
			root->posY = (*nodeInfo)[POSY];
			root->edited = true;
			return;
		}
		if((*nodeInfo)[PARENT] >= 0) AddChildNode((*nodeInfo)[PARENT], (*nodeInfo)[ID]);
		tmp = FindNodeById((*nodeInfo)[ID]);
		if(tmp->childs.empty()) tmp->data = (*nodeInfo)[DATA];
		tmp->posX = (*nodeInfo)[POSX];
		tmp->posY = (*nodeInfo)[POSY];
		tmp->edited = true;
	}
	void SetFirstPlayer(int val=-2){
		if(val == -2){
			if(whoFirst == MAX_PLAYER || whoFirst < 0) whoFirst = MIN_PLAYER;
			else if(whoFirst == MIN_PLAYER) whoFirst = MAX_PLAYER;
		} else whoFirst = val;
	}
	int GetFirstPlayer(){return whoFirst;}
	void DeleteNode(unsigned int id){
		if(Empty())	return;
		ClearNodeById(id);
		if(id == 0) root = nullptr;
	}
	void SetData(int idx, int data){
		Node *tmp = FindNodeById(idx);
		tmp->data = data;
	}
	void ResetPruning(){PruningToDefault(root);}
};

#endif