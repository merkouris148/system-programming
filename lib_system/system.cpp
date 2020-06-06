#include "system.h"

#include <cstdio>

void safe_system(const String& command){
	char* cstr = command.cstring();
		assert(system(cstr) != -1);
	delete[] cstr;
}
