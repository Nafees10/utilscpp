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
	CSVFile(const CSVFile&);
	virtual ~CSVFile();
	/// Read from filename
	virtual bool fileRead(const DString&);
	/// write memory to file
	/// Returns: true if done, false if error
	virtual bool fileWrite();
	/// if file been modified since loading
	virtual bool modified();
	/// number of rows
	virtual int rowCount();
	/// number of columns in specific row
	virtual int colCount(int);
	/// get cell at row, col
	virtual const DString& cell(int, int);
	/// set cell at row, col
	/// Returns: true if done, false if out of bounds
	virtual bool cell(int, int, const DString&);
	/// Appends a row of n columns
	virtual void rowAppend(int);
	/// clears the CSV data in-memory
	virtual void clear();
};

/// Encodes a string for being saved in a CSV file
DString csvStrEncode(const DString&);

/// Decodes a string that was encoded by csvStrEncode
DString csvStrDecode(const DString&);

#endif
