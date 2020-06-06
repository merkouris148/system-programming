#ifndef TIME_H

#define TIME_H

#include <iostream>

#include "../lib_containers/string.h"

class Time{
private:
	int year;
	int month;
	int day;
	int hour;	//24
	int min;
public:
//constructor destructor
	Time();				//default constructor sets to now
	//Time(const String& str);
	Time(int dday, int mmonth, int yyear, int hhour, int mmin);

//accessors
	int get_year()	const	{return year;};
	int get_month()	const	{return month;};
	int get_day()	const	{return day;};
	int get_hour()	const	{return hour;};
	int get_min()	const	{return min;};
};


//relations
bool operator==(const Time& t1, const Time& t2);
bool operator<(const Time& t1, const Time& t2);

//show
std::ostream& operator<<(std::ostream& out, const Time& t);

//predicates
bool is_day(int d);
bool is_mon(int m); 
bool is_yea(int y); 

bool is_hou(int h); 
bool is_min(int m); 
 

#endif
