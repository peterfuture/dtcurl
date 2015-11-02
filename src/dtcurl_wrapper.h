/*
 * =====================================================================================
 *
 *    Filename   :  dtcurl_wrapper.h
 *    Description:
 *    Version    :  1.0
 *    Created    :  2015年10月30日 10时44分46秒
 *    Revision   :  none
 *    Compiler   :  gcc
 *    Author     :  peter-s (), peter_future@outlook.com
 *    Company    :  dt
 *
 * =====================================================================================
 */

#include "curl/curl.h"

#include "dtcurl_buffer.h"
#include "dtcurl_lock.h"

#define CURL_MAX_TIMEOUT 10000
#define CURL_MAX_CONNECTION_TIMEOUT 10000
#define CURL_MAX_REDIRECTS 10

typedef enum dtcurl_proto {
    CURL_PROTO_UNKOWN = 0x000,
    CURL_PROTO_HTTP  = 0x001,
    CURL_PROTO_HTTPS = 0x002,
    CURL_PROTO_FTP   = 0x003,
    CURL_PROTO_FILE  = 0x004
} dtcurl_proto_t;

typedef struct dtcurl_wrapper {
    char *uri;
    dtcurl_proto_t proto;
    char curl_error_buf[CURL_ERROR_SIZE];

    dtcurl_buffer_t cache;
    CURL *curl_handle;
    CURLM *multi_handle;
    pthread_t download_pid;
    int request_quit;
} dtcurl_wrapper_t;
