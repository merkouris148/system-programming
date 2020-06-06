#include "path.h"

#include "directory.h"
#include "file_io.h"

#include <iostream>

using namespace std;

const bool path_debug = true;

//construct
void make_dir_path(String& path){
	if(path.get_last() != '/')  path += "/";
}

void make_file_path(String& path){
	assert(path.get_last() != '/');
}

//predicates
bool path_is_dir(const String& path){
	return path[path.get_length()-1] == '/';
}

bool path_is_file(const String& path){
	return not path_is_dir(path);
}

//operations
/*
 * This function takes as input a p in PATH and
 * returns a list in the form
 * 		["dirname1/", "dirname2/", ..., "dirnamen/", "filename"]
 * or
 * 		["dirname1/", "dirname2/", ..., "dirnamen/", "dirname/"]
 */
List<String> path_tokenize(const String& path){
	List<String> tokens = tokenize_string(path, "/");
	
	auto cursor = tokens.get_begin();
	for(; cursor->has_next(); cursor = cursor->get_next()){
		String& token = cursor->get_data_handle();
		token += "/";
	}
	
	//last token may be a directory or a file
	if(path_is_dir(path)){
		String& token = cursor->get_data_handle();
		token += "/";
	}
	
	return tokens;
}


void create_path(const String& root, const String& path, const String& file){
	assert(not root.empty());
	
	auto tokens = path_tokenize(path);
	auto cursor = tokens.get_begin();
		
	String current_path = root;
	for(; cursor->has_next(); cursor = cursor->get_next()){
		current_path += cursor->get_data();
		
		if(dir_exist(current_path))  	continue;
		else 							mkdir(current_path);
	}
	//last iteration
	current_path += cursor->get_data();
	
	
	//create leaf dir of file
	if(path_is_dir(current_path)){
		if(not dir_exist(current_path))  mkdir(current_path);
	}
	else{
		file_out f(current_path);
		f.putline(file);
	}
}

String path_pop_head(const String& path){
	auto tokens = path_tokenize(path);
	tokens.pop_head();
	
	return append(tokens);
}

int path_get_size(const String& path){
	return path_tokenize(path).get_size();
}
