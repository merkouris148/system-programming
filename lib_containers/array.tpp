#include <cassert>

template <typename T>
Array<T>::Array(int ssize){
	assert(ssize >= 0); size = ssize;	//<-- carefull!!
	
	array = new T[size];
}

template <typename T>
Array<T>::~Array(){
	delete[] array;
}

template <typename T>
Array<T>::Array(const Array& A){
	size = A.get_size();
	array = new T[size];
	for(int i = 0; i <= size-1; ++i){
		//DEBUG
		//std::cout << "array copy constructor i = " << i << std::endl;
		
		operator[](i) = A[i];
	}
}

template <typename T>
const Array<T>& Array<T>::operator=(const Array<T>& A){
	assert(get_size() == A.get_size());
	
	int size = get_size();
	for(int i = 0; i <= size-1; ++i) operator[](i) = A[i];
}

template <typename T>
void Array<T>::clear_pointers(){
	for(int i = 0; i <= size-1; ++i)  delete array[i];
}


//accessors
template <typename T>
T Array<T>::operator[](int i) const{
	assert( 0 <= i && i <= size-1);
	
	return array[i];
}

template <typename T>
int Array<T>::get_size() const{
	return size;
}


//search
template <typename T>
bool Array<T>::exists(T q) const{
	for(int i = 0; i <= size-1; ++i) if((*this)[i] == q) return true;
	
	return false; 
}

template <typename T>
int Array<T>::get_first(T q) const{
	for(int i = 0; i <= size-1; ++i) if((*this)[i] == q) return i;
	
	assert(("Array Error: item does not exist in array", false));
}

//mutators
template <typename T>
T& Array<T>::operator[](int i){
	assert( 0 <= i && i <= size-1);
	
	//std::cout << size << std::endl;
	
	return array[i];
}


//Show
template <typename T>
std::ostream& operator<<(std::ostream& out, const Array<T>& A){
	out << "[ ";
	
	for(int i = 0; i <= A.get_size()-1; ++i)  out << A[i] << ", ";
	
	out << " ]";
	
	return out;
}


//relations
template <typename T>
bool operator==(const Array<T>& A, const Array<T>& B){
	//std::cout << A << "==" << B << std::endl;
	
	
	if(A.get_size() != B.get_size())  return false;
	
	int size = A.get_size();
	for(int i = 0; i <= size-1; ++i) if(A[i] != B[i]) return false;
	
	return true;
}
