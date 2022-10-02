#include "ds.h"
#include <iostream>

DString::DString(){}

DString::DString(int n) : List<char>(n){}

DString::DString(const char *str){
	if (str == nullptr)
		return;
	for (int i = 0; str[i]; i ++)
		append(str[i]);
}

DString::DString(const DString &from) : List<char>(from){}

DString::DString(const DString &from, int start, int end) :
List<char>(end - start){
	for (int i = start; i < end; i ++)
		(*this)[i - start] = from[i];
}

DString::~DString(){
	_free();
}

DString::operator char*() const{
	return array(0);
}

bool DString::operator==(const DString &rhs) const{
	if (rhs.length() != length())
		return false;
	for (int i = 0; i < length(); i ++){
		if ((*this)[i] != rhs[i])
			return false;
	}
	return true;
}

bool DString::operator==(const char *rhs) const{
	for (int i = 0; i < length(); i ++){
		if (rhs[i] == 0 ||(*this)[i] != rhs[i])
			return false;
	}
	return true;
}

bool DString::operator!=(const DString &rhs) const{
	return !(*this == rhs);
}

DString DString::operator+(const DString& rhs) const{
	DString ret;
	ret.length(length() + rhs.length());
	for (int i = 0, end = length(); i < end; i ++)
		ret[i] = (*this)[i];
	for (int i = 0, put = length(), end = rhs.length(); i < end; i ++)
		ret[put ++] = rhs[i];
	return ret;
}

DString& DString::operator=(const DString& rhs){
	_free();
	_copy(rhs);
	return *this;
}

std::ostream& operator<<(std::ostream &stream, const DString &str){
	for (int i = 0; i < str.length(); i ++)
		stream << str[i];
	return stream;
}
