/*
 * common.h
 *
 *  Created on: Jul 7, 2020
 *      Author: seba
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <string>

#include "date.h"

//#define debug		1
#define verbose		1

#define INEX		-1

#define DIRECTORY_MAIN		"DietTracker"
#define DIRECTORY_DAYS		"days"

using namespace std;

typedef unsigned short us;

struct profile_structure{
	Date birth;
	us height;
	char S;
	float weight;
	float A;
	us target_macros[3];
};

extern struct profile_structure profile;
extern string extract(string source, string start, string end);

#endif /* COMMON_H_ */
