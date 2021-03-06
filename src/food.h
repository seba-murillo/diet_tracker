/*
 * Food.h
 *
 *  Created on: Jul 5, 2020
 *      Author: seba
 */

#ifndef FOOD_H_
#define FOOD_H_

#include <string>
#include <list>
#include <map>

#define MAX_FOODNAME_SIZE	50

using namespace std;

class Food{
private:
	static Food* find_by_alias(string foodname);
public:
	Food(string name, string alias, string unit, float serving, float kcal, float C, float F, float P);
	string name, alias, unit;
	float serving, kcal, C, F, P;
	bool operator==(const Food& other_food) const{
		return (name == other_food.name);
	}
	string getName() const;
	string getAlias();
	string getUnit();
	void print();
	void setAlias(string alias);
	static void print_all();
	static Food* find_food(string foodname);
};

extern map<string, Food*> food_map;
extern map<string, Food*> alias_map;

namespace std {
	template<> struct hash<Food> {
		std::size_t operator()(const Food& food) const{
			using std::size_t;
			using std::hash;
			using std::string;
			return hash<string>()(food.name); // @suppress("Ambiguous problem")
		}
	};
}
#endif /* FOOD_H_ */
