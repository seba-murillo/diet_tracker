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
	food_map.insert({name, this});
	if(alias != EMPTY_ALIAS) alias_map.insert( {alias, this});
}

void Food::print(){
	cout << fixed;
	cout.precision(1);
	cout << BOLD "- nutritional info of '" << this->name << "':" ENDL;
	if(this->alias != EMPTY_ALIAS){
		cout << FORMAT_TAG << left << setw(10) << "alias:";
		cout << FORMAT_DATA << right << setw(15) << this->alias << ENDL;
	}
	cout << FORMAT_TAG << left << setw(10) << "unit:" << FORMAT_DATA << right << setw(15) << this->unit << ENDL;
	cout << FORMAT_TAG << left << setw(10) << "serving:" << FORMAT_DATA << right << setw(15) << this->serving << ENDL;
	cout << FORMAT_TAG << left << setw(10) << "kcal:" << FORMAT_DATA << right << setw(15) << this->kcal << ENDL;
	cout << FORMAT_TAG << left << setw(10) << "C:" << FORMAT_DATA << right << setw(15) << this->C << ENDL;
	cout << FORMAT_TAG << left << setw(10) << "F:" << FORMAT_DATA << right << setw(15) << this->F << ENDL;
	cout << FORMAT_TAG << left << setw(10) << "P:" << FORMAT_DATA << right << setw(15) << this->P << ENDL;
}

Food* Food::find_food(string foodname){
	map<string, Food*>::iterator i = food_map.find(foodname);
	if(i == food_map.end()){
		return find_by_alias(foodname);
	}
	return i->second;
}

Food* Food::find_by_alias(string foodname){
	map<string, Food*>::iterator i = alias_map.find(foodname);
	if(i == alias_map.end()) return nullptr;
	return i->second;
}

void Food::print_all(){
	for(auto food : food_map){
		food.second->print();
	}
}
