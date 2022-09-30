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
	virtual void _free();
	virtual void _copy(const List<T>&);
public:
	List();
	/// initialise list with n elements
	List(int n);
	/// initialise list with n elements
	List(int n, int allocStep);
	/// Copy constructor
	List(const List &from);
	/// Destructor
	virtual ~List();
	/// Returns: number of elements
	int count() const;
	/// Returns: number of elements
	int length() const;
	/// Set length
	void length(int newLength);
	/// Returns: capacity as of now
	int capacity() const;
	/// Returns: number of appends before allocation happens
	int capacityAppend() const;
	/// Deallocate any extra memory that has been allocated
	void freeExtra();
	/// Appends an element
	/// Returns: index after appending
	int append(const T &element);
	/// Appends a List
	void append(const List<T> &list);
	/// puts an element at index
	/// Returns: true if done, false if out of bounds
	bool put(int index, const T &element);
	/// gets an element from index.
	/// Returns: the element, or dummy element if out of bounds
	T& get(int index);
	/// gets an element from index.
	/// Returns: the element, or dummy element if out of bounds
	T get(int index) const;
	/// finds index at which an element is found.
	/// * data is the element to search for
	/// * start is the startind index for linear search, default to 0
	/// Returns: index of first hit. or -1 in case no hits
	int indexOf(T data, int start = 0) const;
	/// Removes `count` number of elements, starting at `index`
	/// Returns: number of elements removed
	int remove(int index, int count = 1);
	/// subscript operator
	T& operator[](int index);
	/// subscript operator
	T operator[](int index) const;
	/// assignment operator
	List<T>& operator=(const List<T> &rhs);
	/// create a new array from this list, append `postfix` at end of that array
	T* array(T postfix) const;
	/// create a new array from this list
	T* array() const;
	/// Creates another List object, which will contain elements from this
	/// List, starting at index, uptill end-1
	List<T>* slice(int start, int end) const;
};

template <class T>
void List<T>::_free(){
	if (_capacity > 0 && _array != nullptr)
		delete[] _array;
	_array = nullptr;
	_capacity = 0;
	_count = 0;
}

template <class T>
void List<T>::_copy(const List<T> &from){
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

template <class T>
List<T>::List() : _dummy(){
	_array = nullptr;
	_allocStep = 16;
	_capacity = 0;
	_count = 0;
}

template <class T>
List<T>::List(int n) : _dummy(){
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

template <class T>
List<T>::List(int n, int allocStep) : _dummy(){
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

template <class T>
List<T>::List(const List &from){
	List<T>::_copy(from);
}

template <class T>
List<T>::~List(){
	List<T>::_free();
}

template <class T>
int List<T>::count() const{
	return _count;
}

template <class T>
int List<T>::length() const{
	return _count;
}

template <class T>
void List<T>::length(int newLength){
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

template <class T>
int List<T>::capacity() const{
	return _capacity;
}

template <class T>
int List<T>::capacityAppend() const{
	return _capacity - _count;
}

template <class T>
void List<T>::freeExtra(){
	if (_count == _capacity)
		return;
	T *newArr = new T[_count];
	for (int i = 0; i < _count; i ++)
		newArr[i] = _array[i];
	delete[] _array;
	_array = newArr;
	_capacity = _count;
}

template <class T>
int List<T>::append(const T &element){
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

template <class T>
void List<T>::append(const List<T> &list){
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

template <class T>
bool List<T>::put(int index, const T &element){
	if (index < 0 || index >= _count)
		return false;
	_array[index] = element;
	return true;
}

template <class T>
T& List<T>::get(int index){
	if (index < 0 || index >= _count)
		return _dummy;
	return _array[index];
}

template <class T>
T List<T>::get(int index) const{
	if (index < 0 || index >= _count)
		return _dummy;
	return _array[index];
}

template <class T>
int List<T>::indexOf(T data, int start) const{
	for (int i = start; i < _count; i ++){
		if (data == _array[i])
			return i;
	}
	return -1;
}

template <class T>
int List<T>::remove(int index, int count){
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

template <class T>
T& List<T>::operator[](int index){
	return this->get(index);
}

template <class T>
T List<T>::operator[](int index) const{
	return this->get(index);
}

template <class T>
List<T>& List<T>::operator=(const List<T> &rhs){
	_free();
	_copy(rhs);
	return *this;
}

template <class T>
T* List<T>::array(T postfix) const{
	T *arr = new T[_count + 1];
	for (int i = 0; i < _count; i ++)
		arr[i] = _array[i];
	arr[_count] = postfix;
	return arr;
}

template <class T>
T* List<T>::array() const{
	if (_count == 0)
		return nullptr;
	T *arr = new T[_count];
	for (int i = 0; i < _count; i ++)
		arr[i] = _array[i];
	return arr;
}

template <class T>
List<T>* List<T>::slice(int start, int end) const{
	if (start < 0 || end < 0 || start >= _count)
		return nullptr;
	if (end > _count)
		end = _count;
	List<T> *ret = new List<T>(end - start);
	for (int i = start; i < end && i < _count; i ++)
		(*ret)[i - start] = _array[i];
	return ret;
}

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
	/// convert to CString, return that.
	char *cstr() const;
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
protected:
	virtual void _free();
	virtual void _copy(const Queue<T>&);
public:
	/// constructor
	Queue();
	/// copy constructor
	Queue(const Queue&);
	/// destructor
	~Queue();
	/// = operator
	Queue<T>& operator=(const Queue<T>& rhs);
	/// if queue is empty
	bool isEmpty() const;
	/// number of items in queue
	int count() const;
	/// Push to queue
	void push(const T&);
	/// Returns: first item in queue
	const T& peek() const;
	/// Pops first item in queue
	/// Returns: first item in queue
	T pop();
};

template <class T>
void Queue<T>::_free(){
	for (Item *i = _firstItem, *next = nullptr; i ; i = next){
		next = i->ptr;
		delete i;
	}
	_firstItem = _lastItem = nullptr;
	_count = 0;
}

template <class T>
void Queue<T>::_copy(const Queue<T> &from){
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

template <class T>
Queue<T>::Queue(){
	_firstItem = _lastItem = nullptr;
	_count = 0;
}

template <class T>
Queue<T>::Queue(const Queue<T> &from){
	_copy(from);
}

template <class T>
Queue<T>::~Queue(){
	_free();
}

template <class T>
Queue<T>& Queue<T>::operator=(const Queue<T> &rhs){
	_free();
	_copy(rhs);
	return *this;
}

template <class T>
bool Queue<T>::isEmpty() const{
	return _count == 0;
}

template <class T>
int Queue<T>::count() const{
	return _count;
}

template <class T>
void Queue<T>::push(const T &data){
	Item *item = new Item(data, nullptr);
	item->data = data;
	if (_firstItem)
		_lastItem->ptr = item;
	else
		_firstItem = item;
	_lastItem = item;
	_count ++;
}

template <class T>
const T& Queue<T>::peek() const{
	if (_firstItem)
		return _firstItem->data;
	return _dummy;
}

template <class T>
T Queue<T>::pop(){
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

#endif
