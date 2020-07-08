/*
 * Day.cpp
 *
 *  Created on: Jul 5, 2020
 *      Author: seba
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include "Screen.h"
#include "Day.h"

using namespace std;

void Day::load(){
	string filename = to_string(date.year) + "_" + to_string(date.month) + "_" + to_string(date.day);
	ifstream file;
	file.open(filename, ios::in);
	if(!file.is_open()) return;
	string line, name;
	us amount;
	while(getline(file, line)){
		name = extract(line, "\"", "\"");
		amount = stoi(extract(line, ": ", ","));
		Food* food = find_food(name);
		if(food == nullptr){
			cout << TAB << TAB COLOR_SYNTAX << "WARNING: '" << name << "' (" << amount << ") not found in database, skipping..." << ENDL;
			continue;
		}
		day_food_map.insert( {food, amount});
	}
	file.close();
	loaded = 1;
}

void Day::save(){
	string filename = to_string(date.year) + "_" + to_string(date.month) + "_" + to_string(date.day);
	ofstream file;
	file.open(filename, ios::out);
	file << "{";
	unsigned int count = 1;
	for(pair<Food*, float> food : day_food_map){
		file << "\"" + food.first->name + "\": " + to_string(food.second);
		if(count++ != day_food_map.size()) cout << ",";
		file << "\n";
	}
	file << "}";
	file.close();
}

Day::Day(us day, us month, us year){
	this->date.day = day;
	this->date.month = month;
	this->date.year = year;
	load();
}

Day::~Day(){

}

bool Day::addFood(Food* food, float amount){
	if(day_food_map.find(food) == day_food_map.end()){
		day_food_map.insert( {food, amount});
		return true;
	}
	day_food_map[food] += amount;
	return false;
}

bool Day::setFood(Food* food, float amount){
	if(day_food_map.find(food) == day_food_map.end()) return false;
	day_food_map.insert( {food, amount});
	return true;
}

bool Day::delFood(Food* food){
	if(day_food_map.erase(food) == 0) return false;
	return true;
}

date_structure Day::getDate(){
	return this->date;
}

float Day::getKcals(){
	float total = 0;
	for(pair<Food*, float> food : day_food_map){
		total += food.first->kcal * (food.second / food.first->serving);
	}
	return total;
}

void Day::print(){
	if(day_food_map.empty()){
		cout << TAB << "no foods logged" << ENDL;
		return;
	}
	float total[] = {0, 0, 0, 0}; // {kcal, C, P ,F}
	float left[] = {0, 0, 0, 0};
	unsigned int count = 1;
	cout << setprecision(0);
	cout << BOLD << UNDERLINE;
	cout << setw(25) << "food";
	cout << setw(10) << "amount";
	cout << setw(5) << "kcal";
	cout << setw(5) << "C";
	cout << setw(5) << "F";
	cout << setw(5) << "P";
	for(pair<Food*, float> food : day_food_map){
		if(count++ == day_food_map.size()) cout << UNDERLINE;
		float mult = food.second / food.first->serving;
		string amount = "(" + to_string(food.second) + " " + food.first->unit + ")";
		cout << TAB << TAB << COLOR_TABLE_BG << left;
		cout << setw(25) << food.first->name;
		cout << setw(10) << amount;
		cout << right;
		cout << setw(5) << food.first->kcal * mult;
		cout << setw(5) << food.first->C * mult;
		cout << setw(5) << food.first->F * mult;
		cout << setw(5) << food.first->P * mult;
		cout << ENDL;
		total[0] += food.first->kcal * mult;
		total[1] += food.first->C * mult;
		total[2] += food.first->F * mult;
		total[3] += food.first->P * mult;
	}
	// get left
	for(int i = 0;i < 4;i++){
		left[i] = profile.target_macros[i] - total[i];
	}
	// print total
	cout << TAB << TAB << COLOR_TABLE_BG << left << COLOR_TABLE_TOTAL << BOLD;
	cout << setw(25) << "TOTAL";
	cout << setw(10) << "";
	cout << setw(5) << to_string(total[0]);
	cout << setw(5) << to_string(total[1]);
	cout << setw(5) << to_string(total[2]);
	cout << setw(5) << to_string(total[3]);
	// print target
	cout << TAB << TAB << COLOR_TABLE_BG << left << COLOR_TABLE_TARGET << BOLD;
	cout << setw(25) << "TARGET";
	cout << setw(10) << "";
	cout << setw(5) << to_string(profile.target_macros[0]);
	cout << setw(5) << to_string(profile.target_macros[1]);
	cout << setw(5) << to_string(profile.target_macros[2]);
	cout << setw(5) << to_string(profile.target_macros[3]);
	// print left
	cout << TAB << TAB << COLOR_TABLE_BG << left << COLOR_TABLE_TOTAL << BOLD;
	cout << setw(25) << "LEFT";
	cout << setw(10) << "" << setw(5);
	string color;
	(left[0] > 0) ? (color = COLOR_OK) : (color = COLOR_FAIL);
	cout << color << to_string(left[0]) << setw(5);
	(left[1] > 0) ? (color = COLOR_OK) : (color = COLOR_FAIL);
	cout << color << to_string(left[1]) << setw(5);
	(left[2] > 0) ? (color = COLOR_OK) : (color = COLOR_FAIL);
	cout << color << to_string(left[2]) << setw(5);
	(left[3] > 0) ? (color = COLOR_OK) : (color = COLOR_FAIL);
	cout << color << to_string(left[3]) << setw(5);
	cout << ENDL;
}

string Day::getName(){
	return to_string(date.day) + "/" + to_string(date.month) + "/" + to_string(date.year);
}

bool day_exists(Date date){
	string filename = to_string(date.year) + "_" + to_string(date.month) + "_" + to_string(date.day);
	ifstream file;
	file.open(filename, ios::in);
	if(!file.is_open()) return false;
	file.close();
	return true;
}

