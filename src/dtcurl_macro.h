#ifndef DTCURL_MACRO_H
#define DTCURL_MACRO_H

/*************************************
** Common
*************************************/
#define TRUE            1
#define FALSE           0
#define MIN(x,y)       ((x)<(y)?(x):(y))
#define MAX(x,y)       ((x)>(y)?(x):(y))
#define CURL_MIN(x,y)       ((x)<(y)?(x):(y))
#define CURL_MAX(x,y)       ((x)>(y)?(x):(y))

#define MAX_URI_SIZE 4096
#define CURL_MAX_CACHE_SIZE (10*1024*1024)

#define CURL_MAX_SELECT_RETRY_TIME 100

// Return Value
#define CURL_ERROR_SELECT_ERROR -2
#define CURL_ERROR_UNKOWN -1
#define CURL_ERROR_NONE 0

#endif
