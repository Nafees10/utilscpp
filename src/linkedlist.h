#ifndef LINKEDLIST_H
#define LINKEDLIST_H

/// Swapping data, bypassing type system (no = operators or copy constructors)
template <class T>
void swapBinary(T* a, T* b){
	for (int i = 0; i < sizeof(T); i ++){
		char temp = ((char*)a)[i];
		((char*)a)[i] = ((char*)b)[i];
		((char*)b)[i] = temp;
	}
};

template <class T>
class LinkedList{
private:
	struct Node{
		T data;
		Node *ptr;
	};

	Node *_head;
	Node *_tail;
	int _count;

public:
	LinkedList(){
		_head = _tail = nullptr;
		_count = 0;
	}

	LinkedList(const LinkedList &from){
		_head = _tail = nullptr;
		_count = 0;
		for (auto itr = from.begin(); itr != from.end(); ++ itr)
			this->append(*itr);
	}

	~LinkedList(){
		while (_head){
			Node *next = _head->ptr;
			delete _head;
			_head = next;
		}
	}

	class Iterator{
	private:
		friend class LinkedList;
		Node *_node;

	public:
		Iterator(Node *node) : _node(node) {}
		/// constructors
		Iterator(const Iterator &from) : _node(from._node) {}
		/// assignment operator
		Iterator& operator=(const Iterator &rhs){
			_node = rhs._node;
			return *this;
		}
		
		/// deref operator
		T& operator*(){
			return _node->data;
		}
		const T& operator*() const {
			return _node->data;
		}

		/// pre-increment
		Iterator operator++(){
			if (_node != nullptr)
				_node = _node->ptr;
			return *this;
		}
		const Iterator operator++() const {
			if (_node != nullptr)
					_node = _node->ptr;
			return *this;
		}

		/// post increment
		Iterator operator++(int dummy){
			Iterator temp = *this;
			if (_node != nullptr)
				++(*this);
			return temp;
		}
		const Iterator operator++(int dummy) const {
			Iterator temp = *this;
			if (_node != nullptr)
				++(*this);
			return temp;
		}

		/// not equals
		bool operator!=(Iterator rhs) const {
			return _node != rhs._node;
		}

		/// equals
		bool operator==(Iterator rhs) const {
			return _node == rhs._node;
		}

		operator bool() const {
			return _node != nullptr;
		}
	};

	/// begin iterator
	Iterator begin(){
		return _head;
	}
	const Iterator begin() const {
		return _head;
	}

	/// end iteraotr
	Iterator end() const {
		return nullptr;
	}

	/// Returns: Iterator for node at index, or end iterator in case out of
	/// bounds. negative index will be subtracted from count
	Iterator read(int index){
		if (_count == 0)
			return nullptr;
		if (index < 0)
			index = _count + index;
		if (index == _count - 1)
			return _tail;
		if (index < 0 || index >= _count)
			return nullptr;
		Node *node = _head;
		for (int i = 0; i < index; i ++){
			if (!node) // just in case
				return nullptr;
			node = node->ptr;
		}
		return node;
	}

	const Iterator read(int index) const {
		return const_cast<LinkedList<T>&>(*this).read(index);
	}

	/// Returns: count of elements in linked list
	int count() const {
		return _count;
	}

	/// Clears list, deleting every element
	void clear(){
		while (_head){
			Node *next = _head->ptr;
			delete _head;
			_head = next;
		}
		_head = _tail = nullptr;
		_count = 0;
	}

	/// Appends an element to end
	void append(const T data){
		Node *node = new Node;
		node->data = data;
		node->ptr = nullptr;
		
		if (_head == nullptr){
			_head = node;
			_tail = node;
		}else{
			_tail->ptr = node;
		}
		_tail = node;
		_count ++;
	}

	/// Appends another LinkedList
	void append(const LinkedList<T> &list){
		for (auto val : list)
			append(val);
	}

	/// Returns: Iterator for where a matching element was found
	Iterator find(const T data){
		Node *node = _head;
		while (node){
			if (node->data == data)
				return node;
			node = node->ptr;
		}
		return nullptr;
	}
	const Iterator find(const T data) const {
		return const_cast<LinkedList<T>&>(*this).find(data);
	}

	bool remove(const Iterator prev, const Iterator itr){
		if (itr == nullptr)
			return false;

		if (itr == _head){
			if (_count == 1){
				clear();
				return true;
			}
			
			Node *next = _head->ptr;
			delete _head;
			_head = next;
			_count --;
			return true;
		}
		if (prev == nullptr)
			return false;
		prev._node->ptr = itr._node->ptr;
		delete itr._node;
		_count --;
		if (itr == _tail)
			_tail = prev._node;
		return true;
	}

	/// Deletes an element through its Iterator
	/// Returns: true if deleted, false if not
	bool remove(const Iterator itr){
		if (itr == nullptr)
			return false;

		// deleting head is simple, so special case for that
		if (itr == _head){
			// if only 1 element, get clear() to do the job
			if (_count == 1){
				clear();
				return true;
			}

			Node *next = _head->ptr;
			delete _head;
			_head = next;
			_count --;
			return true;
		}
		// deleting tail is a special case, requiring traversal of entire list:
		if (itr == _tail){
			Node *prev = _head;
			while (prev->ptr != _tail)
				prev = prev->ptr;
			delete _tail;
			_tail = prev;
			return true;
		}

		// move itself to next element
		swapBinary(&(itr._node->data), &(itr._node->ptr->data));
		// commit kil
		Node *nextPtr = itr._node->ptr->ptr;
		delete itr._node->ptr;
		itr._node->ptr = nextPtr;
		_count --;
		return true;
	}

	/// Inserts a node after an iterator
	/// Inserting after nullptr will insert at head
	/// Returns: true if done, false if not (probably list is empty?)
	bool insert(const Iterator itr, T data){
		if (itr == nullptr){
			Node *node = new Node;
			node->data = data;
			node->ptr = _head;
			_head = node;
			if (_count == 0)
				_tail = node;
			_count ++;
			return true;
		}

		if (_head == nullptr)
			return false;

		Node *node = new Node;
		node->data = data;
		node->ptr = itr._node->ptr;
		itr._node->ptr = node;
		_count ++;
		return true;
	}

	/// Returns: true Iterator for element if found, otherwise end iterator
	Iterator exists(const T data){
		Node *node = _head;
		while (node){
			if (node->data == data)
				return node;
			node = node->ptr;
		}
		return nullptr;
	}
	const Iterator exists(const T data) const {
		return const_cast<LinkedList<T>&>(*this).exists(data);
	}
	
	/// Returns: Iterator for an element, searching from after another iterator,
	/// or an end iterator
	Iterator exists(const Iterator itr, const T data){
		Node *node = itr->ptr;
		while (node){
			if (node->data == data)
				return node;
			node = node->ptr;
		}
		return nullptr;
	}
	const Iterator exists(const Iterator itr, const T data) const {
		return const_cast<LinkedList<T>&>(*this).exists(itr, data);
	}

};

#endif
