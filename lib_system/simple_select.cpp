#include "simple_select.h"

#include "../lib_safe_c/safe_c.h"
#include <sys/select.h>
#include <cassert>


List<int> simple_select(List<int> watch_fds){
	assert(watch_fds.get_size() <= FD_SETSIZE);
	
	//initialize set
	fd_set set; FD_ZERO(set);
	for(auto cursor = watch_fds.get_begin(); cursor->has_next(); cursor = cursor->get_next()){
		FD_SET(cursor->get_data(), set);
	}
	//last it
	FD_SET(cursor->get_data(), set);
	
	sc_assert(select(watch_fds))
}
