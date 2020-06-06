#ifndef DROPBOX_PROTOCOL_H

#define DROPBOX_PROTOCOL_H

#include <sys/select.h>

#include "../lib_containers/list.h"
#include "../lib_utilities/pair.h"
#include "../lib_containers/string.h"
#include "../lib_system/socket.h"
#include "../lib_system/directory.h"

const int max_clients = 10;


/*
 *********************
 * Protocol Commands *
 *********************
 */

//protocol constants
const int action_prefix_size = 6;  // the size of an action string
const int pathname_size = 128;

//server requests
const String LOG_ON 		= "LOG_ON";
const String LOG_OFF 		= "LOG_OF";
const String GET_CLIENTS 	= "GET_CL";
const String CLIENT_LIST 	= "CLIE_L";

//clients request
const String GET_FILE_LIST 	= "GET_FL";
const String GET_FILE 		= "GET_FI";


/*
 *******************
 * DropBox Request *
 * *****************
 */

typedef Pair</*path*/String, SocketAdress> DBRequest;

inline bool is_none(const DBRequest& request){
	return request.fst.empty() and request.snd.is_empty();
}


/*
 ******************
 * DropBox Server *
 ******************
 */

class DropBoxServer : private TCPServerSocket{
private:
	//int n_clients;
	/*Set*/List<SocketAdress> ClientInfos;
	/*Set*/List<int> active_socks;			// parent socket fd + additional client sockets fds

//private operations
//server operations
	void log_on(int socket);
	void log_off(int socket);
	void get_clients(int socket);
	
//helper functions
	String get_action(int socket);
	void determine_action(const String& action, int socket);
	void broadcast_client_info(const String& action /* action = LOG_ON | LOG_OFF */, const SocketAdress& client);
	void close_clients_socket(int socket);
	void send_client_infos(int socket);
	
public:
//constructor destructor
	DropBoxServer(int port, int n_clients = max_clients);
	//~DropBoxServer();

//operations
	void operate();
};


/*
 ******************
 * DropBox Client *
 ******************
 */


class DropBoxClient : private TCPClientSocket{
	SocketAdress my_address;
	/*Set*/List<SocketAdress>& PeerInfos;
	
	List<DBRequest>& requestQ;

//private operations
//request from server
	void determine_action(const String&);
	void peer_on();
	void peer_off();
	void peer_list_received();

//helping operations
	String get_server_action();

public:
//constructor destructor
	DropBoxClient(
					const SocketAdress& server,
					const SocketAdress& mmy_address,
					List<SocketAdress>& PPeerInfos,
					List<DBRequest>& rrequestQ
				);
	//~DropBoxClient();

//operations
	void operate();
	
//requests to server
	void log_on();
	void log_off();
	void get_clients();
};



/*
 **********************
 * Dropbox PeerServer *
 **********************
 */

class DropBoxPeerServer : private TCPServerSocket{
private:	
	const /*Set*/List<SocketAdress>& PeerInfos;
	
	String my_dir;
	
//private operations
//peer server operations	
	void get_file(int socket);
	void get_file_list(int socket);

//helper operations
	String get_action(int socket);
	void determine_action(const String&, int socket);
	
public:
//constructor destructor
	DropBoxPeerServer(
						int port,
						const List<SocketAdress>& PPeerInfos,
						const String& path_to_my_dir,
						int n_clients = max_clients
					);
	//~DropBoxPeerServer();
	
//operations
	void operate();
};



/*
 ****************
 * Dropbox Peer *
 ****************
 */

class DropBoxPeer{
private:
	List<DBRequest>& requestQ;
	
	String path_to_mirror_dir; 
	
//private operations
	void request_file(const DBRequest& request);
	void request_file_list(const DBRequest& request);

//helper operations
	void determine_request(const DBRequest& request);

public:
//constructor destructor
	DropBoxPeer(List<DBRequest>& rrequestQ, const String& ppath_to_mirror_dir);
	//~DropBoxPeer();

//operations
	void operate();

//thread operate
static void* thread_operate(void* args);
};

typedef Pair<DropBoxPeer*, pthread_mutex_t*> DBP_thread_operate_arg;

#endif
