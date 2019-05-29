#pragma once
#ifndef PRUNING_H
#define PRUNING_H
#include "Tree.h"
using namespace std;

//отсечения

class Pruning{
private:
	int firstPlayer;
	int GetPlayer(Node &n){
		if(n.lvl % 2 == 0) return firstPlayer;
		return !firstPlayer;
	}
	void CalculateValue(Node &n){
		if(n.childs.empty()) return;
		int nowPlayer = GetPlayer(n);
		for(unsigned int i = 0; i < n.childs.size(); i++){
			if(nowPlayer == MAX_PLAYER){
				n.childs[i]->alpha = n.alpha;
				n.childs[i]->deepAlpha = n.deepAlpha;
				n.childs[i]->deepBetta = n.betta;
				CalculateValue(*n.childs[i]);
				if(n.childs[i]->data > n.alpha) n.data = n.alpha = n.childs[i]->data;
				if(n.childs[i]->data > n.betta){
					n.childs[i]->pruningType = BETTA;
					return;
				}else if(n.childs[i]->data > n.deepBetta){
					n.childs[i]->pruningType = DEEP_BETTA;
					return;
				}
			} else{
				n.childs[i]->betta = n.betta;
				n.childs[i]->deepBetta = n.deepBetta;
				n.childs[i]->deepAlpha = n.alpha;
				CalculateValue(*n.childs[i]);
				if(n.childs[i]->data < n.betta) n.data = n.betta = n.childs[i]->data;
				if(n.childs[i]->data < n.alpha){
					n.childs[i]->pruningType = ALPHA;
					return;
				}else if(n.childs[i]->data < n.deepAlpha){
					n.childs[i]->pruningType = DEEP_ALPHA;
					return;
				} 
			}
		}
		return;
	}
public:
	void PruningProcedure(Tree &tree){
		if(tree.Empty()) return;
		tree.ResetPruning();
		firstPlayer = tree.GetFirstPlayer();
		CalculateValue(*tree.root);
	}
};

#endif