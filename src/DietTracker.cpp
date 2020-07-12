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

#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <zip.h>

#include "readline/readline.h"
#include "readline/history.h"

#include "common.h"
#include "day.h"
#include "food.h"
#include "screen.h"

using namespace std;

// file defines
#define FILENAME_PROFILE 			".profile"
#define FILENAME_WEIGHTS			".weights"
#define FILENAME_FOOD_SPREADSHEET 	"food_data.ods"
// XML parsing defines
#define FOODDATA_SUBFILENAME		"content.xml"
#define TAG_END						">"
#define TAG_ROW_START				"<table:table-row"
#define TAG_ROW_END					"</table:table-row>"
#define TAG_CELL_START				"<table:table-cell"
#define TAG_CELL_END				"</table:table-cell>"
#define TAB_CELL_REPEAT 			"table:number-columns-repeated=\""
#define TAG_TEXT_START				"<text:p>"
#define TAG_TEXT_END				"</text:p>"
#define TAG_INDEX_NAME				"<text:p>name</text:p>"

#define DEFAULT_DAYS		7

us get_BMR();
us get_TDEE();
/*
us get_average_kcals(Date date, us days);
us get_average_kcals(Date from, Date to);
*/
bool load_foods();
bool load_profile();
bool load_weights();
bool save_profile();
bool save_weights();
void print_profile();
void print_weights();
void print_streak();
void create_profile();
void init_files();
void exit_program();
void signal_handler(int signal);
void process_command(string command);
void load_day(Date day);
void command_load(string day);
void command_add(string foodname, float amount);
void command_set(string foodname, float amount);
void command_del(string foodname);
void command_info(string foodname);
void command_weight(float weight);
void command_average(us days);
void command_last(us days);
void command_help();
void command_show();
string extract(string source, string start, string end);

map<string, float> weight_map;
struct profile_structure profile;
Day* selected_day;

int main(int argc, char** argv){
	cout << "> starting [" COLOR_FAIL BOLD "DietTracker v1.0" RESET "]" ENDL;
	// setup signal handling
	atexit(exit_program);
	struct sigaction action;
	action.sa_handler = signal_handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	sigaction(SIGINT, &action, NULL);
	sigaction(SIGHUP, &action, NULL);
	sigaction(SIGTERM, &action, NULL);
	// create directory system and load food data and profile
	init_files();
	if(!load_foods()) exit(EXIT_FAILURE);
	if(!load_profile()) create_profile();
	load_weights();
	print_streak();
	// load today
	selected_day = new Day(get_today());
	selected_day->print();
	// main loop
	char* command;
	while((command = readline("> ")) != nullptr){
		if(strlen(command) > 0) add_history(command);
		if(strcmp(command, "exit") == 0 || strcmp(command, "quit") == 0 || strcmp(command, "q") == 0) break;
		process_command(string(command));
		free(command);
	}
	return EXIT_SUCCESS;
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
	return (us) (get_BMR() * profile.A);
}
/*
us get_average_kcals(Date date, us days){
	double kcal = 0;
	us day;
	for(day = 1;day <= days;day++){
		if(!day_exists(date)) break;
		Day D = Day(date.day, date.month, date.year);
		kcal += D.get_kcals();
		date--;
	}
	return (us) (kcal / day);
}

us get_average_kcals(Date from, Date to){
	if(from > to) return 0;
	double total = 0;
	us days = 0;
	while(true){
		if(from == to) break;
		Day day = Day(from);
		float kcal = day.get_kcals();
		if(kcal > 0) total += kcal;
		days++;

	}
	return (us) (total / days);
}
*/

bool load_foods(){
#ifdef verbose
	cout << left << setw(SPACING_DEFAULT) << "- loading foods...";
#endif
	int error = 0;
	zip* spreadsheet = zip_open(DIRECTORY_MAIN "/" FILENAME_FOOD_SPREADSHEET, 0, &error);
	if(error){
		cout << FAIL;
		return false;
	}
	struct zip_stat zstat;
	zip_stat_init(&zstat);
	zip_stat(spreadsheet, FOODDATA_SUBFILENAME, 0, &zstat);
	char* CONTENT_XML = new char[zstat.size];
	zip_file* f = zip_fopen(spreadsheet, FOODDATA_SUBFILENAME, 0);
	zip_fread(f, CONTENT_XML, zstat.size);
	zip_fclose(f);
	zip_close(spreadsheet);
	// parse
#ifdef debug
	us ROW_COUNT = 1;
	us CELL_COUNT = 1;
#endif
	char* pos[6];
	pos[1] = strstr(CONTENT_XML, TAG_ROW_START); // first row position
	char row[2048]; // should be enough
	char cell[256]; // should be enough
	char text[64]; // should be enough
	while((pos[0] = strstr(pos[1], TAG_ROW_START))){
#ifdef debug
		ROW_COUNT++;
		CELL_COUNT = 1;
#endif
		us element = 1;
		string name, alias, unit;
		float serving = INEX, kcal = INEX, C = INEX, F = INEX, P = INEX;
		pos[0] += strlen(TAG_ROW_START);
		pos[1] = strstr(pos[0], TAG_END) - 1;
		if((pos[1])[0] != '/') pos[1] = strstr(pos[0], TAG_ROW_END);
		memset(row, 0, sizeof(row));
		strncpy(row, pos[0], pos[1] - pos[0]);
		if(strstr(row, TAG_INDEX_NAME) != NULL) continue; // check row is not index
		if(strstr(row, TAG_TEXT_START) == NULL) continue; // check row is not "empty"
		// process row
		pos[3] = strstr(row, TAG_CELL_START);
		short repeats = 0;
		while((pos[2] = strstr(pos[3], TAG_CELL_START))){
			pos[2] += strlen(TAG_CELL_START);
			pos[3] = strstr(pos[2], TAG_END) - 1;
			if((pos[3])[0] == '/'){ // empty cell
				element++;
				continue; // goto next cell
			}
			pos[3] = strstr(pos[2], TAG_CELL_END);
			memset(cell, 0, sizeof(cell));
			strncpy(cell, pos[2], pos[3] - pos[2]);
			// process cell
			// check if value is repeated
			if(repeats < 0) repeats = 0;
			if((pos[4] = strstr(cell, TAB_CELL_REPEAT)) != NULL){
				pos[4] += strlen(TAB_CELL_REPEAT);
				pos[5] = strstr(pos[4], "\"");
				memset(text, 0, sizeof(text));
				strncpy(text, pos[4], pos[5] - pos[4]);
				repeats = stoi(text) - 1;
			}
			// find value
			pos[4] = strstr(cell, TAG_TEXT_START) + strlen(TAG_TEXT_START);
			pos[5] = strstr(pos[4], TAG_TEXT_END);
			memset(text, 0, sizeof(text));
			strncpy(text, pos[4], pos[5] - pos[4]);
#ifdef debug
			cout << TAB TAB << "cell[" << CELL_COUNT++ << "]: " << text << " (" << repeats << " repeats)";
			cout << " [e: " << element << "]" << endl;
#endif
			// apply values
			for(;repeats >= 0;repeats--){
				switch(element){
					case 1: {
						name = string(text);
						break;
					}
					case 2: {
						alias = string(text);
						break;
					}
					case 3: {
						unit = string(text);
						break;
					}
					case 4: {
						serving = stod(text);
						break;
					}
					case 5: {
						kcal = stod(text);
						break;
					}
					case 6: {
						C = stod(text);
						break;
					}
					case 7: {
						F = stod(text);
						break;
					}
					case 8: {
						P = stod(text);
						break;
					}
				}
				element++;
			}
			if(element > 8) break; // next row
		}
		// create food
		new Food(name, alias, unit, serving, kcal, C, F, P);
	}
	delete[] CONTENT_XML;
#ifdef verbose
	cout << OK;
#endif
	return true;
}

bool load_profile(){
#ifdef verbose
	cout << left << setw(SPACING_DEFAULT) << "- loading profile...";
#endif
	string filename = DIRECTORY_MAIN "/" FILENAME_PROFILE;
	ifstream file;
	file.open(filename, ios::in);
	if(!file.is_open()){
		cout << FAIL;
		return false;
	}
	string tmp;
	file >> tmp >> tmp >> tmp;
	tmp[tmp.find("\"")] = ' ';
	tmp[tmp.find("\"")] = ' ';
	tmp[tmp.find(",")] = ' ';
	stringstream stream(tmp);
	string birth[3];
	getline(stream, birth[0], '/');
	getline(stream, birth[1], '/');
	getline(stream, birth[2], '/');
	profile.birth.day = stoi(birth[0]);
	profile.birth.month = stoi(birth[1]);
	profile.birth.year = stoi(birth[2]);
	file >> tmp >> profile.height >> tmp;
	file >> tmp >> profile.weight >> tmp;
	file >> tmp >> profile.S >> tmp;
	file >> tmp >> profile.A >> tmp;
	file >> tmp >> profile.target_macros[0] >> tmp;
	file >> tmp >> profile.target_macros[1] >> tmp;
	file >> tmp >> profile.target_macros[2];
	file.close();
#ifdef verbose
	cout << OK;
#endif
	return true;
}

bool load_weights(){
#ifdef verbose
	cout << left << setw(SPACING_DEFAULT) << "- loading weights...";
#endif
	string filename = DIRECTORY_MAIN "/" FILENAME_WEIGHTS;
	ifstream file;
	file.open(filename, ios::in);
	if(!file.is_open()) return false;
	string str;
	float W;
	while(file >> str >> W){
		weight_map.insert( {str, W});
	}
	file.close();
	cout << OK;
	return true;
}

bool save_profile(){
	string filename = DIRECTORY_MAIN "/" FILENAME_PROFILE;
	ofstream file;
	file.open(filename, ios::out);
	if(!file.is_open()){
		cout << ERROR FORMAT_ERROR "unable to open '" << filename << "'" ENDL;
		return false;
	}
	file << "{" << endl;
	file << "\"birthdate\": \"" << profile.birth.day << "/" << profile.birth.month << "/" << profile.birth.year << "\"," << endl;
	file << "\"height\": " << profile.height << ", " << endl;
	file << "\"weight\": " << profile.weight << ", " << endl;
	file << "\"sex\": " << profile.S << ", " << endl;
	file << "\"activity\": " << profile.A << ", " << endl;
	file << "\"target_C\": " << profile.target_macros[0] << ", " << endl;
	file << "\"target_F\": " << profile.target_macros[1] << ", " << endl;
	file << "\"target_P\": " << profile.target_macros[2] << endl;
	file << "}";
	file.close();
	return true;
}

bool save_weights(){
	string filename = DIRECTORY_MAIN "/" FILENAME_WEIGHTS;
	ofstream file;
	file.open(filename, ios::out);
	if(!file.is_open()){
		cout << ERROR FORMAT_ERROR "unable to open '" << filename << "'" ENDL;
		return false;
	}
	for(pair<string, float> weight : weight_map){
		file << weight.first << "\t" << weight.second << endl;
	}
	file.close();
	return true;
}

void print_profile(){
	string target = to_string(profile.target_macros[0]) + "-" + to_string(profile.target_macros[1]) + "-" + to_string(profile.target_macros[2]);
	cout << fixed;
	cout.precision(1);
	cout << FORMAT_TAG << left << setw(15) << "age:" << FORMAT_DATA << right << setw(15) << get_age(profile.birth) << ENDL;
	cout << FORMAT_TAG << left << setw(15) << "height (cm):" << FORMAT_DATA << right << setw(15) << profile.height << ENDL;
	cout << FORMAT_TAG << left << setw(15) << "weight (kg):" << FORMAT_DATA << right << setw(15) << profile.weight << ENDL;
	cout << FORMAT_TAG << left << setw(15) << "sex:" << FORMAT_DATA << right << setw(15) << profile.S << ENDL;
	cout << FORMAT_TAG << left << setw(15) << "macros:" << FORMAT_DATA << right << setw(15) << target << ENDL;
	cout << FORMAT_TAG << left << setw(15) << "activity:" << FORMAT_DATA << setprecision(2) << right << setw(15) << profile.A << ENDL;
	cout << FORMAT_TAG << left << setw(15) << "BMR:" << FORMAT_DATA << right << setw(15) << get_BMR() << ENDL;
	cout << FORMAT_TAG << left << setw(15) << "TDEE:" << FORMAT_DATA << right << setw(15) << get_TDEE() << ENDL;
}

void print_weights(){
	string date_format = TAB TAB BOLD COLOR_DATE COLOR_TABLE_BG;
	string weight_format = COLOR_WEIGHT COLOR_TABLE_BG;
	for(auto row : weight_map){
		cout << date_format << left << setw(20) << row.first << RESET;
		cout << weight_format << setprecision(1) << row.second << ENDL;
	}
}

void print_streak(){
	Date date = get_today();
	for(us streak = 0;;streak++){
		date--;
		if(Day(date).get_kcals() > 0) continue;
		cout << BOLD "- current streak: " COLOR_SYNTAX << streak << " days!" ENDL;
		return;
	}
}

void create_profile(){
	cout << TAB COLOR_HELP BOLD "> creating new profile..." ENDL;
	string input;
	while(true){
		cout << TAB "1. enter your birth date (dd/mm/yyyy): ";
		cin >> input;
		sscanf(input.c_str(), "%2hu/%2hu/%4hu", &profile.birth.day, &profile.birth.month, &profile.birth.year);
		if(is_valid_date(profile.birth.day, profile.birth.month, profile.birth.year)) break;
	}
	while(true){
		cout << TAB "2. enter your height (cm): ";
		cin >> input;
		sscanf(input.c_str(), "%hu", &profile.height);
		if(profile.height > 100 && profile.height < 250) break;
	}
	while(true){
		cout << TAB "3. enter your weight (kg): ";
		cin >> input;
		sscanf(input.c_str(), "%f", &profile.weight);
		if(profile.weight > 40 && profile.weight < 200) break;
	}
	while(true){
		cout << TAB "4. enter your sex ('M' or 'F'): ";
		cin >> profile.S;
		if(profile.S == 'M' || profile.S == 'F') break;
	}
	while(true){
		cout << TAB "6. enter your target macros (C-F-P): ";
		cin >> input;
		sscanf(input.c_str(), "%hu-%hu-%hu", &profile.target_macros[0], &profile.target_macros[1], &profile.target_macros[2]);
		break;
	}
	while(true){
		cout << TAB "6. choose one: " ENDL;
		cout << TAB TAB "A) sedentary: little to no exercise" ENDL;
		cout << TAB TAB "B) lightly active: light exercise" ENDL;
		cout << TAB TAB "C) moderately active: moderate exercise" ENDL;
		cout << TAB TAB "D) very active: heavy exercise" ENDL;
		cout << TAB TAB "E) extremely active: very heavy exercise" ENDL;
		cout << TAB "- your choice: ";
		char choice;
		cin >> choice;
		switch(choice){
			case 'A':
				profile.A = 1.2;
				break;
			case 'B':
				profile.A = 1.375;
				break;
			case 'C':
				profile.A = 1.55;
				break;
			case 'D':
				profile.A = 1.725;
				break;
			case 'E':
				profile.A = 1.9;
				break;
		}
		if(profile.A != 1.0) break;
	}
	if(!save_profile()) return;
	cout << "> profile " BOLD COLOR_OK "created" ENDL;
}

void init_files(){
	mkdir(DIRECTORY_MAIN, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	mkdir(DIRECTORY_MAIN "/" DIRECTORY_DAYS, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

void exit_program(){
	cout << "> stopping [" COLOR_FAIL BOLD "DietTracker v1.0" RESET "]" ENDL;
}

void signal_handler(int signal){
	cout << endl;
	exit_program();
	_Exit(EXIT_SUCCESS);
}

void process_command(string command){
	if(command.empty()) return;
	stringstream stream(command);
	string cmd, arg, tmp;
	float number = INEX;
	stream >> cmd;
	if(cmd == "load"){
		while(stream >> tmp){
			if(!arg.empty()) arg += " ";
			arg += tmp;
		}
		command_load(arg);
		return;
	}
	while(stream >> tmp){
		try{
			number = stod(tmp);
		}
		catch(invalid_argument& e){
			if(!arg.empty()) arg += " ";
			arg += tmp;
		}
	}
	if(cmd == "show" || cmd == "list" || cmd == "ls"){
		command_show();
	}
	else if(cmd == "add"){
		if(number == INEX) number = 1;
		command_add(arg, number);
	}
	else if(cmd == "set"){
		if(number == INEX) number = 1;
		command_set(arg, number);
	}
	else if(cmd == "del" || cmd == "delete" || cmd == "remove"){
		command_del(arg);
	}
	else if(cmd == "info"){
		command_info(arg);
	}
	else if(cmd == "weight"){
		command_weight(number);
	}
	else if(cmd == "last"){
		if(number == INEX) number = DEFAULT_DAYS;
		command_last((us) number);
	}
	else if(cmd == "average"){
		if(number == INEX) number = DEFAULT_DAYS;
		command_average((us) number);
	}
	else if(cmd == "profile"){
		print_profile();
	}
	else if(cmd == "weights"){
		print_weights();
	}
	else if(cmd == "help"){
		command_help();
	}
	else if(cmd == "foodlist"){
		Food::print_all();
	}
	else if(cmd == "clear"){
		for(int i = 0;i < CLEAR_LINES;i++)
			cout << ENDL;
	}
	else{
		cout << BOLD COLOR_SYNTAX "- unknown command:" RESET " use 'help' to se available commands" ENDL;
	}
}

void load_day(Date day){
	selected_day->save();
	delete (selected_day);
	selected_day = new Day(day);
	selected_day->print();
}

void command_load(string day){
	Date today = get_today();
	if(day == "now" || day == "today"){
		load_day(today);
		return;
	}
	else if(day == "yes" || day == "yesterday"){
		load_day(--today);
		return;
	}
	else if(day == "tom" || day == "tomorrow"){
		load_day(++today);
		return;
	}
	else if(day == "prev"){
		Date date = selected_day->get_date();
		load_day(--date);
		return;
	}
	else if(day == "next"){
		Date date = selected_day->get_date();
		load_day(++date);
		return;
	}
	size_t pos;
	while((pos = day.find('/')) != string::npos)
		day[pos] = ' '; // remove '/'
	Date date;
	date.year = today.year;
	stringstream stream(day);
	stream >> date.day;
	stream >> date.month;
	stream >> date.year;
	if(date.year == 0) date.year = today.year;
	if(!is_valid_date(date)){
		cout << BOLD COLOR_SYNTAX "- invalid date" ENDL;
		return;
	}
	load_day(date);
}

void command_add(string foodname, float amount){
	Food* food = Food::find_food(foodname);
	if(!selected_day->add_food(food, amount)){
		cout << ERROR FORMAT_ERROR "unknown food '" << foodname << "'" ENDL;
		return;
	}
	cout << "- added " BOLD COLOR_AMOUNT << amount << " " << food->unit;
	cout << RESET << " of '" BOLD COLOR_FOOD << foodname << RESET "'";
	cout << " to " BOLD COLOR_DAY << selected_day->get_name() << RESET "'s foods" ENDL;
	;
}

void command_set(string foodname, float amount){
	Food* food = Food::find_food(foodname);
	if(amount < 0.1){
		cout << ERROR FORMAT_ERROR "cannot set '" << foodname << "' to a negative value" ENDL;
		return;
	}
	if(!selected_day->set_food(food, amount)){
		cout << ERROR FORMAT_ERROR "'" << foodname << "' is not part of the day's foods" ENDL;
		return;
	}
	cout << "- set '" BOLD COLOR_FOOD << foodname << RESET "'";
	cout << " to " BOLD COLOR_AMOUNT << amount << " " << food->unit << RESET;
	cout << " on " BOLD COLOR_DAY << selected_day->get_name() << RESET "'s foods" ENDL;
}

void command_del(string foodname){
	Food* food = Food::find_food(foodname);
	if(!selected_day->del_food(food)){
		cout << ERROR FORMAT_ERROR "'" << foodname << "' is not part of the day's foods" ENDL;
		return;
	}
	cout << "- deleted '" BOLD COLOR_FOOD << foodname << RESET "'";
	cout << " from " BOLD COLOR_DAY << selected_day->get_name() << RESET "'s foods" ENDL;
}

void command_info(string foodname){
	Food* food = Food::find_food(foodname);
	if(food == nullptr){
		cout << ERROR FORMAT_ERROR "unknown food '" << foodname << "'" ENDL;
		return;
	}
	food->print();
}

void command_weight(float weight){
	if(weight < 30.0){
		cout << ERROR FORMAT_ERROR "invalid weight" ENDL;
		return;
	}
	weight_map.insert( {selected_day->get_name(), weight});
	cout << "- set " BOLD COLOR_DAY << selected_day->get_name() << RESET;
	cout << "'s weight to " COLOR_WEIGHT << weight << ENDL;
	save_weights();
}

void command_average(us days){
	if(days < 2){
		cout << ERROR FORMAT_ERROR "invalid days" ENDL;
		return;
	}
	Date date = get_today() - days;
	double total = 0;
	us valid_days = 0;
	for(int i = 0;i < days;i++){
		Day day = Day(date);
		float kcals = day.get_kcals();
		if(kcals > 0){
			total += kcals;
			valid_days++;
		}
		date++;
	}
	if(valid_days == 0){
		cout << "> " COLOR_SYNTAX "not enough" RESET " info to get average" ENDL;
		return;
	}
	cout << "> average over the last " COLOR_AMOUNT << valid_days << RESET " days: ";
	cout << BOLD COLOR_AMOUNT << total / valid_days << RESET " kcal" ENDL;
}

void command_last(us days){
	if(days < 2){
		cout << ERROR FORMAT_ERROR "invalid amount of days" ENDL;
		return;
	}
	string FORMAT_DATE = TAB BOLD COLOR_DATE COLOR_TABLE_BG;
	string FORMAT_KCAL = COLOR_AMOUNT COLOR_TABLE_BG;
	Date date = get_today() - days;
	cout << "> last " COLOR_AMOUNT BOLD << days << RESET " days:" ENDL;
	for(int i = 0;i < days;i++){
		Day day = Day(date);
		float kcals = day.get_kcals();
		cout << FORMAT_DATE << left << setw(20) << day.get_name();
		if(kcals > 0) cout << FORMAT_KCAL << right << setw(8) << day.get_kcals() << ENDL;
		else cout << FORMAT_KCAL << right << setw(8) << "no info" << ENDL;
		date++;
	}
}

void command_help(){
	cout << COLOR_HELP BOLD "- available commands are:" ENDL << left;
	cout << TAB TAB COLOR_HELP << setw(15) << "load" RESET "[DD/MM or DD/MM/YYYY]" ENDL;
	cout << TAB TAB COLOR_HELP << setw(15) << "add" RESET "['foodname'] [amount]" ENDL;
	cout << TAB TAB COLOR_HELP << setw(15) << "set" RESET "['foodname'] [amount]" ENDL;
	cout << TAB TAB COLOR_HELP << setw(15) << "delete" RESET "['foodname']" ENDL;
	cout << TAB TAB COLOR_HELP << setw(15) << "info" RESET "['foodname']" ENDL;
	cout << TAB TAB COLOR_HELP << setw(15) << "weight" RESET "['weight']" ENDL;
	cout << TAB TAB COLOR_HELP << setw(15) << "last" RESET "['days']" ENDL;
	cout << TAB TAB COLOR_HELP << setw(15) << "average" RESET "['days']" ENDL;
	cout << TAB TAB COLOR_HELP "list" ENDL;
	cout << TAB TAB COLOR_HELP "profile" ENDL;
	cout << TAB TAB COLOR_HELP "weights" ENDL;
	//cout << TAB TAB COLOR_HELP "foodlist" ENDL;
	cout << TAB TAB COLOR_HELP "clear" ENDL;
	cout << TAB TAB COLOR_HELP "exit" ENDL;
}

void command_show(){
	selected_day->print();
}

string extract(string source, string start, string end){
	if(source.empty()) return string();
	int pos1, pos2;
	pos1 = source.find(start);
	if(pos1 == -1) return string();
	pos1 += start.length();
	pos2 = (source.substr(pos1)).find(end);
	if(pos2 == -1 || end.empty()) pos2 = source.length();
	return source.substr(pos1, pos2);
}

