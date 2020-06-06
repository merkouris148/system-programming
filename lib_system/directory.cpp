#include "directory.h"

#include <sys/stat.h>

#include <cassert>
#include "../lib_safe_c/safe_c.h"

#include <iostream>

#include "system.h"

using namespace std;

const bool directory_debug = false;

//constants
//const mode_t dir_mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
const mode_t dir_mode = 0777;

/*
 *******************
 * Class Directory *
 *******************
 */


//constructor destructor
Directory::Directory(const String& ppath){
	char* cstr = ppath.cstring();
	
	assert((dir = opendir(cstr)) != NULL);
	
	delete[] cstr;
	root = ppath;
}

Directory::~Directory(){
	//DEBUG
	if(directory_debug)  cout << "Directory: destructor" << endl;
	
	sc_assert(closedir(dir));
}

//operations
List<String> Directory::get_entries() const{
	
	List<String> entries;
	
	for(struct dirent* cur = readdir(dir); cur != NULL; cur = readdir(dir)){
		String newpath = root;
		
		//append '/' if missing
		if(newpath[newpath.get_length()-1] != '/')  newpath += "/";  newpath += cur->d_name;
		
		//append '/' at the end of the path if the entry is a directory
		if(is_dir(newpath))  newpath += "/";
		
		//discard the directories . , ..																	//should I check if it is a deleted entry ???
		if(newpath[newpath.get_length()-1] == '/' and newpath[newpath.get_length()- 2] == '.')  continue;
		
		//append entry to entries list
		entries.append(newpath);
	}
	
	//rewind DIR pointer
	rewinddir(dir);
	
	//return entries
	return entries;
}

List<String> Directory::get_subdirs() const{
	auto entries = get_entries();
	if(entries.empty())  	return entries;
	else 					return entries.filter(is_dir);
}

List<String> Directory::get_files() const{
	auto entries = get_entries();
	if(entries.empty())  	return entries;
	else 					return entries.filter(is_file);
}


/*
 ******************************
 * Other Directory Operations *
 ******************************
 */

List<String> get_entries(const String& path){
	Directory dir(path);
	return dir.get_entries();
}

/*
List<String> get_entries_by_suffix(const String& path, const String& suffix){
	
}
*/

List<String> get_subdirs(const String& path){
	Directory dir(path);
	return dir.get_subdirs();
}

List<String> get_files(const String& path){
	Directory dir(path);
	return dir.get_files();
}

/*
 * As "leafs" are regurded either the files, or the empty directories.
 */ 
List<String> get_leafs(const String& root){
	List<String> queue; queue.append(root);		//BFS_queue
	List<String> leafs;
	
	//BFS
	while(not queue.empty()){
		String newpath = queue.pop_head();
		
		if(is_leaf(newpath)) 	leafs.append(newpath);
		else 					queue.append(get_entries(newpath));
	}
	
	return leafs;
}

void mkdir(const String& path){
	char* cstr = path.cstring();
		assert(::mkdir(cstr, dir_mode) != -1);
	delete[] cstr;
}

void rmdir_r(const String& path){
	safe_system("rm -r " + path);
}

//predicates
bool is_dir(const String& path){
	struct stat s;
	char* cstr = path.cstring();
	
	assert(stat(cstr, &s) != -1);
	
	delete[] cstr;
	return (s.st_mode & S_IFMT) == S_IFDIR;
}

bool is_file(const String& path){
	return not is_dir(path);
}

/*
 * As "leafs" are regurded either the files, or the empty directories (with no entries).
 */ 
bool is_leaf(const String& path){
	if(is_file(path))  return true;
	
	return get_entries(path).empty();
}


bool dir_exist(const String& path){
	char* cstr = path.cstring();
		DIR* dir = opendir(cstr);
	delete[] cstr;
	
	bool r = dir != NULL;
	
	closedir(dir);
	
	return r;
}
