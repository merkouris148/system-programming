#include "file_io.h"

#include "../lib_safe_c/safe_c.h"

#include <iostream>

#include <unistd.h>
#include <cassert>
#include <cstdio>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

const bool file_io_debug = true;

/*
 ***********
 * file_in *
 ***********
 */
 
//constructor destructor
file_in::file_in(const String& filename){
	char* cstr = filename.cstring();
		F = fopen(cstr, "r");
		if(F == NULL) {cout << "Error: file does not exists!" << endl; exit(1);}
	delete[] cstr;
}

//operations
String file_in::getline(){
	char* cstr = NULL; 	size_t csize = 0;
	int err = ::getline(&cstr, &csize, F);
	//cstr[err-1] = 0; 							//discard last '\n' character
		
	String str;
	if(err > 0){
		cstr[err-1] = 0;
		str = cstr;		//if not eof
	}
	
	free(cstr);									//delete c-string
	
	return str;
}

void file_in::getlines(List<String>& lines){
	while(!is_eof()) lines.append(getline());
	lines.erase(lines.get_last());
}

String file_in::getfile(){
	String sfile;
	
	while(!is_eof()){
		String newline = getline();
		
		if(not newline.empty())  {sfile += newline;	sfile += '\n';}
	}
	
	return sfile;
}

//operations
void gettokens(const String& filen, List<List<String>>& tokenlines, const String& delim){
	//get c-string
	char* filename = filen.cstring();
	
	file_in f(filename);
	
	List<String> lines; f.getlines(lines);
	auto cur = lines.get_begin();
	while(cur->has_next()){
		tokenlines.append(tokenize_string(cur->get_data(), delim));
		
		cur = cur->get_next();
	}
	
	//last iteration
	tokenlines.append(tokenize_string(cur->get_data(), delim));

	//delete c-string
	delete[] filename;
}

String getfile(const String& path){
	file_in f(path);
	
	return f.getfile();
}


/*
 ************
 * file_out *
 ************
 */

//constructor destructor
file_out::file_out(const String& filename){
	char* cstr = filename.cstring();
		assert((F = fopen(cstr, "w")) != NULL);
	delete[] cstr;
}

//operations
void file_out::putline(const String& s){
	char* cstr = s.cstring();
		fprintf(F, "%s", cstr);
	delete[] cstr;
}


/*
 ***************
 * file_in/out *
 ***************
 */

//operations
bool file_exists(const String& filen){
	//get c-string
	char* filename = filen.cstring();
	
	bool r = access(filename, F_OK) != -1;
	
	//delete c-string
	delete[] filename;
	
	return r;
}


/*
 ***************
 * file_append *
 ***************
 */


//constructor destructor
file_app::file_app(const String& filename){
	char* cstr = filename.cstring();
		assert((F = fopen(cstr, "a")) != NULL);
	delete[] cstr;
}

//operations
void file_app::putline(const String& s){
	char* cstr = s.cstring();
		fprintf(F, "%s", cstr);
	delete[] cstr;
}


/*
 ***************
 * low file_in *
 ***************
 */

//operations
char low_file_in_methods::getchar(){
	char c;
	sc_assert(::read(desc, &c, 1) == 1);
	
	return c;
}

String low_file_in_methods::safe_read(){
	int N  = read_int();  													//if(file_io_debug) cout << "safe_read(): N = " << N << endl;
	return read(N);
}

String low_file_in_methods::read(int n_bytes /*= read_max_chars*/){
	assert(n_bytes >= 1);
	String s;  for(int i = 1; i <= n_bytes; i++)  s += getchar();
	
	//DEBUG
	//cout << "read(): s.get_length() = " << s.get_length() << endl;
	//cout << "read(): s.get_size() = " << s.get_size() << endl;
	
	return s;
}

short int low_file_in_methods::read_short_int(){
	short int i;
	sc_assert(::read(desc, &i, 2) != -1);
	
	return i;
}

int low_file_in_methods::read_int(){
	int i = -1;
	sc_assert(::read(desc, &i, 4) == 4);
	
	//DEBUG
	//cout << "read_int(): i = " << i << endl;
	
	return i;
}

unsigned int low_file_in_methods::read_uint(){
	unsigned int i;
	sc_assert(::read(desc, &i, 4) != -1);
	
	return i;
}
 
 
 /*
 ****************
 * low file_out *
 ****************
 */


//operations
void low_file_out_methods::putchar(char c){
	sc_assert(::write(desc, &c, 1) != -1);
}

void low_file_out_methods::write(const String& s){

	int len = s.get_length();
	for(int i = 0; i <= len-1; ++i)  putchar(s[i]);
}

void low_file_out_methods::safe_write(const String& s){
	write_int(s.get_length());								//if(file_io_debug) cout << "safe_write(): N = " << s.get_size() << endl;
	write(s);
}

void low_file_out_methods::write_short_int(short int i){
	sc_assert(::write(desc, &i, 2) != -1);
}

void low_file_out_methods::write_int(int i){
	sc_assert(::write(desc, &i, 4) != -1);
}

void low_file_out_methods::write_uint(unsigned int i){
	sc_assert(::write(desc, &i, 4) != -1);
}


/*
 ***************
 * low file_io *
 ***************
 */

/*low_file_io::low_file_io(const String& filename){
	char* cfilename = filename.cstring();
		int ddesc;  sc_assert(ddesc = open(cfilename, O_RDWR));
	delete[] cfilename;
	
	//initialize base class
	low_file_in_methods::desc = ddesc;
	low_file_out_methods::desc = ddesc;
	
	//initialize me
	desc = ddesc;
}



low_file_io::~low_file_io(){
	sc_assert(close(desc));
	
	//DEBUG
	cout << desc << endl;
}*/	

low_file_io_methods::low_file_io_methods(int ddesc){
	//initialize base class
	low_file_in_methods::desc = ddesc;
	low_file_out_methods::desc = ddesc;
	
	//initialize me
	desc = ddesc;
}

//copy
low_file_io_methods::low_file_io_methods(const low_file_io_methods& lfio){
	//initialize base class
	low_file_in_methods::desc = lfio.desc;
	low_file_out_methods::desc = lfio.desc;
	
	//initialize me
	desc = lfio.desc;
}

const low_file_io_methods& low_file_io_methods::operator=(const low_file_io_methods& lfio){
	if(&lfio != this){
		//initialize base class
		low_file_in_methods::desc = lfio.desc;
		low_file_out_methods::desc = lfio.desc;
		
		//initialize me
		desc = lfio.desc;
	}
	
	return *this;
}

//conversion, compatability with c functions
low_file_io_methods::operator int() const{
	return get_desc();
}

//show
std::ostream& operator<<(std::ostream& out, const low_file_io_methods& lfio){
	out << lfio.get_desc();
	return out;
}
