#include <cassert>

#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>

#include "../lib_containers/string.h"
#include "senter_receiver.h"
#include "../lib_system/path.h"
#include "../lib_system/pipes.h"
#include "../lib_system/file_io.h"

/*
 * Arguments: SenterId ReceiverId CommonDir MirrorDir LogFile
 */

using namespace std;

int main(int argc, char** argv){
	//Check Arguments
	assert(argc == 6);
	
	String SenterId 	= argv[1];
	String ReceiverId 	= argv[2];
	String CommonDir	= argv[3];		assert(path_is_dir(CommonDir));
	String MirrorDir	= argv[4];		assert(path_is_dir(MirrorDir));
	String LogFile		= argv[5];		assert(not path_is_dir(LogFile));
	
	//create fifo
	String fifo_name = CommonDir + SenterId + "_to_" + ReceiverId + ".fifo";
	mkfifo(fifo_name);
	
	//open log file
	file_app log(LogFile);
	

	//receive files
	Receiver receiv(MirrorDir, SenterId, ReceiverId, fifo_name, log);
	receiv.receive_files();
	
	//destroy
	rmfifo(fifo_name);
	
	return 0;
}
