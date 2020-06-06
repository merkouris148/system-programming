#ifndef SIMPLE_POLL_H

#define SIMPLE_POLL_H

#include "../lib_containers/list.h"

const int sp_default_waiting = 1; //in seconds

List<int> simple_poll(const List<int>& watch_fds, int waiting_time = sp_default_waiting);
bool simple_poll(int watch_fd, int waiting_time = sp_default_waiting);

#endif
