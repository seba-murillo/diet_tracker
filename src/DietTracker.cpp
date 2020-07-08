//============================================================================
// Name        : DietTracker.cpp
// Author      : Seba Murillo
// Version     : 0.1
// Copyright   : GPL
// Description : DietTracker in C++
//============================================================================

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <map>
#include <sys/stat.h>
//#include <libxls/xls.h>
#include "readline/readline.h"
#include "readline/history.h"
#include "Screen.h"
#include "Day.h"
#include "common.h"
#include "Food.h"

using namespace std;
// TODO
/*
 food -> load from spreadsheet

 load_spreadsheet()

 show_streak()

 us get_TDEE() -> measured
 us get_average();

 void command_add()
 void command_set()
 void command_remove()
 void command_weight()
 void command_BMR()
 void command_TDEE()
 void command_average()
 void command_last()
 void command_weights()
 void command_help()
 void command_profile()
 void command_foodlist()
 void command_show()
 */

void process_command(string command);
void screen_test();

void init_files(); //
bool load_spreadsheet(); //
bool load_foodfile(); //
bool load_profile(); //
bool load_weighfile();

// show
void show_profile();
void show_weights();
void show_streak();

// save
bool save_foodfile(); //
bool save_profile(); //
bool save_weighfile();

// calc
us get_BMR();
us get_TDEE();
us get_average(us days);

// other
void create_profile();
string extract(string source, string start, string end);
// commands
void command_add();
void command_set();
void command_remove();
void command_weight();
void command_BMR();
void command_TDEE();
void command_average();
void command_last();
void command_weights();
void command_help();
void command_profile();
void command_foodlist();
void command_show();

map<string, float> weight_map;
struct profile_structure profile;

int main(int argc, char** argv){
	cout << "Hey there!" << endl;
	cout << "> starting [" << COLOR_FAIL << BOLD << "DietTracker v1.0" << RESET << "]" << ENDL;
	// load
	init_files();

	if(!load_foodfile()){
		cout << COLOR_FAIL << BOLD << TAB << "foodfile not found" << ENDL;
		exit(EXIT_FAILURE);
	}
	/*
	 if(!load_profile()) create_profile();
	 */
	// main loop
	char* command;
	while((command = readline("> ")) != nullptr){
		if(strlen(command) > 0){
			add_history(command);
		}
		process_command(string(command));
		free(command); // readline malloc's a new buffer every time.
	}
	return EXIT_SUCCESS;
}

void process_command(string command){
	cout << "process_command: [" << command << "]" << endl;
	if(command == "exit" || command == "quit" || command == "q"){
		cout << "> stopping [" << COLOR_FAIL << BOLD << "DietTracker v1.0" << RESET << "]" << ENDL;
		exit(EXIT_SUCCESS);
	}else if(command == "clear"){
		for(int i = 0;i < CLEAR_LINES;i++)
			cout << ENDL;
	}else if(command == "add"){
		// add
	}else if(command == "remove"){
		// remove
	}else if(command == "weight"){
		// weight
	}else if(command == "bmr"){
		// bmr
	}else if(command == "tdee"){
		// tdee
	}else if(command == "average"){
		//
	}else if(command == "last"){
		//
	}else if(command == "weights"){
		//
	}else if(command == "help"){
		//
	}else if(command == "profile"){
		//
	}else if(command == "foodlist"){
		//
	}else if(command == "show" || command == "list" || command == "ls"){
		//
	}else{
		cout << BOLD << "- unknown command." << RESET << ". use 'help' to se available commands\n" << ENDL;
	}
}

void screen_test(){
	cout << "screen test:" << ENDL;
	cout << OK << ENDL;
	cout << FAIL << ENDL;
}

void init_files(){
	mkdir(DIRECTORY_MAIN, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	mkdir(DIRECTORY_MAIN "/" DIRECTORY_DAYS, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

bool save_foodfile(){
	string filename = FILENAME_FOOD_DATA;
	ofstream file;
	file.open(filename, ios::out);
	if(!file.is_open()){
		cout << COLOR_FAIL << "ERROR: could not open [" << filename << "]" << ENDL;
		return false;
	}
	file << "{\n\"foods\": [\n";
	list<Food*>::iterator food_list_it;
	file.precision(1);
	file << fixed;
	Food* food;
	us count = 1;
	map<string, Food*>::iterator i;
	for(i = food_map.begin();i != food_map.end();i++){
		food = i->second;
		file << "\t{";
		file << "\"name\": \"" << food->name << "\", ";
		file << "\"alias\": \"" << food->alias << "\", ";
		file << "\"unit\": \"" << food->unit << "\", ";
		file << "\"serving\": " << food->serving << ", ";
		file << "\"kcal\": " << food->kcal << ", ";
		file << "\"C\": " << food->C << ", ";
		file << "\"F\": " << food->F << ", ";
		file << "\"P\": " << food->P;
		if(count++ == food_map.size()) file << "},\n";
		else file << "}\n";
	}
	file << "]\n}";
	file.close();
	return true;
}

bool load_foodfile(){
	string filename = FILENAME_FOOD_DATA;
	ifstream file;
	file.open(filename, ios::in);
	// TODO use last_updated file
	if(!file.is_open()){
		if(!load_spreadsheet()) return false;
		save_foodfile();
		return true;
	}
	string line, skip;
	string name, alias, unit;
	float serving, kcal, C, F, P;
	while(getline(file, line)){
		line.substr();
		cout << "line: " << line << endl << endl;
		name = extract(line, "\"name\": \"", "\",");
		alias = extract(line, "\"alias\": \"", "\",");
		unit = extract(line, "\"unit\": \"", "\",");
		serving = stod(extract(line, "\"serving\": ", ","));
		kcal = stod(extract(line, "\"kcal\": ", ","));
		C = stod(extract(line, "\"C\": ", ","));
		F = stod(extract(line, "\"F\": ", ","));
		P = stod(extract(line, "\"P\": ", "}"));
		Food(name, alias, unit, serving, kcal, C, F, P);
	}
	file.close();
	return true;
}

bool load_spreadsheet(){
	return true;
}

void command_add(){

}

void command_set(){

}

void command_remove(){

}

void command_weight(){

}

void command_BMR(){

}

void command_TDEE(){

}

void command_average(){

}

void command_last(){

}

void command_weights(){

}
//
void command_help(){
	cout << COLOR_HELP << BOLD << "- available commands are:" << ENDL << left;
	cout << TAB << TAB << COLOR_HELP << setw(15) << "load" << RESET << "[DD/MM or DD/MM/YYYY]" << ENDL;
	cout << TAB << TAB << COLOR_HELP << setw(15) << "add" << RESET << "['foodname'] [amount]" << ENDL;
	cout << TAB << TAB << COLOR_HELP << setw(15) << "set" << RESET << "['foodname'] [amount]" << ENDL;
	cout << TAB << TAB << COLOR_HELP << setw(15) << "remove" << RESET << "['foodname']" << ENDL;
	cout << TAB << TAB << COLOR_HELP << setw(15) << "weight" << RESET << "['weight']" << ENDL;
	cout << TAB << TAB << COLOR_HELP << setw(15) << "last" << RESET << "['days']" << ENDL;
	cout << TAB << TAB << COLOR_HELP << setw(15) << "average" << RESET << "['days']" << ENDL;
	cout << TAB << TAB << COLOR_HELP << "BMR" << ENDL;
	cout << TAB << TAB << COLOR_HELP << "TDEE" << ENDL;
	cout << TAB << TAB << COLOR_HELP << "profile" << ENDL;
	cout << TAB << TAB << COLOR_HELP << "show" << ENDL;
	cout << TAB << TAB << COLOR_HELP << "weights" << ENDL;
	cout << TAB << TAB << COLOR_HELP << "clear" << ENDL;
	cout << TAB << TAB << COLOR_HELP << "show / list / ls" << ENDL;
	cout << TAB << TAB << COLOR_HELP << "exit / quit / q" << ENDL;
}

void command_profile(){

}

void command_foodlist(){

}

void command_show(){

}

void create_profile(){
	cout << COLOR_HELP << BOLD << "> creating new profile..." << ENDL;
	string input;
	while(true){
		cout << TAB << "1. enter your birth date (dd/mm/yyyy): ";
		cin >> input;
		sscanf(input.c_str(), "%2hu/%2hu/%4hu", &profile.birth.day, &profile.birth.month, &profile.birth.year);
		if(isValidDate(profile.birth.day, profile.birth.month, profile.birth.year)) break;
	}
	while(true){
		cout << TAB << "2. enter your height (cm): ";
		cin >> input;
		sscanf(input.c_str(), "%hu", &profile.height);
		if(profile.height > 100 && profile.height < 250) break;
	}
	while(true){
		cout << TAB << "3. enter your weight (kg): ";
		cin >> input;
		sscanf(input.c_str(), "%hu", &profile.weight);
		if(profile.weight > 40 && profile.weight < 200) break;
	}
	while(true){
		cout << TAB << "4. enter your sex ('M' or 'F'): ";
		cin >> profile.S;
		if(profile.S == 'M' || profile.S == 'F') break;
	}
	while(true){
		cout << TAB << "6. enter your target macros (C-F-P): ";
		cin >> input;
		sscanf(input.c_str(), "%hu/%hu/%hu", &profile.target_macros[0], &profile.target_macros[1], &profile.target_macros[2]);
		break;
	}
	while(true){
		cout << TAB << "6. choose one: ";
		cout << TAB << TAB << "A) sedentary: little to no exercise";
		cout << TAB << TAB << "B) lightly active: light exercise";
		cout << TAB << TAB << "C) moderately active: moderate exercise";
		cout << TAB << TAB << "D) very active: heavy exercise";
		cout << TAB << TAB << "E) extremely active: very heavy exercise";
		char choice;
		cin >> choice;
		switch(choice){
			case 'A':
				profile.M = 1.2;
				break;
			case 'B':
				profile.M = 1.375;
				break;
			case 'C':
				profile.M = 1.55;
				break;
			case 'D':
				profile.M = 1.725;
				break;
			case 'E':
				profile.M = 1.9;
				break;
		}
		if(profile.M != 1.0) break;
	}
	if(!save_profile()) return;
	cout << TAB << "- profile " << BOLD << COLOR_OK << "created" << ENDL;
}

bool load_profile(){
	string filename = FILENAME_PROFILE;
	ifstream file;
	file.open(filename, ios::in);
	if(!file.is_open()) return false;
	string line;
	getline(file, line);
	sscanf(line.c_str(), "{\"birthdate\": \"%2hu/%2hu/%4hu, \"height\": %hu, \"weight\": %hu, \"sex\": %c, "
			"\"activity\": %f, \"target_C\": %hu, \"target_F\": %hu, \"target_P\": %hu}", &profile.birth.day, &profile.birth.month, &profile.birth.year, &profile.height, &profile.weight, &profile.S, &profile.M, &profile.target_macros[0], &profile.target_macros[1], &profile.target_macros[2]);
	file.close();
	return true;
}

bool save_profile(){
	string filename = FILENAME_PROFILE;
	ofstream file;
	file.open(filename, ios::out);
	if(!file.is_open()){
		cout << COLOR_FAIL << "ERROR: could not open [" << filename << "]" << ENDL;
		return false;
	}
	file << "{\"birthdate\": \"" << profile.birth.day << "/" << profile.birth.month << "/" << profile.birth.year << "\",";
	file << "\"height\": " << profile.height << ", ";
	file << "\"weight\": " << profile.weight << ", ";
	file << "\"sex\": " << profile.S << ", ";
	file << "\"activity\": " << profile.M << ", ";
	file << "\"target_C\": " << profile.target_macros[0] << ", ";
	file << "\"target_F\": " << profile.target_macros[1] << ", ";
	file << "\"target_P\": " << profile.target_macros[2] << "}";
	file.close();
	return true;
}

bool load_weighfile(){
	string filename = FILENAME_WEIGHTS;
	ifstream file;
	file.open(filename, ios::in);
	if(!file.is_open()) return false;
	//struct date_structure date;
	string str_date;
	float W;
	while(file >> str_date >> W){
		//sscanf(str_date.c_str(), "%2hu/%2hu/%4hu", &date.day, &date.month, &date.year);
		weight_map.insert( {str_date, W});
	}
	file.close();
	return true;
}

bool save_weighfile(){
	ofstream file;
	file.open(FILENAME_WEIGHTS, ios::out);
	if(!file.is_open()){
		cout << COLOR_FAIL << "ERROR: could not open [" << FILENAME_WEIGHTS << "]" << ENDL;
		return false;
	}
	for(pair<string, float> weight : weight_map){
		file << weight.first << "\t" << weight.second << endl;
	}
	file.close();
	return true;
}

// show
void show_profile(){
	string format = string(TAB) + TAB + BOLD + COLOR_PROFILE + COLOR_TABLE_BG;
	string target = to_string(profile.target_macros[0]) + "-" + to_string(profile.target_macros[1]) + "-" + to_string(profile.target_macros[2]);
	us align = 25;
	cout << format << left << setw(align) << "age:" << right << 27 << ENDL;
	cout << format << left << setw(align) << "height (cm):" << right << profile.height << ENDL;
	cout << format << left << setw(align) << "weight (kg):" << right << profile.weight << ENDL;
	cout << format << left << setw(align) << "sex:" << right << profile.S << ENDL;
	cout << format << left << setw(align) << "macros:" << right << target << ENDL;
	cout << format << left << setw(align) << "activity:" << right << profile.M << ENDL;
	cout << format << left << setw(align) << "BMR:" << right << get_BMR() << ENDL;
	cout << format << left << setw(align) << "TDEE:" << right << get_TDEE() << ENDL;
}

void show_weights(){
	string date_format = string(TAB) + TAB + BOLD + COLOR_DATE + COLOR_TABLE_BG;
	string weight_format = string(COLOR_PROFILE) + COLOR_TABLE_BG;
	for(pair<string, float> weight : weight_map){
		cout << left << setw(SPACING_DEFAULT) << weight.first << setprecision(1) << weight.second << ENDL;
	}
}

void show_streak(){
	Date date = get_today();
	for(us streak = 0;;streak++){
		date = get_previous_day(date);
		if(day_exists(date)) continue;
		cout << BOLD << "- current streak: " << COLOR_SYNTAX << streak << " days!" << ENDL;
		return;
	}
}

us get_average(us days){
	Date date = get_today();
	double kcal = 0;
	us day;
	for(day = 1;day <= days;day++){
		if(!day_exists(date)) break;
		Day D = Day(date.day, date.month, date.year);
		kcal += D.getKcals();
		date = get_previous_day(date);
	}
	return (us) (kcal / day);
}

us get_BMR(){
	// some online method
	/*
	 if(sex == 'F') return (int) (655 + (9.6 * profile.weight) + (1.8 * profile.height) - (4.7 * get_age()));
	 return (int) (66 + (13.7 * profile.weight) + (5 * profile.height) - (6.8 * get_age()));
	 */
	// Mifflin-St Jeor
	/*
	 int S = 5;
	 if(sex == 'F') S = -151;
	 return (int) (S + (10 * profile.weight) + (6.25 * profile.height) - (5.0 * get_age()));
	 */
	// Harris-Benedict
	if(profile.S == 'F')
		return (447.593 + (9.247 * profile.weight) + (3.098 * profile.height) - (4.330 * get_age(profile.birth)));
	return (447.593 + (13.397 * profile.weight) + (3.098 * profile.height) - (4.330 * get_age(profile.birth)));
}

us get_TDEE(){
	return (us) (get_BMR() * profile.M);
}

string extract(string source, string start, string end){
	if(source.empty()) return string();
	int pos1, pos2;
	pos1 = source.find(start);
	if(pos1 == -1) return string();
	pos1 += start.length();
	pos2 = (source.substr(pos1)).find(end);
	if(pos2 == -1) return string();
	if(end.empty()) pos2 = source.length();
	return source.substr(pos1, pos2);
}
