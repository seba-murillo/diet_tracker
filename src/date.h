/*
 * Date.h
 *
 *  Created on: Jul 7, 2020
 *      Author: seba
 */

#ifndef DATE_H_
#define DATE_H_

#include <iostream>

typedef unsigned short us;

typedef struct date_structure{
	us day;
	us month;
	us year;
} Date;

// operators
bool operator<(const Date& date1, const Date& date2);
bool operator>(const Date& date1, const Date& date2);
bool operator<=(const Date& date1, const Date& date2);
bool operator>=(const Date& date1, const Date& date2);
bool operator==(const Date& date1, const Date& date2);
bool operator!=(const Date& date1, const Date& date2);
std::ostream& operator<<(std::ostream& stream, const Date& date);
std::istream& operator>>(std::istream& is, Date& date);
Date& operator--(Date& date);
Date operator--(Date& date, int);
Date& operator++(Date& date);
Date operator++(Date& date, int);
Date& operator+=(Date& date, int days);
Date operator+(Date date, int days);
Date& operator-=(Date& date, int days);
Date operator-(Date date, int days);
// functions
Date get_today();
Date get_blank_date();
us get_age(Date birth);
bool is_valid_date(us day, us month, us year);
bool is_valid_date(Date date);
bool is_leapyear(us year);

//double get_time_since(Date date);

#endif /* DATE_H_ */
