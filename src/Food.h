/*
 * Food.h
 *
 *  Created on: Jul 5, 2020
 *      Author: seba
 */

#ifndef FOOD_H_
#define FOOD_H_

#define EMPTY_ALIAS "<none>"

#include <string>
#include <list>
#include <unordered_map>

namespace std {
	class Food{
		public:
			Food(string name, string alias, string unit, float serving, float kcal, float C, float F, float P);
			string name, alias, unit;
			float serving, kcal, C, F, P;
			virtual ~Food();
			bool operator==(const Food& other_food) const{
				return (name == other_food.name);
			}
			string getName() const;
			string getAlias();
			string getUnit();
			void print();
			void setAlias(string alias);

	};
	template<> struct hash<Food> {
			std::size_t operator()(const Food& food) const{
				using std::size_t;
				using std::hash;
				using std::string;
				return hash<string>()(food.name);
			}
	};
	extern list<Food*> food_list;
	extern unordered_map<string, Food*> alias_map;
}/* namespace std */

#endif /* FOOD_H_ */
