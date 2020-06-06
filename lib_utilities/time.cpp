#include "time.h"

#include <ctime>
#include <cassert>
#include <cctype>

#include <iostream>

//using namespace std;

//constructor destructor
Time::Time(){
	time_t now = time(0);
	tm* local_tm = localtime(&now);
	
	year 	= local_tm->tm_year + 1900;
	month	= local_tm->tm_mon + 1;
	day		= local_tm->tm_mday;
	hour	= local_tm->tm_hour;
	min		= local_tm->tm_min;
}

bool is_time_srt(const String& str){
	bool check1 = str.get_length() == 16 and str[2] == '-' and str[5] == '-' and str[10] == ' ' and str[13] == ':';
	
	bool check2 = true;
	for(int i = 0; i <= 15; ++i){
		if(i == 2 or i == 5 or i == 10 or i == 13)  continue;
		
		check2 = check2 and isdigit(str[i]);
	}
	
	return check1 and check2;
}

/*
Time::Time(const String& str){
	cout << "Time: " << str << endl;
	
	assert(is_time_srt(str));
	
	String sday = str(0,1);		
	String smon = str(3,4);		
	String syea = str(6,9);		
	String shou = str(11,12);	
	String smin = str(14,15);	
	
	year 	= syea.stoi();	assert(1900 <= year);
	month	= smon.stoi();	assert(1 <= month && month <= 12);
	day		= sday.stoi();	assert(1 <= day && day <= 31);
	hour	= shou.stoi();	assert(0 <= hour && hour <= 24);
	min		= smin.stoi();	assert(0 <= min && min <= 59);
}
* */

Time::Time(int dday, int mmonth, int yyear, int hhour, int mmin){	
	year 	= yyear;	assert(1900 <= year);
	month	= mmonth;	assert(1 <= month && month <= 12);
	day		= dday;		assert(1 <= day && day <= 31);
	hour	= hhour;	assert(0 <= hour && hour <= 24);
	min		= mmin;		assert(0 <= min && min <= 59);
}

//relations
bool operator==(const Time& t1, const Time& t2){
	return 	t1.get_year() 	==	t2.get_year() 	and
			t1.get_month() 	==	t2.get_month() 	and
			t1.get_day()	==	t2.get_day()	and
			t1.get_hour()	==	t2.get_hour()	and
			t1.get_min()	==	t2.get_min();
}

bool operator<(const Time& t1, const Time& t2){
	return 
		t1.get_year()	<	t2.get_year()
		or
		(t1.get_year()	==	t2.get_year() and t1.get_month() < t2.get_month())
		or
		(t1.get_year()	==	t2.get_year() and t1.get_month() == t2.get_month() and t1.get_day() < t2.get_day())
		or
		(t1.get_year()	==	t2.get_year() and t1.get_month() == t2.get_month() and t1.get_day() == t2.get_day() and t1.get_hour() < t2.get_hour())
		or
		(t1.get_year()	==	t2.get_year() and t1.get_month() == t2.get_month() and t1.get_day() == t2.get_day() and t1.get_hour() == t2.get_hour() and t1.get_min() < t2.get_min());
}


//show
std::ostream& operator<<(std::ostream& out, const Time& t){
	out << t.get_day() << "-" << t.get_month() << "-" << t.get_year() << " " << t.get_hour() << ":" << t.get_min() << std::endl;

	return out;
}

//predicates
bool is_day(int d) {return 1 <= d and d <= 31;} 
bool is_mon(int m) {return 1 <= m and m <= 12;} 
bool is_yea(int y) {return 1900 <= y;}

bool is_hou(int h) {return 0 <= h and h <= 24;} 
bool is_min(int m) {return 0 <= m and m <= 59;} 
