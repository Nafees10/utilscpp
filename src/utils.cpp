#include "utils.h"
#include "ds.h"
#include <iostream>
#include "sha256.h"

#define B64_MASK	0x3F // 00111111
#define B64_OVER	0xC0 // 11000000
#define B64_START	0x30 // 00110000, ascii character 0

bool cnicIsValid(const DString& c){
	return c.length() == 13 && isInteger(c);
}

bool phoneIsValid(const DString& c){
	return c.length() == 12 && isInteger(c);
}

bool idIsValid(const DString& c){
	// confirming with customer (sir), id <999 is not valid
	return c.length() == 4 && isInteger(c) && c[0] != '0';
}

/// Converts binary data, bin, of size n bytes, to a null terminated
/// base 64 string. **This is not a standard base64 implementation**
/// Returns: the base 64 string
DString toBase64(const char* bin, int n){
	DString ret;
	char overflow = 0;
	char overflowCount = 0;
	for (int i = 0; i < n; i ++){
		overflow |= (bin[i] & B64_OVER) >> (++overflowCount * 2);
		ret.append(B64_START + (bin[i] & B64_MASK));
		if (overflowCount == 3){
			ret.append(B64_START + overflow);
			overflow = overflowCount = 0;
		}
	}
	if (overflowCount)
		ret.append(B64_START + overflow);
	return ret;
}

DString hashSHA256(const DString& str){
	char* temp = str.cstr();

	SHA256_CTX ctx;
	unsigned char hash[SHA256_BLOCK_SIZE]; // 32 bytes
	sha256_init(&ctx);
	sha256_update(&ctx, (const BYTE*)temp, strLen(temp));
	sha256_final(&ctx, hash);
	// now make it base64
	return toBase64((char*)hash, SHA256_BLOCK_SIZE);
}

DString strEncode(const DString& str){
	DString ret;
	const int len = str.length();
	for (int i = 0; i < len; i ++){
		const char ch = str[i];
		if (ch == ',' || ch == '\n' || ch == '\r' || ch == '\\'){
			ret.append('\\');
			if (ch == ',')
				ret.append('c');
			else if (ch == '\n')
				ret.append('n');
			else if (ch == '\r')
				ret.append('r');
			else
				ret.append(ch);
		}else{
			ret.append(ch);
		}
	}
	return ret;
}

DString strDecode(const DString& str){
	DString ret;
	const int len = str.length();
	for (int i = 0; i < len; i ++){
		const char ch = str[i], chNext = str[i + 1];
		if (ch != '\\'){
			ret.append(ch);
			continue;
		}
		if (chNext == 0)
			return nullptr; //	user messed with strings.
											//	⠟⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡟⠛⢻⣿
											//	⡆⠊⠈⣿⢿⡟⠛⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣎⠈⠻
											//	⣷⣠⠁⢀⠰⠀⣰⣿⣿⣿⣿⣿⣿⠟⠋⠛⠛⠿⠿⢿⣿⣿⣿⣧⠀⢹⣿⡑⠐⢰
											//	⣿⣿⠀⠁⠀⠀⣿⣿⣿⣿⠟⡩⠐⠀⠀⠀⠀⢐⠠⠈⠊⣿⣿⣿⡇⠘⠁⢀⠆⢀
											//	⣿⣿⣆⠀⠀⢤⣿⣿⡿⠃⠈⠀⣠⣶⣿⣿⣷⣦⡀⠀⠀⠈⢿⣿⣇⡆⠀⠀⣠⣾
											//	⣿⣿⣿⣧⣦⣿⣿⣿⡏⠀⠀⣰⣿⣿⣿⣿⣿⣿⣿⡆⠀⠀⠐⣿⣿⣷⣦⣷⣿⣿
											//	⣿⣿⣿⣿⣿⣿⣿⣿⡆⠀⢰⣿⣿⣿⣿⣿⣿⣿⣿⣿⡄⠀⠀⣿⣿⣿⣿⣿⣿⣿
											//	⣿⣿⣿⣿⣿⣿⣿⣿⡆⠀⣾⣿⣿⠋⠁⠀⠉⠻⣿⣿⣧⠀⠠⣿⣿⣿⣿⣿⣿⣿
											//	⣿⣿⣿⣿⣿⣿⣿⣿⣿⡀⣿⡿⠁⠀⠀⠀⠀⠀⠘⢿⣿⠀⣺⣿⣿⣿⣿⣿⣿⣿
											//	⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⣠⣂⠀⠀⠀⠀⠀⠀⠀⢀⣁⢠⣿⣿⣿⣿⣿⣿⣿⣿
											//	⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣶⣄⣤⣤⣔⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
		if (chNext == 'c')
			ret.append(',');
		else if (chNext == 'n')
			ret.append('\n');
		else if (chNext == 'r')
			ret.append('\r');
		else
			ret.append(chNext);
		i ++;
	}
	return ret;
}

int strLen(const char* str){
	if (str == nullptr)
		return 0;
	int len = 0;
	while (str[len])
		len ++;
	return len;
}

char* strCopy(const char* str, int count){
	if (str == nullptr)
		return nullptr;
	int len = strLen(str);
	if (count > 0 && len > count)
		len = count;
	char *ret = new char[len + 1];
	for (int i = 0; i < len; i ++)
		ret[i] = str[i];
	ret[len] = 0;
	return ret;
}

bool strSame(const char* a, const char* b){
	if (a == nullptr || b == nullptr)
		return a == b;
	while (*a == *b){
		if (*a == 0)
			return true;
		a ++, b ++;
	}
	return false;
}

bool isDigit(char ch){
	return ch >= '0' && ch <= '9';
}

bool isInteger(const DString& s, bool allowSign){
	int len = s.length();
	if (len <= 0)
		return false;
	int i = 0;
	if (allowSign && (s[0] == '+' || s[0] == '-')){
		if (len == 1)
			return false;
		i ++;
	}
	while (i < len){
		if (!isDigit(s[i]))
			return false;
		i ++;
	}
	return true;
}

int strToInt(const DString& str){
	int ret = 0, len = str.length();
	if (!len)
		return 0;
	const bool isNeg = str[0] == '-';
	int endInd = 0;
	if (str[0] == '-' || str[0] == '+'){
		if (len == 1)
			return 0;
		endInd = 1;
	}
	// now time for conversion
	for (int i = len - 1, pow = 1; i >= endInd; i --, pow *= 10)
		ret += (str[i] - 48) * pow;
	if (isNeg)
		ret = -ret;
	return ret;
}

DString intToStr(int num, int digits){
	DString ret;
	if (num == 0)
		return DString("0");
	
	if (num < 0){
		ret.append('-');
		num = -num;	
	}

	int pow = 1;
	while (num % pow != num)
		pow *= 10;
	
	while (pow > 1){
		pow /= 10;
		ret.append(48 + (num / pow % 10));
	}

	if (ret.count() < digits){
		DString str(digits);
		const int startIndex = digits - ret.count();
		for (int i = 0; i < startIndex; i ++)
			str[i] = '0';
		for (int i = startIndex; i < digits; i ++)
			str[i] = ret[i - startIndex];
		str[digits] = 0;
		return str;
	}
	return ret;
}
