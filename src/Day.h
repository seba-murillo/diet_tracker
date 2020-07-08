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
	bool loaded = false;
	map<Food*, float> day_food_map;
	Date date;
public:
	Day(us day, us month, us year);
	virtual ~Day();
	bool addFood(Food* food, float amount);
	bool setFood(Food* food, float amount);
	bool delFood(Food* food);
	date_structure getDate();
	float getKcals();
	void print();
	void save();
	string getName();
};

bool day_exists(Date date);

#endif /* DAY_H_ */
