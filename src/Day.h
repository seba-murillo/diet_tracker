/*
 * Day.h
 *
 *  Created on: Jul 5, 2020
 *      Author: seba
 */

#ifndef DAY_H_
#define DAY_H_

#include <ctime>
#include <string>
#include <map>
#include "common.h"
#include "Food.h"
#include "Date.h"

using namespace std;

class Day{
private:
	void load();
	//bool loaded = false;
	map<Food*, float> day_food_map;
	Date date;
public:
	Day(us day, us month, us year);
	Day(Date date);
	bool add_food(Food* food, float amount);
	bool set_food(Food* food, float amount);
	bool del_food(Food* food);
	date_structure get_date();
	float get_kcals();
	void print();
	void save();
	string get_name();
};

bool day_exists(Date date);

#endif /* DAY_H_ */
