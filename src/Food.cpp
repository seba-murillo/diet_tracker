/*
 * Food.cpp
 *
 *  Created on: Jul 5, 2020
 *      Author: seba
 */

#include <iostream>
#include <iomanip>
#include "Food.h"
#include "Screen.h"

namespace std {
	list<Food*> food_list;
	unordered_map<string, Food*> alias_map;

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
		food_list.push_back(this);
		if(alias != EMPTY_ALIAS) alias_map.insert({alias, this});
	}

	Food::~Food(){
		//
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

}/* namespace std */
