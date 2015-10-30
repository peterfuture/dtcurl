#ifndef DTCURL_LOCK_H
#define DTCURL_LOCK_H

#include "pthread.h"

#define dtcurl_lock_t         pthread_mutex_t
#define dtcurl_lock_init(x,v) pthread_mutex_init(x,v)
#define dtcurl_lock(x)        pthread_mutex_lock(x)
#define dtcurl_unlock(x)      pthread_mutex_unlock(x)

#endif
