#include "socket.h"

#include "../lib_safe_c/safe_c.h"

#include <cstring>
#include <cassert>
#include <cstdlib>

#include <iostream>

#include <netdb.h>
#include <netinet/in.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

/*
 ********************
 * Helper Functions *
 ********************
 */

unsigned int in_addr2n_uint(in_addr ina);
in_addr n_uint2in_addr(unsigned int n_binary);



/*
 *****************
 * Socket Adress *
 *****************
 */

SocketAdress::SocketAdress(){
	port = foo_port;
	ip_address = foo_ip;
}

SocketAdress::SocketAdress(int pport, const String& iip_address /*= ""*/){
	port = pport;
	if(iip_address.empty())  	ip_address = IpAny;
	else 						ip_address = iip_address;
}

SocketAdress::SocketAdress(const sockaddr_in& c_sockaddr){
	port = ntohs(c_sockaddr.sin_port);
	ip_address = binary2ip(c_sockaddr.sin_addr.s_addr);
}

SocketAdress::SocketAdress(const SocketAdress& s){
	port = s.get_port();
	ip_address = s.get_ip_address();
}

const SocketAdress& SocketAdress::operator=(const SocketAdress& s){
	if(&s != this){
		port = s.get_port();
		ip_address = s.get_ip_address();
	}
	
	return *this;
}

//accessors
int SocketAdress::get_port() const{
	return port;
}

String SocketAdress::get_ip_address() const{
	return ip_address;
}

//predicates
bool SocketAdress::is_empty() const{
	return port == foo_port or ip_address == foo_ip;
}

bool SocketAdress::is_any() const{
	return ip_address == IpAny;
}

//convertions
sockaddr_in SocketAdress::get_sockaddr() const{
	sockaddr_in sock_addr;
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(port);
	
	//write ip_address to binary
	char* cip_address = ip_address.cstring();
		sc_assert_show_retval(inet_pton(AF_INET, cip_address, &sock_addr.sin_addr), 1);
	delete[] cip_address;
	
	
	
	return sock_addr;
}


//Show
ostream& operator<<(ostream& out, const SocketAdress& sock_addr){
	out << sock_addr.get_port() << " " << sock_addr.get_ip_address();
	
	return out;
}

//relations
bool operator==(const SocketAdress& A, const SocketAdress& B){
	return A.get_port() == B.get_port() and A.get_ip_address() == B.get_ip_address();
}

bool operator!=(const SocketAdress& A, const SocketAdress& B){
	return not (A == B);
}

//other operations
//convertions
String name2ip(const String& address_name){
	char* caddress_name = address_name.cstring();
		hostent* h = gethostbyname(caddress_name);
		in_addr inadd; memcpy((void*) &inadd, h->h_addr, h->h_length);
	delete[] caddress_name;
	
	
	return inet_ntoa(inadd);
}

//helper function definition
unsigned int in_addr2n_uint(in_addr ina){
	return ina.s_addr;
}

in_addr n_uint2in_addr(unsigned int n_binary){
	in_addr ina;
	ina.s_addr = n_binary;
	return ina; 
}
//end helper function definition

unsigned int ip2binary(const String& ip_address){
	in_addr ina;
	char* cip_address = ip_address.cstring();
		inet_pton(AF_INET, cip_address, &ina);
	delete[] cip_address;
	
	return in_addr2n_uint(ina);
}

String binary2ip(unsigned int binary_ip_address){
	in_addr ina = n_uint2in_addr(binary_ip_address);
	return inet_ntoa(ina);
}

/*
 *******
 * TCP *
 *******
 */
 
 
// TCP Server //

//constructor destructor
TCPServerSocket::TCPServerSocket(){
	sc_assert( (sock_desc = socket(AF_INET, SOCK_STREAM, 0)) );
}

TCPServerSocket::~TCPServerSocket(){
	sc_assert(close(sock_desc));
}

//operations
void TCPServerSocket::bind(const SocketAdress& sock_addr){
	sockaddr_in sai = sock_addr.get_sockaddr();
	sc_assert(::bind(sock_desc, (sockaddr*) &sai, sizeof(sock_addr)));
}

/*
void TCPServerSocket::bind(int port, const String& adress_name ){
	bind(SocketAdress( port, name2ip(adress_name) ));
}
*/

void TCPServerSocket::listen(int backlog /*INT_MAX*/){
	sc_assert(::listen(sock_desc, backlog));
}

int TCPServerSocket::accept(){
	int new_sock_desc; sc_assert(new_sock_desc = ::accept(sock_desc, NULL, NULL));
	
	return new_sock_desc;
}

int TCPServerSocket::accept(SocketAdress& client_info){
	sockaddr_in cinfo;
	unsigned int cinfo_size = sizeof(cinfo);
	
	int new_sock_desc; sc_assert(new_sock_desc = ::accept(sock_desc, (sockaddr*) &cinfo, &cinfo_size));
	client_info = cinfo;
	
	return new_sock_desc;
}

// TCP Client //

//constructor destructor
TCPClientSocket::TCPClientSocket() : low_file_io_methods( (sock_desc = socket(AF_INET, SOCK_STREAM, 0)) ){
	sc_assert(sock_desc);
}

TCPClientSocket::~TCPClientSocket(){
	sc_assert(close(sock_desc));
}

//operations
void TCPClientSocket::connect(const SocketAdress& sock_addr){
	assert(not sock_addr.is_any());
	
	sockaddr_in sai = sock_addr.get_sockaddr();
	sc_assert( ::connect(sock_desc, (sockaddr*) &sai, sizeof(sock_addr)) );
}

/*
void TCPClientSocket::connect(int port, const String& adress){
	assert(not adress.empty());
	
	connect(SocketAdress(port, adress));
}
*/
