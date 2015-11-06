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

#ifndef DTCURL_WRAPPER_H
#define DTCURL_WRAPPER_H

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

typedef struct http_context {
    char *location;
    int http_code;
    int chunksize;
    int end_header;
    int seekable;
    int64_t filesize;
    int64_t read_off;
    int eof;
    int is_streamed;
} http_context_t;

typedef struct dtcurl_wrapper {
    char *uri;
    dtcurl_proto_t proto;
    char curl_error_buf[CURL_ERROR_SIZE];
    dtcurl_buffer_t cache;
    http_context_t http;
    CURL *curl_handle;
    CURLM *multi_handle;
    pthread_t download_pid;
    int request_quit;
} dtcurl_wrapper_t;


int dtcurl_wrapper_open(dtcurl_wrapper_t *wrapper, const char *uri);
int dtcurl_wrapper_read(dtcurl_wrapper_t *wrapper, char *buf, int size);
int dtcurl_wrapper_seek(dtcurl_wrapper_t *wrapper, int64_t off, int whence);
int dtcurl_wrapper_get_filesize(dtcurl_wrapper_t *wrapper);
const char *dtcurl_wrapper_get_location(dtcurl_wrapper_t *wrapper);
int dtcurl_wrapper_close(dtcurl_wrapper_t *wrapper);

#endif
