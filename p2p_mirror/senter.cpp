#include <cassert>

#include <sys/types.h>
#include <sys/stat.h>

#include "../lib_containers/string.h"
#include "senter_receiver.h"
#include "../lib_system/path.h"
#include "../lib_system/pipes.h"

/*
 * Arguments: SenterId ReceiverId CommonDir InputDir LogFile
 */

int main(int argc, char** argv){
	//Check Arguments
	assert(argc == 6);
	
	//get arguments
	String SenterId 	= argv[1];
	String ReceiverId 	= argv[2];
	String CommonDir	= argv[3];		assert(path_is_dir(CommonDir));
	String InputDir		= argv[4];		assert(path_is_dir(InputDir));
	String LogFile		= argv[5];		assert(not path_is_dir(LogFile));
	
	
	//create fifo
	String fifo_name = CommonDir + SenterId + "_to_" + ReceiverId + ".fifo";
	//mkfifo(fifo_name);
	
	//open log file
	file_app log(LogFile);
	
	
	//sent files
	Senter sent(InputDir, SenterId, ReceiverId, fifo_name, log);
	sent.sent_files();
	
	
	return 0;
}
