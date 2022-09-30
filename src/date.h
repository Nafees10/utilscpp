#ifndef DATE_H
#define DATE_H

#include "ds.h"

class Date{
private:
	int _year, _month, _day;
public:
	/// default constructor
	Date();
	/// constructor from existing values
	Date(int, int, int);
	/// if is valid
	bool isValid();
	/// get year
	int year();
	/// set year, retruns false if invalid
	bool year(int);
	/// gets month
	int month();
	/// set month, return false if invalid
	bool month(int);
	/// gets day
	int day();
	/// sets day. return false if invalid
	bool day(int);
	/// tries reading date from a DD//MM/YYYY string
	/// Returns: true if done, false if invalid string
	bool string(const DString&);
	/// Returns: a new DD/MM/YYYY string representing this Date
	DString string();
	/// == operator
	bool operator==(const Date&);
	/// < operator
	bool operator<(const Date&);
	/// operator >=
	bool operator<=(const Date&);
	/// > operator
	bool operator>(const Date&);
	/// operator <=
	bool operator>=(const Date&);
	/// operator +
	Date operator+(const Date&);
	/// operator -
	Date operator-(const Date&);
};

/// Returns: todays date
Date today();

#endif
