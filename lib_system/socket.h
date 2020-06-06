#ifndef SOCKET_H

#define SOCKET_H

#include <climits>

#include <netinet/ip.h>

#include "file_io.h"

const String IpHome = "127.0.0.1";
const String IpAny = "0.0.0.0";

/*
 ******************
 * Socket Address *
 ******************
 */

const int foo_port = -1;
const String foo_ip = "-1.-1.-1.-1";

class SocketAdress{
	/*Host*/int port;
	String ip_address;

public:
//constructor destructor
	SocketAdress();			//Warning !! Creates an invalid SocketAddress !!
	SocketAdress(int port, const String& ip_address = "");
	SocketAdress(const sockaddr_in& c_sockaddr);
	SocketAdress(const SocketAdress& s);
	const SocketAdress& operator=(const SocketAdress& s);

//accessors
	int get_port() const;
	String get_ip_address() const;

//predicates
	bool is_any() const;
	bool is_empty() const;
	
//convertions
	//returns a sockaddr_in c struct for compatability
	sockaddr_in get_sockaddr() const;
};

//Show
std::ostream& operator<<(std::ostream& out, const SocketAdress& sock_addr);

//relations
bool operator==(const SocketAdress& A, const SocketAdress& B);
bool operator!=(const SocketAdress& A, const SocketAdress& B);

//convertions
String name2ip(const String& address_name);
unsigned int ip2binary(const String& ip_address);
String binary2ip(unsigned int binary_ip_address);

/*
 *******
 * TCP *
 *******
 */

class TCPServerSocket{
protected:
	int sock_desc;

public:
//constructor destructor
	TCPServerSocket();
	~TCPServerSocket();

//operations
	void bind(const SocketAdress& sock_addr);
	void listen(int backlog = INT_MAX);
	int accept();
	int accept(SocketAdress& client_info);
};



class TCPClientSocket : public low_file_io_methods{
protected:
	int sock_desc;

public:
//constructor destructor
	TCPClientSocket();
	~TCPClientSocket();

//operations
	void connect(const SocketAdress& sock_addr);
};



/*
 *******
 * UDP *
 *******
 */


class UDPSocket{
	int sock_desc;
	
public:
//constructor destructor
	UDPSocket();
	~UDPSocket();

//operations
	void bind(const SocketAdress& sock_addr);
	void bind(int port);
	
	void send_to(const SocketAdress& sock_addr, const String& messege);
	String receive_from(const SocketAdress* sock_addr);
};

#endif
