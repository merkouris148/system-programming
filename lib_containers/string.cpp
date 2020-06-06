#include "string.h"

#include <cstring>
#include <cassert>
#include <cctype>
#include <cmath>
#include <cstdio>

using namespace std;

//constructor destructor
String::String(int len /*= string_default_length*/) : Array<char>(2*len){
	length = 0;
}

String::String(const char* cstr) :
	Array<char>( (assert(cstr != NULL), 2*strlen(cstr)) ){
	
	length = strlen(cstr);
	
	//int size = get_size(); //cout << size << endl;
	for(int i = 0; i <= length-1; ++i) operator[](i) = cstr[i];
}


String::String(const String& str) : Array<char>(str){
	length = str.get_length();
}


const String& String::operator=(const String& rstr){
	if(&rstr != this){
		//new length
		length = rstr.get_length();
		
		if(get_size() != rstr.get_size()){
			delete[] array;
			
			//new length
			//length = rstr.get_length();
			//new array
			array = new char[2*length];  size = 2*length;
		}
		
		//copy string
		for(int i = 0; i <= length-1; ++i)  operator[](i) = rstr[i];
	}
	
	return *this;	
}

//predicates
bool String::empty() const{
	return get_length() == 0;
}

bool String::exist(char c) const{
	
	int len = get_length();
	for(int i = 0; i <= len-1; ++i)
		if((*this)[i] == c)  return true;
	
	return false;
}


//search
int String::get_first(char c) const{
	assert(exist(c));
	
	int len = get_length();
	for(int i = 0; i <= len-1; ++i)
		if((*this)[i] == c)  return i;
}


//accessors
int String::get_length() const{
	return length;
}

char String::get_last() const{
	return (*this)[get_length()-1];
}


//operations
char String::operator[](int i) const{
	assert(i <= length-1);
	
	return Array::operator[](i);
}

char& String::operator[](int i){
	//assert(i <= length-1);
	
	return Array::operator[](i);	
}

String String::operator()(int i, int j) const{
	assert(i <= j);
	assert(0 <= i && i <= length-1);  assert(0 <= j && j <= length-1);
	
	String substr(j-i+1);
	
	for(int k = i; k <= j; ++k) substr += operator[](k);
	
	return substr;
}

// DON'T TOUCH IT!! IT WORKS!!
String& String::operator+=(const char& c){
	int new_len = length + 1;
	
	if(new_len > get_size()){
		//allocate new array					//update new arrays size
		char* new_array = new char[2*new_len];  size = 2*new_len;
		
		//copy old array to new
		for(int i = 0; i <= length-1; ++i) new_array[i] = operator[](i);
		
		//replace old array
		char* old_array = array;
		array = new_array;
		delete[] old_array; 
	}
	
	length = new_len;
	operator[](length-1) = c;
	
	return *this;
}

String& String::operator+=(const String& str){
	int len = str.get_length();
	for(int i = 0; i <= len-1; ++i) operator+=(str[i]);

	return *this;
}

/* Given: a string in the form <s><delim><t>
 * Returns: <s>*/
String String::get_prefix(char delim) const{
	return (*this)(0, get_first(delim)-1);
}

//conversions
int char2int(char c){
	assert(isdigit(c));
	
	return c - '0';
}

int String::stoi(){
	//handle sign{
		int sign = 1;
		int begin = 0;
		if(operator[](0) == '-'){
			sign = -1;
			begin = 1;
		}
	//}
	
	int length = get_length();
	int number = 0;
	for(int i = begin; i <= length-1; ++i)  number += char2int(operator[](i)) * pow(10, length - i - 1);
	
	return sign*number;
}

char* String::cstring() const{
	int clen = get_length()+1;
	
	char* cstr = new char[clen];
	for(int i = 0; i <= clen-2; ++i)  cstr[i] = (*this)[i];
	cstr[clen-1] = 0;
	
	return cstr;
}

//Show
ostream& operator<<(ostream& out, const String& str){
	int length = str.get_length();
	for(int i = 0; i <= length-1; ++i) out << str[i];
	
	return out;
}

//methods
List<String> tokenize_string(const String& str, const String& delim){
	assert(not str.empty() and not delim.empty());
	
	char* 			cstr 	= str.cstring();
	char* 			cdelim	= delim.cstring();
	List<String> 	tokens;
	
	
	for(char* token = strtok(cstr, cdelim); token != NULL; token = strtok(NULL, cdelim))
		tokens.append(String(token));
	
	delete[] cstr;
	delete[] cdelim;
	
	return tokens;
}

//relations
bool operator==(const String& A, const String& B){
	//debug
	//cout << A << "==" << B << endl;
	//cout << A.get_length() << "==" << B.get_length() << endl;
	
	if(A.get_length() != B.get_length())  return false;
	
	int len = A.get_length();
	for(int i = 0; i <= len-1; ++i) if(A[i] != B[i]) return false;
	
	return true;
}

bool operator!=(const String& A, const String& B){
	return not (A == B);
}

bool is_suffix(const String& A, const String& B){
	if(A.get_length() < B.get_length())  return false;
	
	int A_len = A.get_length();
	int B_len = B.get_length();
	
	for(int i = 1; i <= B_len; ++i)
		if(A[A_len-i] != B[B_len-i])  return false;
	
	return true;
}

//operations
String operator+(const String& A, const String& B){
	String AB;
	(AB += A) += B;
	
	return AB;
}

int get_length(const String& s){
	return s.get_length();
}

String append(const List<String>& ss){
	String s;
	if(not ss.empty()){
		auto cursor = ss.get_begin();
		for(; cursor->has_next(); cursor = cursor->get_next()) s += cursor->get_data();
		//last it
		s += cursor->get_data();
	}
	
	return s;
}

//convertions
String itoa(int i){
	char s[10];
	
	sprintf(s, "%d", i);
	
	String ss(s);
	
	return ss;
}
