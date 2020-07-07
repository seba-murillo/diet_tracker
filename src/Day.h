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
#include <unordered_map>
#include "Food.h"

typedef unsigned short us;

namespace std {
	struct date_structure{
		us day;
		us month;
		us year;
	};
	class Day{
		private:
			//https://thispointer.com/unordered_map-usage-tutorial-and-example/
			unordered_map<Food*, float> day_food_map;
			struct date_structure date;
		public:
			Day(us day, us month, us year);
			virtual ~Day();
			bool addFood(Food* food, float amount);
			bool setFood(Food* food, float amount);
			bool delFood(Food* food);
			date_structure getDate();
			float getCals();
			void print();
			void save();
			string getName();
	};
}/* namespace std */

#endif /* DAY_H_ */
