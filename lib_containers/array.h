#ifndef ARRAY_H

#define ARRAY_H

#include <iostream>

template <typename T>
class Array{
protected:
	T* array;
	int size;

public:
//constructor destructor
	Array(int ssize);
	~Array();
	Array(const Array& A);
	const Array<T>& operator=(const Array<T>& rstr);
;	void clear_pointers();

//accessors
	T operator[](int i) const;
	int get_size() const;

//search
	bool exists(T q) const;
	int get_first(T q) const;

//mutators
	T& operator[](int i);
};

//Show
template <typename T>
std::ostream& operator<<(std::ostream& out, const Array<T>& A);

//relations
template <typename T>
bool operator==(const Array<T>& A, const Array<T>& B);

#include "array.tpp"

#endif
