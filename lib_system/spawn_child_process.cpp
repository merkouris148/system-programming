#include "spawn_child_process.h"

#include "../lib_utilities/utilities.h"
#include "../lib_containers/list.h"


//constructor destructor
SpawnChildProcess::SpawnChildProcess(const String& executable, const String& aargs){
	//tokenize args
	auto args = tokenize_string(aargs, " ");
	
	//alloc c_args
	argc = args.get_size() + 1;	//#args + 1 for the executable as 0th argument
	argv = new char*[argc+1];	//argc + 1 for the last NULL
	
	//initialize c_args
	argv[0] = executable.cstring();
	
	auto cursor = args.get_begin();
	int i = 1;
	for(; cursor->has_next(); cursor = cursor->get_next()){
		argv[i] = cursor->get_data().cstring();
		++i;
	}
	//last iteration
	argv[i] = cursor->get_data().cstring();
	
	argv[argc] = NULL;
}

SpawnChildProcess::~SpawnChildProcess(){
	for(int i = 0; i <= argc-1; ++i)
		delete[] argv[i];
	
	delete[] argv;
}


//operation
pid_t SpawnChildProcess::spawn(){
	pid_t pid;
	assert((pid = fork()) != -1);
	
	//child
	if(pid == 0)  		execv(argv[0], argv);
	else if(pid != 0)	return pid;
}


//operations
int waitpid(pid_t child){
	int status;
	while(waitpid(child, &status, WNOHANG) == 0)  sleep(1);
	
	return WEXITSTATUS(status);
}
