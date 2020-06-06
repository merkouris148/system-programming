#include "../lib_system/socket.h"
#include "dropbox_protocol.h"
#include "../lib_system/thread.h"
#include "../lib_system/path.h"

#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <cstring>
#include <cstdlib>
#include <cassert>

using namespace std;

pthread_mutex_t mutex;

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
	if(argc < 9)			return false;	
	
	//skip first argument (name of the func)
	//check if there is "-d", "-m", "-p", "-sp" in args
	//check only odds args
	bool exist_dirName			= false;
	bool exist_mirrorDir		= false;
	bool exist_portNum			= false;
	bool exist_serverPort		= false;
	for(int i = 1; i <= (argc-1); i += 2){
		if(strcmp(argv[i], "-d") == 0)		  	exist_dirName		= true;
		else if(strcmp(argv[i], "-m") == 0)		exist_mirrorDir		= true;
		else if(strcmp(argv[i], "-p") == 0)		exist_portNum		= true;
		else if(strcmp(argv[i], "-sp") == 0)	exist_serverPort	= true;
	}
	
	return exist_dirName and exist_mirrorDir and exist_portNum /*and exist_workerThreads*/ and exist_serverPort /*and exist_serverIP*/;
}

void get_args(
				int argc,
				char** argv,
				String& dirName,
				String& mirrorDir,
				int& portNum,
				String& myIP,
				int& workerThreads,
				int& serverPort,
				String& serverIP
			){
	for(int i = 1; i <= (argc-1); i += 2){
		if(strcmp(argv[i], "-d") == 0)  			dirName			= argv[i+1];
		else if(strcmp(argv[i], "-m") == 0)			mirrorDir		= argv[i+1];
		else if(strcmp(argv[i], "-p") == 0)			portNum			= atoi(argv[i+1]);
		else if(strcmp(argv[i], "-ip") == 0)		myIP			= argv[i+1];
		else if(strcmp(argv[i], "-w") == 0)			workerThreads	= atoi(argv[i+1]);
		else if(strcmp(argv[i], "-sp") == 0)		serverPort		= atoi(argv[i+1]);
		else if(strcmp(argv[i], "-sip") == 0)		serverIP		= argv[i+1];
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
	String dirName;
	String mirrorDir;
	int portNum;
	String myIP = IpHome;
	int workerThreads = 4;
	int serverPort;
	String serverIP = IpHome;
	
	assert(check_arguments(argc, argv));
	get_args(argc, argv, dirName, mirrorDir, portNum, myIP, workerThreads, serverPort, serverIP);
	
	//make dir paths to be members of PATH
	make_dir_path(mirrorDir); make_dir_path(dirName);
	
	// create data structures
	// common DS for the 3 client classes to communicate
	List<SocketAdress> PeerInfos;  List<DBRequest> requestQ;
	
	
	
	//create server to receive peer request
	DropBoxPeerServer peer_server(portNum, PeerInfos, dirName);
	
	//create client to communicate with dropbox server
	DropBoxClient client(SocketAdress(serverPort, serverIP), SocketAdress(portNum, myIP), PeerInfos, requestQ);
	client.log_on();  client.get_clients();
	
	//create dropbox peer to serve request to peers
	DropBoxPeer peer(requestQ, mirrorDir);
	
	
	
	//operate drobox client
	while(on){
		DBP_thread_operate_arg arg(&peer, &mutex);
		Threads thrs(workerThreads, DropBoxPeer::thread_operate, (void*) &arg);
		
		client.operate();
		peer_server.operate();
		
		//sleep(1);
	}
	
	client.log_off();
	
	return 0;
}
