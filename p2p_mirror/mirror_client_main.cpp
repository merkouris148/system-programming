#include <iostream>

#include <cstring>
#include <cassert>

#include <unistd.h>
#include <signal.h>
#include <stdio.h>

#include "../lib_containers/string.h"
#include "mirror_client.h"
#include "../lib_system/directory.h"
#include "../lib_system/path.h"

using namespace std;

//constants
const char* senter_path = "./senter";
const char* receiver_path = "./receiver";

//global variables
bool on = true;

/*
 *********************
 * Helping Functions *
 *********************
 */


//arguments

bool check_arguments(int argc, char** argv){
	if(!(argc % 2 == 1))	return false;
	if(argc < 11)			return false;	
	
	//skip first argument (name of the func)
	//check if there is "-n", "-c", "-i", "-m", "-l" in args
	//check only odds args
	bool exist_id				= false;
	bool exist_common_dir		= false;
	bool exist_input_dir		= false;
	bool exist_mirror_dir		= false;
	bool exist_log_file			= false;
	for(int i = 1; i <= (argc-1); i += 2){
		if(strcmp(argv[i], "-n") == 0)		  	exist_id			= true;
		else if(strcmp(argv[i], "-c") == 0)		exist_common_dir	= true;
		else if(strcmp(argv[i], "-i") == 0)		exist_input_dir		= true;
		else if(strcmp(argv[i], "-m") == 0)		exist_mirror_dir	= true;
		else if(strcmp(argv[i], "-l") == 0)		exist_log_file		= true;
	}
	
	return exist_id and exist_common_dir and exist_input_dir and exist_mirror_dir and exist_log_file;
}

void get_args(
				int argc,
				char** argv,
				String& id,
				String& common_dir,
				String& input_dir,
				String& mirror_dir,
				String& log_file
			){
	for(int i = 1; i <= (argc-1); i += 2){
		if(strcmp(argv[i], "-n") == 0)  		id			= argv[i+1];
		else if(strcmp(argv[i], "-c") == 0)		common_dir	= argv[i+1];
		else if(strcmp(argv[i], "-i") == 0)		input_dir	= argv[i+1];
		else if(strcmp(argv[i], "-m") == 0)		mirror_dir	= argv[i+1];
		else if(strcmp(argv[i], "-l") == 0)		log_file	= argv[i+1];
	}
}

void signal_handler(int signo){
	on = false;
}


int main(int argc, char** argv){
	
	//get arguments{
		String id;
		String common_dir;
		String input_dir;
		String mirror_dir;
		String log_file;
	
		assert(check_arguments(argc, argv));
		get_args(
					argc,
					argv,
					
					id,
					common_dir,
					input_dir,
					mirror_dir,
					log_file
				);
	//}
	
	//pre-process arguments{
		//make path members of PATH (see path.h){
			make_dir_path(common_dir);
			make_dir_path(input_dir);
			make_dir_path(mirror_dir);
			
			make_file_path(log_file);			
		//}
		
		if(not dir_exist(input_dir))  	{ cout << ("Client " + id + ": Error: input directory doesn't exist") << endl; exit(1); }
		if(dir_exist(mirror_dir))		{ cout << ("Client " + id + ": Error: mirror directory already exists") << endl; exit(1); }
		if(not file_exists(log_file))	{ cout << ("Client " + id + ": Error: log file doesn't exist") << endl; exit(1); }
	//}
	
	//signals{
		struct sigaction act;
		act.sa_handler = &signal_handler;
		sigfillset(&(act.sa_mask));
		sigaction(SIGINT, &act, NULL); sigaction(SIGQUIT, &act, NULL);
	//}
	
	//create client
	MirrorClient client(id, senter_path, receiver_path, mirror_dir, input_dir, common_dir, log_file);
	//operate client
	while(on){ client.operate(); sleep(1); }
	
	//log out
	client.write_to_log("LOG OUT");
	
	return 0;
}

