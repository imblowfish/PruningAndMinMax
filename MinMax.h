#pragma once
#ifndef MIN_MAX_H
#define MIN_MAX_H
#include "Tree.h"
using namespace std;
//процедура minmax

class MinMax{
private:
	const int MAX_VAL = INT_MAX;
	const int MIN_VAL = INT_MIN;
	int firstPlayer;
	int GetPlayer(Node &n){
		if(n.lvl % 2 == 0) return firstPlayer;
		return !firstPlayer;
	}
	int CalculateValue(Node &n){
		if(n.childs.empty()) return n.data;
		vector<int> values;
		for(unsigned int i = 0; i < n.childs.size(); i++)
			values.push_back(CalculateValue(*n.childs[i]));
		if(GetPlayer(n) == MAX_PLAYER) return n.data = FindMax(&values);
		else return n.data = FindMin(&values);
	}
	int FindMax(vector<int> *arr){
		int val = MIN_VAL;
		for(unsigned int i = 0; i < (*arr).size(); i++){
			if(val < (*arr)[i]) val = (*arr)[i];
		}
		(*arr).clear();
		return val;
	}
	int FindMin(vector<int> *arr){
		int val = MAX_VAL;
		for(unsigned int i = 0; i < (*arr).size(); i++){
			if(val > (*arr)[i]) val = (*arr)[i];
		}
		(*arr).clear();
		return val;
	}
public:
	void MinMaxProcedure(Tree &tree){
		if(tree.Empty()) return;
		firstPlayer = tree.GetFirstPlayer();
		CalculateValue(*tree.root);
	}
};

#endif