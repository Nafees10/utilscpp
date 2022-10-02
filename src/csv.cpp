#include "csv.h"
#include "utils.h"
#include "ds.h"

#include <fstream>
#include <iostream>

int CSVFile::_getCellIndex(int row, int col){
	if (row < 0 || row >= _colCount.count() || col < 0 ||
	col >= _colCount[row])
		return -1;
	int index = 0;
	for (int i = 0; i < row; i ++)
		index += _colCount[i];
	return index + col;
}

CSVFile::CSVFile(){
	_modified = false;
}

CSVFile::CSVFile(const CSVFile &from) : _grid(from._grid),
	_colCount(from._colCount), _filename(from._filename),
	_modified(from._modified) {}

CSVFile::~CSVFile(){}

bool CSVFile::fileRead(const DString& filename){
	_filename = filename;
	char *temp = filename.cstr();
	std::ifstream file(temp);
	delete[] temp;

	if (!file){
		throw DString("Failed to open file ") + filename;
	}
	this->clear();
	DString word;
	_colCount.append(0);
	bool prevWasComma = false;
	while (!file.eof()){
		char ch;
		file.get(ch);
		if (ch == '\r' || ch == '\n' || ch == ',' || file.eof() ||
		ch == 0){
			if (word.count() || prevWasComma){
				DString thisWord = csvStrDecode(word);
				word.length(0);
				_grid.append(thisWord);
				_colCount[_colCount.count() - 1] ++;
			}
			if ((ch == '\r' || ch == '\n') &&
			_colCount[_colCount.count() - 1] != 0)
				_colCount.append(0);
		}else{
			word.append(ch);
		}
		prevWasComma = ch == ',';
	}
	file.close();
	// if last count in colLengths is 0, remove it
	while (_colCount.count() && _colCount[_colCount.count() - 1] == 0)
		_colCount.length(_colCount.count() - 1);
	_modified = false;
	return true;
}

bool CSVFile::fileWrite(){
	if (!_modified)
		return true;
	char *temp = _filename.cstr();
	std::ofstream file(temp);
	delete[] temp;

	if (!file){
		throw DString("Failed to open file ") + _filename;
	}
	for (int r = 0; r < _colCount.count(); r ++){
		for (int c = 0; c < _colCount[r]; c ++){
			const int index = _getCellIndex(r, c);
			if (index < 0)
				continue;
			DString encodedStr = csvStrEncode(_grid[index]);
			file << encodedStr;
			if (c + 1 < _colCount[r])
				file << ',';
		}
		file << std::endl;
	}
	file.close();
	_modified = false;
	return true;
}

bool CSVFile::modified(){
	return _modified;
}

int CSVFile::rowCount(){
	return _colCount.count();
}

int CSVFile::colCount(int index){
	if (index < 0 || index >= _colCount.count())
		return false;
	return _colCount[index];
}

const DString& CSVFile::cell(int row, int col){
	int index = _getCellIndex(row, col);
	if (index < 0)
		return _dummy;
	return _grid[index];
}

bool CSVFile::cell(int row, int col, const DString& newVal){
	int index = _getCellIndex(row, col);
	if (index < 0)
		return false;
	if (_grid[index] == newVal)
		return true;
	_grid[index] = newVal;
	_modified = true;
	return true;
}

void CSVFile::rowAppend(int cols){
	_colCount.append(cols);
	for (int i = 0; i < cols; i ++)
		_grid.append(DString());
	_modified = true;
}

void CSVFile::clear(){
	// if already 0, being called twice, then have lists free allocated
	// memory too
	if (_grid.count() == 0){
		_grid.freeExtra();
		_colCount.freeExtra();
		return;
	}
	_grid.length(0);
	_colCount.length(0);
}

DString csvStrEncode(const DString& str){
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

DString csvStrDecode(const DString& str){
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
