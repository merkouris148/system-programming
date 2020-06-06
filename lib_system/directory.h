#ifndef DIRECTORY_H

#define DIRECTORY_H

#include <sys/types.h>
#include <dirent.h>

#include "../lib_containers/list.h"
#include "../lib_containers/string.h"


/*
 *******************
 * Class Directory *
 *******************
 */


class Directory{
private:
	DIR* dir;
	String root;
public:
//constructor destuctor
	Directory(const String& ppath);
	~Directory();

//operations
	List<String> get_entries() const;
	List<String> get_subdirs() const;
	List<String> get_files() const;
};


/*
 ******************************
 * Other Directory Operations *
 ******************************
 */

//operations
List<String> get_entries(const String& path);
List<String> get_entries_by_suffix(const String& path, const String& suffix);
List<String> get_subdirs(const String& path);
List<String> get_files(const String& path);
List<String> get_leafs(const String& root);

void mkdir(const String& path);
void rmdir_r(const String& path);

//predicates
bool is_dir(const String& path);
bool is_file(const String& path);
bool is_leaf(const String& path);

bool dir_exist(const String& path);

#endif
