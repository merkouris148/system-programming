#include "dropbox_protocol.h"

#include <iostream>
#include <unistd.h>

#include "../lib_safe_c/safe_c.h"
#include "../lib_system/simple_poll.h"
#include "../lib_system/path.h"


using namespace std;

/*
const LOG_ON_actual_size = 6;
const LOG_OFF_actual_size = 7;
const GET_CLIENTS_actual_size = 11;
*/

const bool dropbox_debug = true;

/*
 *******************
 * DropBox Request *
 * *****************
 */

/*
bool is_none(const DBRequest& request){
	return sequest.
}*/

/*
 ******************
 * DropBox Server *
 ******************
 */


//private operations
//helper operations
void DropBoxServer::broadcast_client_info(const String& action /* action = LOG_ON | LOG_OFF */, const SocketAdress& client){
	//last client left, there is no one to broadcast to! //only server's listening socket 
	if(active_socks.get_size() == 1)  return;
	
	auto cursor = active_socks.get_begin();
	cursor = cursor->get_next();				//first socket in the list is me!, so skip it!
	
	for(; cursor->has_next(); cursor = cursor->get_next()){
		
		low_file_io_methods f(cursor->get_data());
		f.write(action);
		f.write_uint(ip2binary(client.get_ip_address()));
		f.write_uint(client.get_port());
	}
	//last it
	low_file_io_methods f(cursor->get_data());
	f.write(action);
	f.write_uint(ip2binary(client.get_ip_address()));
	f.write_uint(client.get_port());
}

String DropBoxServer::get_action(int socket){
	low_file_io_methods f(socket);
	return f.read(action_prefix_size);
}

void DropBoxServer::determine_action(const String& action, int socket){
	if(action == LOG_ON)		return log_on(socket);
	if(action == LOG_OFF)		return log_off(socket);
	if(action == GET_CLIENTS)	return get_clients(socket);
	else  							assert(( "Dropbox Server: unkown action", false ));
}

void DropBoxServer::close_clients_socket(int socket){
	assert(active_socks.exists(socket));
	
	active_socks.erase_first(socket);
	close(socket);
}

void DropBoxServer::send_client_infos(int socket){
	if(ClientInfos.empty())  return;
	
	low_file_io_methods f(socket);
	auto cursor = ClientInfos.get_begin();
	for(; cursor->has_next(); cursor = cursor->get_next()){
		f.write_uint(ip2binary(cursor->get_data().get_ip_address()));
		f.write_uint(cursor->get_data().get_port());
	}
	//last it
	f.write_uint(ip2binary(cursor->get_data().get_ip_address()));
	f.write_uint(cursor->get_data().get_port());
	
	//DEBUG
	if(dropbox_debug){
		cout << "Dropbox Server: Senting clients' infos: " << endl;
		cout << ClientInfos << endl;
	}
}



//server operations
void DropBoxServer::log_on(int socket){
	low_file_io_methods f(socket);
	
	//skip command
	//f.read(action_prefix_size);
	
	//read client's infos
	unsigned int bin_ip	= f.read_uint();
	unsigned int port	= f.read_uint();
	
	
	SocketAdress client(port, binary2ip(bin_ip));
	if( ClientInfos.exists(client) )  cout << "Warning: Dropbox Server: client " << client << " already exist!" << endl;
	else{
		ClientInfos.append(client);
		broadcast_client_info(LOG_ON, client);
		cout << "Dropbox Server: client " << client << " just logged on!" << endl;
	}
}


void DropBoxServer::log_off(int socket){
	low_file_io_methods f(socket);
	
	//skip command
	//f.read(action_prefix_size);
	
	//read clieant's infos
	unsigned int bin_ip	= f.read_uint();
	unsigned int port	= f.read_uint();
	
	
	SocketAdress client(port, binary2ip(bin_ip));
	if( not ClientInfos.exists(client) )  cout << "Error: Dropbox Server: client " << client << " not found!" << endl;
	else{
		ClientInfos.erase_first(client);  close_clients_socket(socket);	//not completely symetrical with log_on()
		broadcast_client_info(LOG_OFF, client);
		cout << "Dropbox Server: client " << client << " leaving!" << endl;
	}
}

void DropBoxServer::get_clients(int socket){
	low_file_io_methods f(socket);
	
	f.write(CLIENT_LIST);
	f.write_int(ClientInfos.get_size());
	send_client_infos(socket);
}



//constructor destructor
DropBoxServer::DropBoxServer(int port, int n_clients /*= max_clients*/){
	bind(SocketAdress(port));
	
	//listen
	//n_clients = nn_clients;
	listen(n_clients);	
	active_socks.append(sock_desc);
}

/*DropBoxServer::~DropBoxServer(){
	sc_assert(close(sock_desc));
}*/



//operations
void DropBoxServer::operate(){
	List<int> ready_sockets = simple_poll(active_socks);
	if(ready_sockets.empty())  return;
	
	auto cursor = ready_sockets.get_begin();
	for(; cursor->has_next(); cursor = cursor->get_next()){
		if(cursor->get_data() == sock_desc) assert( active_socks.insert( accept() ) );
		else  								determine_action( get_action(cursor->get_data()),  cursor->get_data());
	}
	//last it
	if(cursor->get_data() == sock_desc) assert( active_socks.insert( accept() ) );
	else  								determine_action( get_action(cursor->get_data()), cursor->get_data() );
}



/*
 ******************
 * DropBox Client *
 ******************
 */



//constructor destructor
DropBoxClient::DropBoxClient(
								const SocketAdress& server,
								const SocketAdress& mmy_address,
								List<SocketAdress>& PPeerInfos,
								List<DBRequest>& rrequestQ
							)
								: 	PeerInfos(PPeerInfos),
									requestQ(rrequestQ)
							{
	my_address = mmy_address;
	connect(server);
}

/*DropBoxClient::~DropBoxClient(){
	sc_assert(close(sock_desc));
}*/


//private operations
//requests to server
void DropBoxClient::log_on(){
	write(LOG_ON);
	write_uint(ip2binary(my_address.get_ip_address()));
	write_uint(my_address.get_port());
}

void DropBoxClient::log_off(){
	write(LOG_OFF);
	write_uint(ip2binary(my_address.get_ip_address()));
	write_uint(my_address.get_port());
}

void DropBoxClient::get_clients(){
	write(GET_CLIENTS);
}



//request from server
void DropBoxClient::determine_action(const String& action){
	if(action == LOG_ON)		return peer_on();
	if(action == LOG_OFF)		return peer_off();
	if(action == CLIENT_LIST)	return peer_list_received();
	else  							assert(( "Dropbox Client: unkown action", false ));
}

void DropBoxClient::peer_on(){
	//read peer's infos
	unsigned int bin_ip	= read_uint();
	unsigned int port	= read_uint();
	
	
	SocketAdress peer(port, binary2ip(bin_ip));
	if( PeerInfos.exists(peer) )  cout << "Warning: Dropbox Client: peer " << peer << " already exist!" << endl;
	else if(peer == my_address){
		cout << "Dropbox Client: Just log on to dropbox server: " << my_address << endl;
	}
	else{
		PeerInfos.append(peer);
		cout << "Dropbox Client: peer " << peer << " just logged on!" << endl;
	
	
	
		//Request Queue
		//add proper request
		requestQ.append( DBRequest("", peer) );
	}
}


void DropBoxClient::peer_off(){
	//read clieant's infos
	unsigned int bin_ip	= read_uint();
	unsigned int port	= read_uint();
	
	
	SocketAdress peer(port, binary2ip(bin_ip));
	if( not PeerInfos.exists(peer) )  cout << "Error: Dropbox Client: peer " << peer << " not found!" << endl;
	else{
		PeerInfos.erase_first(peer);
		cout << "Dropbox Client: peer " << peer << " leaving!" << endl;
		
		//close_clients_socket(socket);	//not completely symetrical with log_on()
	}
}


void DropBoxClient::peer_list_received(){
	int N = read_int();
	if(N == 0)  return;
	
	for(int i = 1; i <= N; ++i){
		String ip = binary2ip(read_uint());
		int port = (int) read_uint();
		
		SocketAdress new_peer = SocketAdress(port, ip);
		if(my_address != new_peer)  PeerInfos.insert(new_peer);
		//PeerInfos.erase_first(my_address);
	}
	
	//DEBUG
	if(dropbox_debug){
		cout << "Dropbox Client: New peer info list is: " << endl;
		cout << PeerInfos << endl;
	}
	
	
	
	//Request Queue
	//add proper get_file_list request in the queue
	if(PeerInfos.empty())  return;
	
	auto cursor = PeerInfos.get_begin();
	for(; cursor->has_next(); cursor = cursor->get_next()){
		requestQ.append( DBRequest("", cursor->get_data()) );
	}
	//last it
	requestQ.append( DBRequest("", cursor->get_data()) );
}



//helper operations
String DropBoxClient::get_server_action(){
	return read(action_prefix_size);
}



//operations
void DropBoxClient::operate(){
	if(simple_poll(sock_desc))  determine_action(get_server_action());
}


/*
 **********************
 * Dropbox PeerServer *
 **********************
 */


//private operations
void DropBoxPeerServer::get_file(int socket){
	if(dropbox_debug)  cout << "Dropbox Peer Server: serving GET_FILE.." << endl;
	
	//read path
	low_file_io_methods f(socket);
	String pathname = f.safe_read();			if(dropbox_debug) cout << "Dropbox Peer Server: file requested: " << pathname  << endl;
	
	//-- isws prepei na balw enan elegxo edw --
	
	//read file to send
	String str_file = getfile(pathname);
	
	//send file
	f.safe_write(str_file);
	
	//close my end of the connection
	close(socket);
	
	if(dropbox_debug)  cout << "Dropbox Peer Server: GET_FILE served!" << endl;
}


void DropBoxPeerServer::get_file_list(int socket){
	if(dropbox_debug)  cout << "Dropbox Peer Server: serving GET_FILE_LIST.." << endl;
	
	
	List<String> file_list = get_leafs(my_dir);
	//assert(( check_valid_paths(file_list), "there is a file which's path is too long!" ));
	
	//send file list
	low_file_io_methods f(socket);
	f.write_int(file_list.get_size());								if(file_list.empty())  return;
	
	
	auto cursor = file_list.get_begin();
	for(; cursor->has_next(); cursor = cursor->get_next())  f.safe_write(cursor->get_data());
	//last it
	f.safe_write(cursor->get_data());
	
	
	//DEBUG
	if(dropbox_debug)  cout << "Dropbox Peer Server: sending file_list to peer:" << file_list << endl;
	
	
	
	//close my end of the connection
	close(socket);
	
	
	if(dropbox_debug)  cout << "Dropbox Peer Server: GET_FILE_LIST served!" << endl;
}



//helper operations
String DropBoxPeerServer::get_action(int socket){
	low_file_io_methods f(socket);
	return f.read(action_prefix_size);
}

void DropBoxPeerServer::determine_action(const String& action, int socket){
	if(action == GET_FILE)			return get_file(socket);
	if(action == GET_FILE_LIST)		return get_file_list(socket);
	else  							assert(( "Dropbox Peer Server: unkown action", false ));
}


//constructor destructor
DropBoxPeerServer::DropBoxPeerServer(
										int port,
										const List<SocketAdress>& PPeerInfos,
										const String& path_to_my_dir,
										int n_clients /*= max_clients*/
									)
										: 	PeerInfos(PPeerInfos),
											my_dir(path_to_my_dir)
									{
	bind(SocketAdress(port));
	
	//listen
	listen(n_clients);								
}

/*DropBoxPeerServer::~DropBoxPeerServer(){
	sc_assert(close(sock_desc));
}*/




//operations
void DropBoxPeerServer::operate(){
	if(not simple_poll(sock_desc))  return;
	
	int new_peer = accept();  determine_action(get_action(new_peer), new_peer);
}



/*
 ****************
 * Dropbox Peer *
 ****************
 */
 
//private operations
void DropBoxPeer::request_file(const DBRequest& request){
	//DEBUG
	if(dropbox_debug)  cout << "Dropbox Peer: serving request: GET_FILE" << request << endl;
	
	
	//connect to peer
	TCPClientSocket peer;
	peer.connect(request.snd);
	
	//send request
	peer.write(GET_FILE);
	peer.safe_write(request.fst);
	
	//wait for response
	while(not simple_poll(peer));
	
	//get file
	String str_file = peer.safe_read();
		
	
	//copy file
	create_path(
					path_to_mirror_dir				,
					
					"client_" 						+
					request.snd.get_ip_address() 	+ 
					"_" 							+
					itoa(request.snd.get_port())	+
					"/"								+
					request.fst						,
					
					str_file
				);
	
	//DEBUG
	if(dropbox_debug)  cout << "Dropbox Peer: GET_FILE " << request << " request served!" << endl;
}


void DropBoxPeer::request_file_list(const DBRequest& request){
	//DEBUG
	if(dropbox_debug){
		cout << "Dropbox Peer: serving request: GET_FILE_LIST " << request << endl;
	}
	
	
	//connect to peer
	TCPClientSocket peer;
	peer.connect(request.snd);
	
	//send request
	peer.write(GET_FILE_LIST);
	
	//wait for response
	while(not simple_poll(peer));

	
	//get file list
	int N = peer.read_int();  	if(N == 0) return;
	List<String> file_list;  	for(int i = 1; i <= N; ++i) file_list.append(peer.safe_read());
	
	
	//DEBUG
	if(dropbox_debug)	cout << "Dropbox Peer: serving request: " << request << " file list received:" << file_list << endl;
	
	//add requests to queue
	auto cursor = file_list.get_begin();
	for(; cursor->has_next(); cursor = cursor->get_next())  requestQ.append(DBRequest(cursor->get_data(), request.snd));
	//last it
	requestQ.append(DBRequest(cursor->get_data(), request.snd));
	
	//DEBUG
	if(dropbox_debug)	cout << "Dropbox Peer: GET_FILE " << request << " request served!" << endl;
}


//helper operations
void DropBoxPeer::determine_request(const DBRequest& request){
	if(request.fst.empty())  	request_file_list(request);
	else  						request_file(request);
}


//constructor destructor
DropBoxPeer::DropBoxPeer(
							List<DBRequest>& rrequestQ,
							const String& ppath_to_mirror_dir
						) 
							: 	requestQ(rrequestQ),
								path_to_mirror_dir(( assert(not ppath_to_mirror_dir.empty()), ppath_to_mirror_dir ))
						{}
 
//operations
void DropBoxPeer::operate(){
	if(requestQ.empty())  return;
	
	DBRequest request = requestQ.pop_head();
	determine_request(request);
}

void* DropBoxPeer::thread_operate(void* aarg){
	// resolve argument
	DBP_thread_operate_arg* arg = static_cast<DBP_thread_operate_arg*>(aarg);
	DropBoxPeer* DBPeer_ptr	= arg->fst;
	pthread_mutex_t* mutex 	= arg->snd;
	
	
	if(DBPeer_ptr->requestQ.empty())  return NULL;
	
	//other threads shall not pass!!
	assert(pthread_mutex_lock(mutex) == 0);
		DBRequest request("", SocketAdress());
		if(not DBPeer_ptr->requestQ.empty())  request = DBPeer_ptr->requestQ.pop_head();
	assert(pthread_mutex_unlock(mutex) == 0);
	
	if(not is_none(request))  DBPeer_ptr->determine_request(request);
	
	pthread_exit(NULL);
	return NULL;
}
