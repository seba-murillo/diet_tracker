/*
 * Date.cpp
 *
 *  Created on: Jul 7, 2020
 *      Author: seba
 */

#include <ctime>
#include <iomanip>
#include "date.h"

bool operator<(const Date& date1, const Date& date2){
	if(date1.year < date2.year) return true;
	if(date1.year > date2.year) return false;
	if(date1.month < date2.month) return true;
	if(date1.month > date2.month) return false;
	if(date1.day < date2.day) return true;
	if(date1.day > date2.day) return false;
	return false;
}

bool operator>(const Date& date1, const Date& date2){
	return (date2 < date1);
}

bool operator<=(const Date& date1, const Date& date2){
	return !(date1 > date2);
}

bool operator>=(const Date& date1, const Date& date2){
	return !(date1 < date2);
}

bool operator==(const Date& date1, const Date& date2){
	if(date1.year != date2.year) return false;
	if(date1.month != date2.month) return false;
	if(date1.day != date2.day) return false;
	return true;
}

bool operator!=(const Date& date1, const Date& date2){
	return !(date1 == date2);
}

std::ostream& operator<<(std::ostream& stream, const Date& date){
	stream << std::right << std::setfill('0') << std::setw(2) << date.day << "/";
	stream << std::setfill('0') << std::setw(2) << date.month << "/";
	stream << std::setfill('0') << std::setw(4) << date.year << std::setfill(' ');
	return stream;
}

std::istream& operator>>(std::istream& stream, Date& date){
	char slash = '/';
	stream >> date.day;
	if(stream.peek() == slash) stream >> slash;
	stream >> date.month;
	if(stream.peek() == slash) stream >> slash;
	stream >> date.year;
	return stream;
}

Date& operator--(Date& date){ // prefix
	if(date.day != 1){
		date.day--;
		return date;
	}
	if(date.month == 1){
		date.day = 31;
		date.month = 12;
		date.year--;
		return date;
	}
	if(date.month == 2){
		if(is_leapyear(date.year)) date.day = 29;
		else date.day = 28;
		date.month = 1;
		return date;
	}
	if(date.month == 4 || date.month == 6 || date.month == 9 || date.month == 11){
		date.day = 31;
		date.month--;
		return date;
	}
	date.day = 30;
	date.month--;
	return date;
}

Date operator--(Date& date, int){ // posfix
	Date value = date;
	--date;
	return value;
}

Date& operator++(Date& date){ // prefix
	if(date.month == 2){
		if(date.day == 29){
			date.day = 1;
			date.month = 3;
			return date;
		}
		if(date.day == 28 && !is_leapyear(date.year)){
			date.day = 1;
			date.month = 3;
			return date;
		}
		date.day++;
		return date;
	}
	if(date.month == 4 || date.month == 6 || date.month == 9 || date.month == 11){
		if(date.day == 30){
			date.day = 1;
			date.month++;
			return date;
		}
		date.day++;
		return date;
	}
	if(date.day == 31){
		date.day = 1;
		if(date.month == 12){
			date.month = 1;
			date.year++;
			return date;
		}
		date.month++;
		return date;
	}
	date.day++;
	return date;
}

Date operator++(Date& date, int){ // posfix
	Date value = date;
	++date;
	return value;
}

Date& operator+=(Date& date, int days){
	if(days > 0) for(int i = 0;i < days;i++)
		date++;
	else for(int i = days;i < 0;i++)
		date--;
	return date;
}

Date operator+(Date date, int days){
	date += days;
	return date;
}

Date& operator-=(Date& date, int days){
	if(days > 0) for(int i = 0;i < days;i++)
		date--;
	else for(int i = days;i < 0;i++)
		date++;
	return date;
}

Date operator-(Date date, int days){
	date -= days;
	return date;
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

Date get_blank_date(){
	Date date;
	date.day = 0;
	date.month = 0;
	date.year = 0;
	return date;
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
	return (us) std::stoi(str);
}

bool is_valid_date(us day, us month, us year){
	if(!(year >= 1900 && year <= 2200)) return false;
	if(month < 1 || month > 12) return false;
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

bool is_leapyear(us year){
	if(year % 400 == 0) return true;
	if(year % 100 == 0) return false;
	if(year % 4 == 0) return true;
	return false;
}

