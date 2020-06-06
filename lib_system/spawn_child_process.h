#ifndef SPAWN_CHILD_PROCESS_H

#define SPAWN_CHILD_PROCESS_H

#include <unistd.h>
#include <cassert>
#include "../lib_containers/string.h"

#include <sys/wait.h>

class SpawnChildProcess{
private:
	int argc;
	char** argv;
public:
//constructor destructor
	SpawnChildProcess(const String& executable, const String& args);
	~SpawnChildProcess();

//operation
	pid_t spawn();
};


//operations
int waitpid(pid_t child);


#endif
