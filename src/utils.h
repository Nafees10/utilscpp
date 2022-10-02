#ifndef UTILS_H
#define UTILS_H

#include "ds.h"

/// Converts binary data, bin, of size n bytes, to a DString storing a
/// base 64 string. **This is not a standard base64 implementation**
/// Returns: the base 64 string
DString toPsuedoBase64(const char* bin, int n);

/// Returns: a base64 string representing the 32 byte SHA256 hash for a string
DString hashSHA256(const DString& str);

/// Returns: length of CString
int strLen(const char*);

/// Returns: a new CString containing contents of existing one
char* strCopy(const char*, int count = -1);

/// Returns: true if both strings are same, or if both are nullptr
bool strSame(const char*, const char*);

/// Checks if a character is a digit
bool isDigit(char);

/// Returns: true if a string contains an integer
/// will allow + or - to be first character if `allowSign`
bool isInteger(const DString& s, bool allowSign = false);

/// Parses a string into an integer.
/// Returns: the integer parsed
int strToInt(const DString&);

/// Creates a new string representing an integer
/// Returns: the string
DString intToStr(int num, int digits = 0);

#endif
