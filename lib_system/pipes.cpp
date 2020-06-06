#include "pipes.h"

#include "file_io.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <cassert>
#include <cstdio>
#include <cstdlib>

#include <iostream>
 


using namespace std;

/*
 **************
 * Pipe Write *
 **************
 */

//constructor destructor
pipe_w::pipe_w(const String& pipename){
	char* cpipename = pipename.cstring();
	//assert( (desc = open(cpipename, O_WRONLY)) != -1);
	/*if((desc = open(cpipename, O_WRONLY)) == -1){
		perror(cpipename);
		exit(1);
	}*/
	
	for(int i = 1; (desc = open(cpipename, O_WRONLY)) == -1 and i <= 30; ++i)  sleep(1);
	if(desc == -1)  exit(1);
	
	delete[] cpipename;
}

pipe_w::~pipe_w(){
	assert(close(desc) !=-1);
}

//operations
void pipe_w::putchar(char c){
	assert(::write(desc, &c, 1) != -1);
}

void pipe_w::write(const String& s){
	//DEBUG
	//cout << "write" << endl;
	
	int len = s.get_length();
	for(int i = 0; i <= len-1; ++i)  putchar(s[i]);
}

void pipe_w::write_short_int(short int i){
	assert(::write(desc, &i, 2) != -1);
}

void pipe_w::write_int(int i){
	assert(::write(desc, &i, 4) != -1);
}

/*
 *************
 * Pipe Read *
 *************
 */


//constructor destructor
pipe_r::pipe_r(const String& pipename){
	char* cpipename = pipename.cstring();
	assert( (desc = open(cpipename, O_RDONLY)) != -1);
	/*if((desc = open(cpipename, O_WRONLY)) == -1){
		perror(cpipename);
		exit(1);
	}*/
	
	delete[] cpipename;
}

pipe_r::~pipe_r(){
	assert(close(desc) !=-1);
}

//predicates
/*
bool pipe_r::is_eof(){
	char c;
	
	//DEBUG
	//cout << 
	
	int err = ::read(desc, &c, 1);
	assert(lseek(desc, -1, SEEK_CUR) != -1);
	
	return err == 0;
}
*/

//operations
char pipe_r::getchar(){
	char c;
	assert(::read(desc, &c, 1) != -1);
	
	return c;
}

String pipe_r::read(int n_bytes /*= INT_MAX*/){
	String s;
	
	//DEBUG
	//cout << "read" << endl;
	
	if(n_bytes >= 1){
		s += getchar();
		int i = 2;
		while(i <= n_bytes /*and not is_eof()*/){
			s += getchar();
			
			//DEBUG
			//cout << s << endl;
			
			++i;
		}
	}
	
	return s;
}

short int pipe_r::read_short_int(){
	short int i;
	assert(::read(desc, &i, 2) != -1);
	
	return i;
}

int pipe_r::read_int(){
	int i;
	assert(::read(desc, &i, 4) != -1);
	
	return i;
}

/*
 ***************
 * PipeCreator *
 ***************
 */


void mkfifo(const String& fifo_name){
	char* cstr = fifo_name.cstring();
		if(mkfifo(cstr, named_pipe_mode) == -1){
			perror(cstr);
			exit(1);
		}
	delete[] cstr;
}

void rmfifo(const String& fifo_name){
	char* cstr = fifo_name.cstring();
		assert(remove(cstr) != -1);
	delete[] cstr;
}

/*
//constructor destructor
PipeCreator::PipeCreator(const String& ppipe_name){
	if(file_exists(ppipe_name)){
		responsibility = false;
		//pipe_name <-- e
	}
	else{
		responsibility = true;
		char* cstr = ppipe_name.cstring();
			assert(mkfifo(cstr, named_pipe_mode) != -1);
		delete[] cstr;
	}
	
	pipe_name = ppipe_name;
}

PipeCreator::~PipeCreator(){
	if(responsibility){
		char* cstr = pipe_name.cstring();
			assert(remove(cstr) != -1);
		delete[] cstr;
	}
}

//accessors
String PipeCreator::get_pipe_name() const {
	return pipe_name;
}
*/
