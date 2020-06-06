#include "dropbox_protocol.h"
#include "string.h"

#include <signal.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <cassert>

using namespace std;

bool on = true;


void signal_handler(int signo){
	on = false;
}

/*
 *********************
 * Helping Functions *
 *********************
 */


//arguments

bool check_arguments(int argc, char** argv){
	if(!(argc % 2 == 1))	return false;
	if(argc < 2)			return false;	
	
	//skip first argument (name of the func)
	//check if there is "-n", "-c", "-i", "-m", "-l" in args
	//check only odds args
	bool exist_port				= false;
	for(int i = 1; i <= (argc-1); i += 2){
		if(strcmp(argv[i], "-p") == 0)  exist_port = true;
	}
	
	return exist_port;
}

void get_args(
				int argc,
				char** argv,
				int& port
			){
	for(int i = 1; i <= (argc-1); i += 2){
		if(strcmp(argv[i], "-p") == 0)  port = atoi(argv[i+1]);
	}
}


int main(int argc, char** argv){
	//signals{
		struct sigaction act;
		memset(&act, '\0', sizeof(act));
		act.sa_handler = &signal_handler;
		sigfillset(&(act.sa_mask));
		sigaction(SIGINT, &act, NULL); sigaction(SIGQUIT, &act, NULL);
	//}
	
	
	//resolve arguments
	int port;
	assert(check_arguments(argc, argv));
	get_args(argc, argv, port);
	
	
	DropBoxServer Server(port);
	while(on) {Server.operate(); /*sleep(1);*/}
	
	return 0;
}
