/*
 * Day.cpp
 *
 *  Created on: Jul 5, 2020
 *      Author: seba
 */

#include <iostream>
#include <iomanip>
#include <fstream>

#include "day.h"
#include "screen.h"

#define TABLE_SPACING_NAME		25
#define TABLE_SPACING_AMOUNT	10
#define TABLE_SPACING_NUMBERS	7

using namespace std;

string get_day_filename(Date date);

Day::Day(us day, us month, us year){
	this->date.day = day;
	this->date.month = month;
	this->date.year = year;
	load();
}

Day::Day(Date date){
	this->date.day = date.day;
	this->date.month = date.month;
	this->date.year = date.year;
	load();
}
void Day::load(){
	string filename = string(DIRECTORY_MAIN "/" DIRECTORY_DAYS "/") + get_day_filename(this->date);
	ifstream file;
	file.open(filename, ios::in);
	if(!file.is_open()) return;
	string name, tmp, dump_s;
	float amount;
	char dump_c;
	file >> dump_c; // dump '{'
	while(file >> name){
		//find_first_of (const char* s, size_t pos = 0) const;
		while(name.find_first_of('"', 1) == string::npos){
			file >> tmp;
			name += " " + tmp;
		}
		name = name.substr(1, name.length() - 3);
		file >> amount >> dump_s;
		Food* food = Food::find_food(name);
		if(food == nullptr){
			cout << TAB TAB COLOR_SYNTAX "WARNING: '" << name << "' (" << amount << ") not found in database, skipping..." ENDL;
			continue;
		}
		day_food_map[food] = amount;
	}
	file.close();
}

void Day::save(){
	string filename = string(DIRECTORY_MAIN "/" DIRECTORY_DAYS "/") + get_day_filename(this->date);
	if(day_food_map.size() == 0){
		remove(filename.c_str());
		return;
	}
	ofstream file;
	file.open(filename, ios::out);
	file << "{\n";
	unsigned int count = 1;
	for(pair<Food*, float> food : day_food_map){
		file << "\"" + food.first->name + "\": ";
		file << food.second;
		if(count++ != day_food_map.size()) file << ",";
		file << "\n";
	}
	file << "}";
	file.close();
}

bool Day::add_food(Food* food, float amount){
	if(food == nullptr) return false;
	if(day_food_map.find(food) == day_food_map.end()){
		day_food_map[food] = amount;
		save();
		return true;
	}
	day_food_map[food] += amount;
	save();
	return true;
}

bool Day::set_food(Food* food, float amount){
	if(food == nullptr) return false;
	if(day_food_map.find(food) == day_food_map.end()) return false;
	day_food_map[food] = amount;
	save();
	return true;
}

bool Day::del_food(Food* food){
	if(food == nullptr) return false;
	if(day_food_map.erase(food) == 0) return false;
	save();
	return true;
}

Date Day::get_date(){
	return this->date;
}

float Day::get_kcals(){
	float total = 0;
	for(pair<Food*, float> food : day_food_map){
		total += food.first->kcal * (food.second / food.first->serving);
	}
	return total;
}

void Day::print(){
	if(day_food_map.empty()){
		cout << TAB "no foods logged for " COLOR_DAY BOLD << this->get_name() << ENDL;
		return;
	}
	cout << "➤ foods of " COLOR_DAY BOLD << this->get_name() << RESET ":" ENDL;
	us table_total[] = {0, 0, 0, 0}; // {kcal, C, P ,F}
	short table_left[] = {0, 0, 0, 0}; // {kcal, C, P ,F}
	unsigned int count = 1;
	cout << TAB TAB COLOR_TABLE_BG UNDERLINE BOLD;
	cout << left << setw(TABLE_SPACING_NAME) << "food";
	cout << left << setw(TABLE_SPACING_AMOUNT) << "amount";
	cout << right << setw(TABLE_SPACING_NUMBERS) << "kcal";
	cout << right << setw(TABLE_SPACING_NUMBERS) << "C";
	cout << right << setw(TABLE_SPACING_NUMBERS) << "F";
	cout << right << setw(TABLE_SPACING_NUMBERS) << "P";
	cout << ENDL;
	for(auto map_row : day_food_map){
		cout << setprecision(0);
		cout << TAB TAB COLOR_TABLE_BG;
		if(count++ == day_food_map.size()) cout << UNDERLINE;
		Food* food = map_row.first;
		float mult = map_row.second / food->serving;
		//string amount = "(" + to_string((us) map_row.second) + " " + food->unit + ")";
		char amount[20];
		sprintf(amount, "(%.1f %s)", map_row.second, (food->unit).c_str());
		if(map_row.second == (int) map_row.second) sprintf(amount, "(%.0f %s)", map_row.second, (food->unit).c_str());
		cout << left << setw(TABLE_SPACING_NAME) << food->name;
		cout << left << setw(TABLE_SPACING_AMOUNT) << amount;
		cout << right << setw(TABLE_SPACING_NUMBERS) << food->kcal * mult;
		cout << setw(TABLE_SPACING_NUMBERS) << food->C * mult;
		cout << setw(TABLE_SPACING_NUMBERS) << food->F * mult;
		cout << setw(TABLE_SPACING_NUMBERS) << food->P * mult;
		cout << ENDL;
		table_total[0] += (us) (food->kcal * mult);
		table_total[1] += (us) (food->C * mult);
		table_total[2] += (us) (food->F * mult);
		table_total[3] += (us) (food->P * mult);
	}
	// get left
	us target_kcal = (profile.target_macros[0] + profile.target_macros[2]) * 4 + profile.target_macros[1] * 9;
	table_left[0] = target_kcal - table_total[0];
	table_left[1] = profile.target_macros[0] - table_total[1];
	table_left[2] = profile.target_macros[1] - table_total[2];
	table_left[3] = profile.target_macros[2] - table_total[3];
	// print TOTAL
	cout << TAB TAB COLOR_TABLE_BG COLOR_TABLE_TOTAL BOLD;
	cout << left << setw(TABLE_SPACING_NAME) << "TOTAL";
	cout << left << setw(TABLE_SPACING_AMOUNT) << "";
	cout << right << setw(TABLE_SPACING_NUMBERS) << table_total[0];
	cout << right << setw(TABLE_SPACING_NUMBERS) << table_total[1];
	cout << right << setw(TABLE_SPACING_NUMBERS) << table_total[2];
	cout << right << setw(TABLE_SPACING_NUMBERS) << table_total[3];
	cout << ENDL;
	// print TARGET
	cout << TAB TAB COLOR_TABLE_BG COLOR_TABLE_TOTAL BOLD;
	cout << left << setw(TABLE_SPACING_NAME) << "TARGET";
	cout << left << setw(TABLE_SPACING_AMOUNT) << "" << COLOR_TABLE_TARGET;

	cout << right << setw(TABLE_SPACING_NUMBERS) << target_kcal;
	cout << right << setw(TABLE_SPACING_NUMBERS) << profile.target_macros[0];
	cout << right << setw(TABLE_SPACING_NUMBERS) << profile.target_macros[1];
	cout << right << setw(TABLE_SPACING_NUMBERS) << profile.target_macros[2];
	cout << ENDL;
	// print LEFT
	cout << TAB TAB COLOR_TABLE_BG COLOR_TABLE_TOTAL BOLD;
	cout << left << setw(TABLE_SPACING_NAME) << "LEFT";
	cout << left << setw(TABLE_SPACING_AMOUNT) << "" << setw(5);
	string color;
	for(int i = 0;i < 4;i++){
		(table_left[i] > 0) ? (color = COLOR_OK) : (color = COLOR_FAIL);
		cout << right << color << setw(TABLE_SPACING_NUMBERS) << table_left[i];
	}
	cout << ENDL;
}

string Day::get_name(){
	Date today = get_today();
	//if(date == today) return string("today");
	//if(date == (today - 1)) return string("yesterday");
	//if(date == (today + 1)) return string("tomorrow");
	char str[20];
	sprintf(str, "%02d/%02d/%04d", date.day, date.month, date.year);
	return string(str);
}

string get_day_filename(Date date){
	char str[20];
	sprintf(str, "%04d_%02d_%02d", date.year, date.month, date.day);
	return string(str);
}
