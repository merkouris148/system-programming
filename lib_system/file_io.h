#ifndef FILE_IO_H

#define FILE_IO_H

#include <cstdio>
#include <cassert>
#include <cstdlib>

#include "../lib_containers/string.h"
#include "../lib_containers/list.h"

/*
 ***********
 * file_in *
 ***********
 */

class file_in{
private:	
	FILE* F;
public:
//constructor destructor
	file_in(const char* filename)  	{F = fopen(filename, "r"); assert(("Error: file does not exist!", F != NULL));};
	file_in(const String& filename);
	file_in()						{F = stdin;};
	~file_in()						{int err = fclose(F); assert(err == 0);};
	
//operations
	String getline();
	void getlines(List<String>& lines);		//get all lines of the file
	String getfile();
	
//predicates
	bool is_eof() {return feof(F);};
};


//operations
void gettokens(const String& filename, List<List<String>>& tokenlines, const String& delim);
String getfile(const String& path);

/*
 ************
 * file_out *
 ************
 */

class file_out{
private:
	FILE* F;
public:
//constructor destructor
	file_out(const char* filename) 	{F = fopen(filename, "w"); assert(F != NULL);};
	file_out(const String& filename);
	file_out()						{F = stdout;};
	~file_out()						{int err = fclose(F); assert(err == 0);};
	
//operations
	void putline(const String& s);
};

/*
 ***************
 * file_in/out *
 ***************
 */

//operations
bool file_exists(const String& filename);


/*
 ***************
 * file_append *
 ***************
 */

class file_app{
private:
	FILE* F;
public:
//constructor destructor
	file_app(const char* filename) 	{F = fopen(filename, "a"); assert(F != NULL);};
	file_app(const String& filename);
	file_app()						{F = stdout;};
	~file_app()						{int err = fclose(F); assert(err == 0);};
	
//operations
	void putline(const String& s);
};


/*
 ***************
 * low file_in *
 ***************
 */

//const int read_max_chars = 1024;

class low_file_in_methods{
protected:
	int desc;

//protected constructor
	low_file_in_methods() {};

public:
//constructor destructor
	low_file_in_methods(int ddesc) : desc(( assert(ddesc >= 0), ddesc )) {};

//operations
	char getchar();
	String read(int n_bytes);
	// reads something that's writen with safe_write() (see below)
	// reads an integer N and then reads N bytes from the low_file
	String safe_read();			
	short int read_short_int();
	int read_int();
	unsigned int read_uint();
};


/*
 ****************
 * low file_out *
 ****************
 */


class low_file_out_methods{
protected:
	int desc;

//protected constructor
	low_file_out_methods() {};

public:
//constructor destructor
	low_file_out_methods(int ddesc) : desc(( assert(ddesc >= 0), ddesc )) {};

//operations
	void putchar(char c);
	void write(const String& s);
	// writes something to be read by safe_read() (see above)
	// writes first the size of s (using s.get_size()) then the string s
	void safe_write(const String& s);
	void write_short_int(short int i);
	void write_int(int i);
	void write_uint(unsigned int i);
};


/*
 ***************
 * low file_io *
 ***************
 */

class low_file_io_methods : public low_file_in_methods, public low_file_out_methods{
private:
	int desc;
	
public:
//constructor destructor
//	low_file_io(const String& filename);  	//open file
//	~low_file_io();							//close file

	low_file_io_methods(int ddesc);					//file already opened 

//copy
	low_file_io_methods(const low_file_io_methods& lfio);
	const low_file_io_methods& operator=(const low_file_io_methods& lfio);

//conversion, compatability with c functions
	operator int() const;

//accessors
	int get_desc() const {return desc;};
};

//show
std::ostream& operator<<(std::ostream& out, const low_file_io_methods& lfio);

#endif
