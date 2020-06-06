#ifndef LIST_SET_H

#define LIST_SET_H

#include "list.h"

/*
 * This library suports set operations
 * with lists
 */

//relations
template <typename T>
bool operator<=(const List<T>& L, const List<T>& S);

template <typename T>
bool operator<(const List<T>& L, const List<T>& S);

template <typename T>
bool operator>=(const List<T>& L, const List<T>& S);

template <typename T>
bool operator>(const List<T>& L, const List<T>& S);



template <typename T>
bool operator==(const List<T>& L, const List<T>& S);

template <typename T>
bool operator!=(const List<T>& L, const List<T>& S);



//operations
template <typename T>
List<T> operator-(const List<T>& L, const List<T>& S);


#include "list_set.tpp"

#endif
