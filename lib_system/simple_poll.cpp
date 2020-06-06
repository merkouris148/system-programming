#include "simple_poll.h"

#include <poll.h>
#include <cassert>
#include "../lib_safe_c/safe_c.h"

const int mili = 1000;

void init_pollfds(const List<int>& watch_list, pollfd* poll_fds, int poll_fds_size){
	assert(watch_list.get_size() == poll_fds_size);
	
	auto cursor = watch_list.get_begin();
	int i = 0;
	for(; cursor->has_next(); cursor = cursor->get_next()){
		poll_fds[i].fd = cursor->get_data();
		poll_fds[i].events = POLLIN;
		
		++i; 
	}
	//last it
	poll_fds[i].fd = cursor->get_data();
	poll_fds[i].events = POLLIN;
}


List<int> get_ready_from_pollfds(pollfd* poll_fds, int poll_fds_size, int poll_ret){
	if(poll_ret == 0) {List<int> e; return e;}
	
	List<int> l;
	for(int i = 0; i <= poll_fds_size-1; ++i) if(poll_fds[i].revents & POLLIN) l.append(poll_fds[i].fd);
	
	return l;
}


List<int> simple_poll(const List<int>& watch_fds, int waiting_time /*= sp_default_waiting*/){
	assert(not watch_fds.empty());
	
	//initialize
	int n_fds = watch_fds.get_size(); pollfd poll_fds[n_fds];
	init_pollfds(watch_fds, poll_fds, n_fds);
	
	//run poll()
	int poll_ret;
	sc_assert((poll_ret = poll(poll_fds, n_fds, waiting_time*mili)));
	
	// if sc_assert hasn't found an error, yet poll_ret == -1 then errno == EINTR
	// meaning the user have kill the proccess, so simple_poll() should return
	// in order for the proccess to terminate properly
	if(poll_ret == -1)  {List<int> e; return e;}
	

	//return a list with fds wich are ready to be read
	return get_ready_from_pollfds(poll_fds, n_fds, poll_ret);
}


bool simple_poll(int watch_fd, int waiting_time /*= sp_default_waiting*/){
	List<int> singleton; singleton.append(watch_fd);
	List<int> ready_singleton = simple_poll(singleton, waiting_time);
	return not ready_singleton.empty();
}
