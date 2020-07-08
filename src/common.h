/*
 * common.h
 *
 *  Created on: Jul 7, 2020
 *      Author: seba
 */

#ifndef COMMON_H_
#define COMMON_H_

#define verbose		1
#define INEX		-1

#define FILENAME_PROFILE 				".profile"
#define FILENAME_WEIGHTS				".weights"
#define FILENAME_FOOD_DATA 				".food_data"
#define FILENAME_FOOD_SPREADSHEET 		"food_data.ods"

#define DIRECTORY_DAYS					"days"
#define DIRECTORY_MAIN					"DietTracker"

#include <string>
#include "Date.h"

typedef unsigned short us;

using namespace std;

struct profile_structure{
	Date birth;
	us height = 0;
	us weight = 0;
	char S = '?';
	float M = 1.0;
	us target_macros[3] = {0, 0, 0};
};

extern struct profile_structure profile;
extern string extract(string source, string start, string end);

#endif /* COMMON_H_ */
