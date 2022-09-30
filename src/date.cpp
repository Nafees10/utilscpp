#include "date.h"
#include "utils.h"
#include <ctime>

bool yearIsLeap(int year){
	if((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
		return true;
	else
		return false;
}

int daysInMonth(int month ,int year){
	if(month == 1){
		return 31;
	}else if(month == 2){
		if(yearIsLeap(year))
			return 29;
		else
			return 28;
	}else if(month == 3)
		return 31;
	else if(month == 4)
		return 30;
	else if(month == 5)
		return 31;
	else if(month == 6)
		return 30;
	else if(month == 7)
		return 31;
	else if(month == 8)
		return 31;
	else if(month == 9)
		return 30;
	else if(month == 10)
		return 31;
	else if(month == 11)
		return 30;
	else if(month == 12)
		return 31;
	return -1;
}


Date::Date(){
	_year = _month = _day = -1;
}

Date::Date(int d, int m, int y){
	_year = y;
	_month = m;
	_day = d;
}

bool Date::isValid(){
	return _year > 0 && 
		_month > 0 && _month <= 12 &&
		_day > 0 && _day < daysInMonth(_month, _year);
}

int Date::year(){
	return _year;
}

bool Date::year(int y){
	if (y < 0)
		return false;
	_year = y;
	return true;
}

int Date::month(){
	return _month;
}

bool Date::month(int m){
	if (m < 1 || m > 12)
		return false;
	_month = m;
	return true;
}

int Date::day(){
	return _day;
}

bool Date::day(int d){
	if (d < 1 || d > daysInMonth(_month, _year))
		return false;
	_day = d;
	return true;
}

bool Date::string(const DString& str){
	if (str.length() != 10){
		throw DString("Invalid string to read date from: ") + str;
		return false;
	}
	// index 0, 1 DD;  3,4 MM;  6,7,8,9 YYYY
	// do year first, then month, and at last, day
	bool flag = true;
	DString tempStr;
	DString yearStr = DString(str, 6, 10),
			monthStr = DString(str, 3, 5),
			dayStr = DString(str, 0, 2);
	
	if (!isInteger(yearStr) || !isInteger(monthStr) || !isInteger(dayStr)){
		throw DString("Invalid string to read date from '") + str +
			DString("' Values are not integers.");
		return false;
	}
	
	flag = year(strToInt(yearStr)) && month(strToInt(monthStr)) &&
		day(strToInt(dayStr));

	if (!flag){
		throw DString("Invalid string to read date from '") + str +
			DString("' Month or day out of range.");
	}

	return flag;
}

DString Date::string(){
	if (!isValid())
		return DString();
	DString str;
	for (int i = 0; i < 10; i ++)
		str[i] = '0';
	str[2] = str[5] = '/';
	// day
	str.append(intToStr(_day, 2));
	str.append('/');
	str.append(intToStr(_month, 2));
	str.append('/');
	str.append(intToStr(_year, 4));
	return str;
}

bool Date::operator==(const Date& rhs){
	return _year == rhs._year && _month == rhs._month && _day == rhs._day;
}

bool Date::operator<(const Date& rhs){
	return _year < rhs._year || 
		(_year == rhs._year && (_month < rhs._month ||
		(_month == rhs._month && _day < rhs._day)));
}

bool Date::operator<=(const Date& rhs){
	return *this < rhs || *this == rhs;
}

bool Date::operator>(const Date& rhs){
	return _year > rhs._year || 
		(_year == rhs._year && (_month > rhs._month ||
		(_month == rhs._month && _day > rhs._day)));
}

bool Date::operator>=(const Date& rhs){
	return *this > rhs || *this == rhs;
}

Date localtimeSafe(time_t &t){
	Date ret;
#ifdef __linux__
	tm *tmPtr = localtime(&t);
	ret.year(tmPtr->tm_year + 1900);
	ret.month(tmPtr->tm_mon + 1);
	ret.day(tmPtr->tm_mday);
#else
	tm tmPtr;
	localtime_s(&tmPtr,&t);

	ret.year(tmPtr.tm_year + 1900);
	ret.month(tmPtr.tm_mon + 1);
	ret.day(tmPtr.tm_mday);
#endif
	return ret;
}

Date Date::operator+(const Date &rhs){
	tm time = {0};
	time.tm_year = (_year + rhs._year) - 1900;
	time.tm_mon = (_month + rhs._month) - 1;
	time.tm_mday = _day + rhs._day;

	time_t t = mktime(&time);
	return localtimeSafe(t);
}

Date Date::operator-(const Date &rhs){
	tm time = {0};
	time.tm_year = (_year - rhs._year) - 1900;
	time.tm_mon = (_month - rhs._month) - 1;
	time.tm_mday = _day - rhs._day;

	time_t t = mktime(&time);
	return localtimeSafe(t);
}

Date today(){
	time_t now = time(nullptr);
	return localtimeSafe(now);
}
