#include "thread.h"

#include <cassert>
#include <iostream>

using namespace std;


//constructor destructor
Threads::Threads(int nn_threads, void* (*thread_func) (void*), void* tf_args){
	assert(nn_threads >= 1);  n_threads = nn_threads;
	
	thread_ids = new pthread_t[n_threads];
	for(int i = 0; i <= n_threads-1; ++i)  assert(pthread_create(&thread_ids[i], NULL, thread_func, tf_args) == 0);
}

Threads::~Threads(){
	for(int i = 0; i <= n_threads-1; ++i) pthread_join(thread_ids[i], NULL);
	
	delete[] thread_ids;
}
