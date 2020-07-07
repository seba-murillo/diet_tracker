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
#include <sys/stat.h>
//#include <libxls/xls.h>
#include "readline/readline.h"
#include "readline/history.h"
#include "Screen.h"
#include "Day.h"
#include "Food.h"

#define verbose 1
#define FILENAME_PROFILE ".profile"
#define FILENAME_WEIGHTS ".weights"
#define FILENAME_FOOD_DATA ".food_data"
#define FILENAME_FOOD_SPREADSHEET "food_data.ods"

using namespace std;
// TODO
/*
	-> time system
	void show_weights()
	show_streak()
	load_spreadsheet()
	void get_BMR()
	void get_TDEE()
	void get_average()
	void get_age()
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
bool isValidDate(us day, us month, us year);

// TODO
void init_files(); // folders and stuff
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
void get_BMR();
void get_TDEE();
void get_average();
void get_age();

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

struct profile_structure{
		struct date_structure date;
		us height = 0;
		us weight = 0;
		char S = '?';
		float M = 1.0;
		us target_macros[3] = {0, 0, 0};
} profile;

unordered_map<string, float> weight_map;

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

/*
commands:
	add
	set
	remove
	weight

	bmr
	tdee

	average
	last
	weights
	help
	profile
	foodlist

	show || list || ls
	clear
	exit || q
*/

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

bool isValidDate(us day, us month, us year){
	if(!(year >= 1900 && year <= 2200)) return false;
	if(day < 1 || day > 31) return false;
	if(month == 2){
		if((year % 100 != 0 && year % 4 == 0) || (year % 400 == 0)){ // leap year
			if(day > 29) return false;
		}else if(day > 28) return false;
	}
	if(month % 2 == 0 && day > 30) return false;
	return true;
}

void init_files(){
	mkdir("DietTracker", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	mkdir("DietTracker/days", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
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
	unsigned int count = 1;
	for(food_list_it = food_list.begin();food_list_it != food_list.end();food_list_it++){
		file << "\t{";
		file << "\"name\": \"" << (*food_list_it)->name << "\", ";
		file << "\"alias\": \"" << (*food_list_it)->alias << "\", ";
		file << "\"unit\": \"" << (*food_list_it)->unit << "\", ";
		file << "\"serving\": " << (*food_list_it)->serving << ", ";
		file << "\"kcal\": " << (*food_list_it)->kcal << ", ";
		file << "\"C\": " << (*food_list_it)->C << ", ";
		file << "\"F\": " << (*food_list_it)->F << ", ";
		file << "\"P\": " << (*food_list_it)->P;
		if(count++ == food_list.size()) file << "},\n";
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
		sscanf(input.c_str(), "%2hu/%2hu/%4hu", &profile.date.day, &profile.date.month, &profile.date.year);
		if(isValidDate(profile.date.day, profile.date.month, profile.date.year)) break;
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
			"\"activity\": %f, \"target_C\": %hu, \"target_F\": %hu, \"target_P\": %hu}",
			&profile.date.day, &profile.date.month, &profile.date.year, &profile.height,
			&profile.weight, &profile.S, &profile.M, &profile.target_macros[0],
			&profile.target_macros[1], &profile.target_macros[2]);
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
	file << "{\"birthdate\": \"" << profile.date.day << "/" << profile.date.month << "/" << profile.date.year << "\",";
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
	struct date_structure date;
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
	string format = TAB + TAB + BOLD + COLOR_PROFILE + COLOR_TABLE_BG;
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
	/*
	String d_format = Screen.TAB + Screen.TAB + Screen.COLOR.TABLE_BG.toString() + Screen.COLOR.BOLD.toString();
	String w_format = Screen.COLOR.RESET.toString() + Screen.COLOR.TABLE_BG.toString() + Screen.COLOR.DATA.toString();
	for(LocalDate day : weights.keySet()){
		String dayname = String.format(Screen.COLOR.BOLD.toString() + Screen.COLOR.DAY + "%02d/%02d/%04d", day.getDayOfMonth(), day.getMonth().getValue(), day.getYear());
		int space = (Screen.LOAD_SPACING + 3);
		double weight = weights.get(day);
		if(weight >= 100.0) space--;
		log(d_format + "%-" + space + "s" + w_format + "%3.1f kg\n", dayname, weight);
	}
	*/
}

void show_streak(){
	/*
	LocalDate today = getToday();
	int streak = 0;
	for(;;streak++){
		LocalDate date = today.minusDays(streak + 1);
		Day day = Day.load(date);
		if(day == null) break;
	}
	log("- " + Screen.COLOR.BOLD + "current streak: " + Screen.COLOR.SYNTAX + streak + " days!\n");
	*/
}




