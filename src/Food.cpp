/*
 * Food.cpp
 *
 *  Created on: Jul 5, 2020
 *      Author: seba
 */

#include <iostream>
#include <iomanip>
#include <map>
#include "Food.h"
#include "Screen.h"

using namespace std;

map<string, Food*> food_map;
map<string, Food*> alias_map;

Food::Food(string name, string alias, string unit, float serving, float kcal, float C, float F, float P){
	this->name = name;
	this->alias = alias;
	this->unit = unit;
	this->serving = serving;
	this->C = C;
	this->F = F;
	this->P = P;
	this->kcal = ((C + P) * 4) + F * 9;
	if(kcal) this->kcal = kcal;
	food_map.insert( {name, this});
	if(alias != EMPTY_ALIAS) alias_map.insert( {alias, this});
}

void Food::print(){
	cout << "Food:[" << this->name << "]" << ENDL;
	cout << TAB << setw(15) << "alias: ";
	if(!this->alias.empty()) cout << "(empty)" << ENDL;
	else cout << this->alias << ENDL;
	cout << TAB << setw(15) << "unit:[" << this->unit << "]" << ENDL;
	cout << TAB << setw(15) << "C:" << this->C << ENDL;
	cout << TAB << setw(15) << "F:" << this->F << ENDL;
	cout << TAB << setw(15) << "P:" << this->P << ENDL;
	cout << TAB << setw(15) << "kcal:" << this->kcal << ENDL;
	cout << TAB << setw(15) << "serving:" << this->serving << ENDL;
}

Food* find_food(string foodname){
	map<string, Food*>::iterator i = food_map.find(foodname);
	if(i == food_map.end()) return nullptr;
	return i->second;
}
