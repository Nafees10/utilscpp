#ifndef CSV_H
#define CSV_H

#include "ds.h"

class CSVFile{
private:
	List<DString> _grid;
	List<int> _colCount;
	int _getCellIndex(int, int);
	DString _filename;
	bool _modified;
	DString _dummy;
public:
	CSVFile();
	virtual ~CSVFile();
	/// Read from filename
	bool fileRead(const DString&);
	/// write memory to file
	/// Returns: true if done, false if error
	bool fileWrite();
	/// if file been modified since loading
	bool modified();
	/// number of rows
	int rowCount();
	/// number of columns in specific row
	int colCount(int);
	/// get cell at row, col
	const DString& cell(int, int);
	/// set cell at row, col
	/// Returns: true if done, false if out of bounds
	bool cell(int, int, const DString&);
	/// Appends a row of n columns
	void rowAppend(int);
	/// clears the CSV data in-memory
	void clear();
};

#endif
