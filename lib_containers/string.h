#ifndef STRING_H

#define STRING_H

#include <iostream>

#include "array.h"
#include "list.h"


const int string_default_length = 16;

class String : public Array<char>{
private:
	int length;

public:
//constructor destructor
	String(int len = string_default_length);
	String(const char* cstr);							//with cautious
	String(const String& str);
	const String& operator=(const String& rstr);

//predicates
	bool empty() const;
	bool exist(char c) const;

//search
	int get_first(char c) const;

//accessors
	int get_length() const;	//string's length != string's size !!! 
	char get_last() const;
	
//operations
	char operator[](int i) const;			//O(1)
	char& operator[](int i);				//O(1)
	String operator()(int i, int j) const;	//O(j-i+1)
	String& operator+=(const String& str);	//amortized O(|str|)
	String& operator+=(const char& c);		//amortized O(1)

	/* Given: a string in the form <s><delim><t>
	 * Returns: <s>*/
	String get_prefix(char delim) const;

//conversions
	int stoi();
	char* cstring() const;	//not recommented!!
};							//the returned c-string should be deleted afterwards with "delete[] cstr;"

//Show
std::ostream& operator<<(std::ostream& out, const String& str);

//methods
List<String> tokenize_string(const String& str, const String& delim);

//relations
bool operator==(const String& A, const String& B);
bool operator!=(const String& A, const String& B);

bool is_suffix(const String& A, const String& B);

//operations
String operator+(const String& A, const String& B);
int get_length(const String& s);
String append(const List<String>& ss);

//convertions
String itoa(int i);

#endif
