#include <cassert>


//relations
template <typename T>
bool operator<=(const List<T>& L, const List<T>& S){
	//trivial cases
	if(L.empty())						return true;
	if(L.get_size() > S.get_size())  	return false;
	
	auto cursor = L.get_begin();
	for(; cursor->has_next(); cursor = cursor->get_next()){
		if(not S.exists(cursor->get_data()))  return false;
	}
	
	//last iteration
	if(not S.exists(cursor->get_data()))  return false;
	
	return true;
}

template <typename T>
bool operator==(const List<T>& L, const List<T>& S){
	if(L.get_size() < S.get_size())  	return false;
	else  								return L <= S;
}

template <typename T>
bool operator!=(const List<T>& L, const List<T>& S){
	return not (L == S);
}

template <typename T>
bool operator<(const List<T>& L, const List<T>& S){
	return L <= S and L != S;
}

template <typename T>
bool operator>=(const List<T>& L, const List<T>& S){
	return S <= L;
}


template <typename T>
bool operator>(const List<T>& L, const List<T>& S){
	return S < L;
}


//operations
template <typename T>
List<T> operator-(const List<T>& L, const List<T>& S){
	assert(L > S);
	//trivial
	if(S.empty())  return L;
	
	List<T> R = L;
	auto cursor = S.get_begin();
	for(;cursor->has_next(); cursor = cursor->get_next())  R.erase_first(cursor->get_data());
	
	//last iteration
	R.erase_first(cursor->get_data());
	
	
	return R;
}
