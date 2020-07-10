/*
 * common.h
 *
 *  Created on: Jul 7, 2020
 *      Author: seba
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <string>
#include "Date.h"

#define verbose		1
#define FORCE_SPREADSHEET	1
#define INEX		-1

#define DIRECTORY_DAYS		"days"
#define DIRECTORY_MAIN		"DietTracker"

typedef unsigned short us;

using namespace std;

struct profile_structure{
	Date birth;
	us height;
	us weight;
	char S;
	float M;
	us target_macros[3];
};

extern struct profile_structure profile;
extern string extract(string source, string start, string end);

#endif /* COMMON_H_ */
