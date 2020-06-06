#ifndef THREAD_H

#define THREAD_H

#include <pthread.h>

class Threads{
private:
	int n_threads;
	pthread_t* thread_ids;

public:
	Threads(int nn_threads, void* (*thread_func) (void*), void* tf_args);
	~Threads();
};



#endif
