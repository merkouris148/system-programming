#ifndef SAFE_C_H

#define SAFE_C_H

#include <cassert>
#include <cstdio>
#include <cerrno>
#include <cstdlib>

const int sc_ok = INT_MAX;


#define sc_assert(c_fun) 										\
		{														\
			int ret = (c_fun);									\
			if(ret == -1 and errno != EINTR){					\
				printf("errno: %d ", errno); fflush(stdout);	\
				perror(#c_fun);									\
				assert((ret) != -1);							\
			}													\
		}

#define sc_assert2(c_fun, c_fun_ok) 							\
		{														\
			int ret = (c_fun);									\
			if(ret != c_fun_ok and errno != EINTR){				\
				printf("errno: %d ", errno); fflush(stdout);	\
				perror(#c_fun);									\
				assert((ret) != c_fun_ok);						\
			}													\
		}

#define sc_assert_show_retval(c_fun, c_fun_ok)							\
		{																\
			int ret = (c_fun);											\
			if(ret != c_fun_ok){										\
				printf("Return Value: %d\n", ret); fflush(stdout);		\
				assert((c_fun) == c_fun_ok);							\
			}															\
		}
				

#define sc_on_error_exit(c_fun)										\
		{															\
			if((c_fun) == -1){										\
				printf("errno: %d ", errno); fflush(stdout);		\
				perror("");											\
				exit(errno);										\
			}														\
		}


#define sc_get_errno(err, c_fun)					\
		{											\
			if((c_fun) == -1)  	err = errno;		\
			else 				err = sc_ok;		\
		}


#endif
