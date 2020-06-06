#ifndef PIPES_H

#define PIPES_H

#include "../lib_containers/string.h"

#include <climits>

//constants
const mode_t named_pipe_mode = 0777;

/*
 **************
 * Pipe Write *
 **************
 */


class pipe_w{
private:
	int desc;
public:
//constructor destructor
	pipe_w(const String& pipename);
	~pipe_w();

//operations
	void putchar(char c);
	void write(const String& s);
	void write_short_int(short int i);
	void write_int(int i);
};


/*
 *************
 * Pipe Read *
 *************
 */


class pipe_r{
private:
	int desc;
public:
//constructor destructor
	pipe_r(const String& pipename);
	~pipe_r();

//predicates
	bool is_eof();

//operations
	char getchar();
	String read(int n_bytes = INT_MAX);
	short int read_short_int();
	int read_int();
};



/*
 ***************
 * PipeCreator *
 ***************
 */
 

void mkfifo(const String& fifo_name);
void rmfifo(const String& fifo_name);


/*
class PipeCreator{
private:
	String pipe_name;
	bool responsibility;
public:
//constructor destructor
	PipeCreator(const String& pipe_name);
	~PipeCreator();

//accessors
	String get_pipe_name() const;
};
*/


#endif
