#ifndef PATH_H

#define PATH_H

#include "../lib_containers/string.h"
#include "../lib_containers/list.h"

/*
 * This library handles strings of the regular language
 * PATH = ./ [concat] filename | dirname/ [concat] (filename | dirname/)*
 * 
 * that is relative paths that either are a file or a directory ending
 * with a '/'.
 */
 
//construct
void make_dir_path(String& path);
void make_file_path(String& path);

//predicates
//bool is_path(const String& path);
bool path_is_dir(const String& path);
bool path_is_file(const String& path);

//operations
/*
 * This function takes as input a p in PATH and
 * returns a list in the form
 * 		["dirname1/", "dirname2/", ..., "dirnamen/", "filename"]
 * or
 * 		["dirname1/", "dirname2/", ..., "dirnamen/", "dirname/"]
 */
List<String> path_tokenize(const String& path);

/*
 * Given:
 * 	1. a root
 * 	2. a path in PATH
 * 	3. a file (maybe empty)
 * 
 * this fuction creates all the directories of the path <root><path> if
 * they not exist and if path is a file at the end creates the file and
 * writes the string file.
 */
void create_path(const String& root, const String& path, const String& file);

String path_pop_head(const String& path);

int path_get_size(const String& path);

#endif
