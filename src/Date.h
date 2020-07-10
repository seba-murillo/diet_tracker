/*
 * Date.h
 *
 *  Created on: Jul 7, 2020
 *      Author: seba
 */

#ifndef DATE_H_
#define DATE_H_

typedef unsigned short us;

typedef struct date_structure {
	us day;
	us month;
	us year;
} Date;

Date get_previous_day(Date date);
Date get_next_day(Date date);
Date get_today();
us get_age(Date birth);
bool isValidDate(us day, us month, us year);
bool isValidDate(Date date);
bool is_leapyear(us year);

//double get_time_since(Date date);


#endif /* DATE_H_ */
