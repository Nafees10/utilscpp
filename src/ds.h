#ifndef DS_H
#define DS_H

#include <iostream>

template <class T>
class List{
private:
	T* _array;
	T _dummy;
	int _capacity;
	int _count;
	int _allocStep;

protected:
	virtual void _free(){
		if (_capacity > 0 && _array != nullptr)
			delete[] _array;
		_array = nullptr;
		_capacity = 0;
		_count = 0;
	}

	virtual void _copy(const List<T> &from){
		_allocStep = from._allocStep;
		_capacity = from._count;
		_count = from._count;
		if (_count > 0){
			_array = new T[_count];
			for (int i = 0; i < _count; i ++)
				_array[i] = from._array[i];
		}else{
			_array = nullptr;
		}
	}

public:
	List() : _dummy(){
		_array = nullptr;
		_allocStep = 16;
		_capacity = 0;
		_count = 0;
	}

	/// initialise list with n elements
	List(int n) : _dummy(){
		_allocStep = 16;
		if (n <= 0){
			_array = nullptr;
			_capacity = 0;
			_count = 0;
		}else{
			_array = new T[n];
			_capacity = n;
			_count = n;
		}
	}

	/// initialise list with n elements
	List(int n, int allocStep) : _dummy(){
		if (allocStep <= 0)
			allocStep = 16;
		if (n <= 0){
			_array = nullptr;
			_capacity = 0;
			_count = 0;
		}else{
			_array = new T[n];
			_capacity = n;
			_count = n;
		}
		_allocStep = allocStep;
	}

	/// Copy constructor
	List(const List &from){
		List<T>::_copy(from);
	}

	/// Destructor
	virtual ~List(){
		List<T>::_free();
	}

	/// Returns: number of elements
	int count() const {
		return _count;
	}

	/// Returns: number of elements
	int length() const {
		return _count;
	}

	/// Set length
	void length(int newLength){
		if (newLength < 0)
			return;
		// if its less than capacity, just set count
		if (newLength <= _capacity){
			_count = newLength;
			return;	
		}
		T *newArr = new T[newLength];
		for (int i = 0; i < _count; i ++)
			newArr[i] = _array[i];
		delete[] _array;
		_array = newArr;
		_capacity = newLength;
		_count = newLength;
	}

	/// Returns: capacity as of now
	int capacity() const {
		return _capacity;
	}

	/// Returns: number of appends before allocation happens
	int capacityAppend() const {
		return _capacity - _count;
	}

	/// Deallocate any extra memory that has been allocated
	void freeExtra(){
		if (_count == _capacity)
			return;
		T *newArr = new T[_count];
		for (int i = 0; i < _count; i ++)
			newArr[i] = _array[i];
		delete[] _array;
		_array = newArr;
		_capacity = _count;
	}

	/// Appends an element
	/// Returns: index after appending
	int append(const T &element){
		// check if need to grow
		if (_count == _capacity){
			T *newArr = new T[_capacity + _allocStep];
			for (int i = 0; i < _capacity; i ++)
				newArr[i] = _array[i];
			delete[] _array;
			_array = newArr;
			_capacity += _allocStep;
		}
		const int index = _count ++;
		_array[index] = element;
		return index;
	}

	/// Appends a List
	void append(const List<T> &list){
		if (list.count() == 0)
			return;
		if (&list == this)
			return;
		// check if need to grow
		if (capacityAppend() < list.count()){
			const int newCap = _count + list._count;
			T *newArr = new T[newCap];
			for (int i = 0; i < _count; i ++)
				newArr[i] = _array[i];
			delete[] _array;
			_array = newArr;
			_capacity = newCap;
		}
		for (int i = 0; i < list._count; i ++)
			_array[_count + i] = list._array[i];
		_count += list._count;
	}

	/// puts an element at index
	/// Returns: true if done, false if out of bounds
	bool put(int index, const T &element){
		if (index < 0 || index >= _count)
			return false;
		_array[index] = element;
		return true;
	}

	/// gets an element from index.
	/// Returns: the element, or dummy element if out of bounds
	T& get(int index){
		if (index < 0 || index >= _count)
			return _dummy;
		return _array[index];
	}

	/// gets an element from index.
	/// Returns: the element, or dummy element if out of bounds
	const T& get(int index) const {
		if (index < 0 || index >= _count)
			return _dummy;
		return _array[index];
	}

	/// finds index at which an element is found.
	/// * data is the element to search for
	/// * start is the startind index for linear search, default to 0
	/// Returns: index of first hit. or -1 in case no hits
	int indexOf(T data, int start = 0) const {
		for (int i = start; i < _count; i ++){
			if (data == _array[i])
				return i;
		}
		return -1;
	}

	/// Removes `count` number of elements, starting at `index`
	/// Returns: number of elements removed
	int remove(int index, int count = 1){
		if (index >= _count)
			return 0;
		int ret = count;
		if (index + ret > _count)
			ret = _count - index;
		if (ret == 0)
			return 0;
		_count -= ret;
		for (int i = index; i < _count; i ++)
			_array[i] = _array[ret + i];
		return ret;
	}

	/// subscript operator
	T& operator[](int index){
		return this->get(index);
	}

	/// subscript operator
	const T& operator[](int index) const {
		return this->get(index);
	}

	/// assignment operator
	List<T>& operator=(const List<T> &rhs){
		_free();
		_copy(rhs);
		return *this;
	}

	/// create a new array from this list, append `postfix` at end of that array
	T* array(T postfix) const {
		T *arr = new T[_count + 1];
		for (int i = 0; i < _count; i ++)
			arr[i] = _array[i];
		arr[_count] = postfix;
		return arr;
	}

	/// create a new array from this list
	T* array() const {
		if (_count == 0)
			return nullptr;
		T *arr = new T[_count];
		for (int i = 0; i < _count; i ++)
			arr[i] = _array[i];
		return arr;
	}

	/// Creates another List object, which will contain elements from this
	/// List, starting at index, uptill end-1
	List<T>* slice(int start, int end) const {
		if (start < 0 || end < 0 || start >= _count)
			return nullptr;
		if (end > _count)
			end = _count;
		List<T> *ret = new List<T>(end - start);
		for (int i = start; i < end && i < _count; i ++)
			(*ret)[i - start] = _array[i];
		return ret;
	}

};

/// A dynamic, memory safe, non-null terminated string
class DString : public List<char>{
public:
	/// default constructor
	DString();
	/// with n random characters
	DString(int n);
	/// construct from cstring
	DString(const char* str);
	/// copy constructor
	DString(const DString&);
	/// copy slice constructor
	DString(const DString& from, int start, int end);
	/// Destructor
	virtual ~DString();
	/// convert to new CString, return that
	operator char*() const;
	/// == operator
	bool operator==(const DString&) const;
	/// == operator
	bool operator==(const char*) const;
	/// != operator
	bool operator!=(const DString&) const;
	/// + operator
	DString operator+(const DString&) const;
	/// = operator
	DString& operator=(const DString&);
};

std::ostream& operator<<(std::ostream&, const DString&);

/// A Queue based on linked list
/// based on https://github.com/Nafees10/utils	ds.h->FIFOStack
template <class T>
class Queue{
private:
	// storing item, with reference to next item
	struct Item{
		T data;
		Item *ptr;
		Item (const T& d, Item *p) : data(d), ptr(p){};
	};

	Item *_firstItem, *_lastItem;
	T _dummy;
	int _count;

	void _free(){
		for (Item *i = _firstItem, *next = nullptr; i ; i = next){
			next = i->ptr;
			delete i;
		}
		_firstItem = _lastItem = nullptr;
		_count = 0;
	}

	void _copy(const Queue<T> &from){
		if (from.isEmpty()){
			_firstItem = nullptr;
			_lastItem = nullptr;
			return;
		}

		_firstItem = new Item(from._firstItem->data, nullptr);
		_lastItem = _firstItem;
		for (Item *i = from._firstItem->ptr; i; i = i->ptr){
			_lastItem->ptr = new Item(i->data, nullptr);
			_lastItem = _lastItem->ptr;
		}
		_count = from._count;
	}

public:
	/// constructor
	Queue(){
		_firstItem = _lastItem = nullptr;
		_count = 0;
	}

	/// copy constructor
	Queue(const Queue &from){
		_copy(from);
	}

	/// destructor
	~Queue(){
		_free();
	}

	/// = operator
	Queue<T>& operator=(const Queue<T>& rhs){
		_free();
		_copy(rhs);
		return *this;
	}

	/// if queue is empty
	bool isEmpty() const {
		return _count == 0;
	}

	/// number of items in queue
	int count() const {
		return _count;
	}

	/// Push to queue
	void push(const T &data){
		Item *item = new Item(data, nullptr);
		item->data = data;
		if (_firstItem)
			_lastItem->ptr = item;
		else
			_firstItem = item;
		_lastItem = item;
		_count ++;
	}

	/// Returns: first item in queue
	const T& peek() const{
		if (_firstItem)
			return _firstItem->data;
		return _dummy;
	}

	/// Pops first item in queue
	/// Returns: first item in queue
	T pop(){
		if (!_firstItem)
			return _dummy;
		Item *ptr = _firstItem;
		T ret = ptr->data;
		
		_firstItem = ptr->ptr;
		delete ptr;
		
		if (!_firstItem)
			_lastItem = nullptr;
		_count --;
		return ret;
	}
};

#endif
