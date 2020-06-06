#ifndef LIST_H

#define LIST_H

#include <iostream>

template<typename ListType>
class List;

template <typename T>
class ListNode{

//template <typename T>
friend class List<T>;	

private:
//data members
	T data;
	ListNode<T>* next;

//methods
//constructor destructor
	ListNode(T ddata);

//mutators
	void set_next(ListNode<T>* nnext);

public:

//accessors
	T get_data() const;
	T& get_data_handle();
	ListNode<T>* get_next();
	
//predicates
	bool has_next() const; 
};


template <typename T>
class List{
private:
	ListNode<T>* head;
	ListNode<T>* last;
	
	int size;

public:
//constructor destructor
	List();
	~List();
	List(const List& L);
	const List& operator=(const List& R);
	
	void clear_pointers();  //be sure T = S* for some type S!!
	
//debug
	void print(void (*print_elem) (T, std::ostream&), std::ostream& out = std::cout) const;

//accessors
	ListNode<T>* get_begin() const;
	ListNode<T>* get_last()	const;
	int get_size() const;
	List<T> get_tail() const;

//predicates
	bool empty() const;
	bool exists(T item) const;
	bool exists(T item,  bool (*equal) (T, T)) const;
	

//search
	ListNode<T>* get_first(T item) const;		//O(n)
	ListNode<T>* get_first(T item, bool (*equal) (T, T)) const;

//max/min
	/* let f:T -> Int maximum returns
	 * for a l in [T] the v in f(T intersection l)
	 * s.t. v >= f(i) for every i in l*/
	int maximum(int (*f)(const T&));

//expand
	void append(T item);
	void append(const List<T>& L);
	
	// appends item only if item doesn't exist in the list
	// this way we can encode sets, as lists
	bool insert(T item);						//if item exists in list returns false
	void insert(const List<T>& L);

//erase
	void erase(ListNode<T>* Node);				//O(n)
	void erase(List< ListNode<T>* >& Nodes);	//O(n*k), k = |Nodes|
	void erase_first(T elem);					//O(n)
	T pop_head();

//sublist
	List<T> filter(bool (*predicate) (const T&));

//conversions
	operator List<int>() const;
};


//Show
template <typename T>
std::ostream& operator<<(std::ostream& out, const List<T>& L);

//operations
template <typename T>
List<List<T>> tokenize_list(const List<T>& L, const T& delim);


#include "list.tpp"

#endif
