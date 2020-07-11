/*
 * Date.cpp
 *
 *  Created on: Jul 7, 2020
 *      Author: seba
 */

#include <ctime>
#include <string>
#include "common.h"
#include "date.h"

using namespace std;

bool is_leapyear(us year){
	if(year % 400 == 0) return true;
	if(year % 100 == 0) return false;
	if(year % 4 == 0) return true;
	return false;
}

Date get_previous_day(Date date){
	Date result = date;
	if(result.day != 1){
		result.day--;
		return result;
	}
	if(result.month == 1){
		result.day = 31;
		result.month = 12;
		result.year--;
		return result;
	}
	if(result.month == 2){
		if(is_leapyear(result.year)) result.day = 29;
		else result.day = 28;
		result.month = 1;
		return result;
	}
	if(result.month == 4 || result.month == 6 || result.month == 9 || result.month == 11){
		result.day = 31;
		result.month = 1;
		return result;
	}
	result.day = 30;
	result.month--;
	return result;
}

Date get_next_day(Date date){
	Date result = date;
	if(result.month == 2){
		if(result.day == 29){
			result.day = 1;
			result.month = 3;
			return result;
		}
		if(result.day == 28 && !is_leapyear(result.year)){
			result.day = 1;
			result.month = 3;
			return result;
		}
		result.day++;
		return result;
	}
	if(result.month == 4 || result.month == 6 || result.month == 9 || result.month == 11){
		if(result.day == 30){
			result.day = 1;
			result.month++;
			return result;
		}
		result.day++;
		return result;
	}
	if(result.day == 31){
		result.day = 1;
		if(result.month == 12){
			result.month = 1;
			result.year++;
			return result;
		}
		result.month++;
		return result;
	}
	result.day++;
	return result;
}

Date add_days(Date date, short days){
	if(days == 0) return date;
	if(days > 0) return add_days(get_next_day(date), days - 1);
	return add_days(get_previous_day(date), days + 1);
}

Date get_today(){
	Date result;
	time_t now;
	time(&now);
	struct tm* now_struct = localtime(&now);
	result.day = (*now_struct).tm_mday;
	result.month = (*now_struct).tm_mon + 1;
	result.year = (*now_struct).tm_year + 1900;
	return result;
}

us get_age(Date birth){
	if(!is_valid_date(birth)) return 0;
	struct tm struct_B;
	struct_B.tm_sec = 0;
	struct_B.tm_min = 0;
	struct_B.tm_hour = 0;
	struct_B.tm_mday = birth.day;
	struct_B.tm_mon = birth.month - 1;
	struct_B.tm_year = birth.year - 1900;
	time_t B, N, D;
	B = mktime(&struct_B);
	time(&N);
	D = (time_t) difftime(N, B);
	struct tm* struct_D = localtime(&D);
	struct_D->tm_year -= 1970;
	char str[4];
	strftime(str, 4, "%Y", struct_D);
	return (us) stoi(str);
}

bool is_valid_date(us day, us month, us year){
	if(!(year >= 1900 && year <= 2200)) return false;
	if(day < 1 || day > 31) return false;
	if(month == 2){
		if(is_leapyear(year)){ // leap year
			if(day > 29) return false;
		}
		else if(day > 28) return false;
	}
	if(month % 2 == 0 && day > 30) return false;
	return true;
}

bool is_valid_date(Date date){
	return is_valid_date(date.day, date.month, date.year);
}

