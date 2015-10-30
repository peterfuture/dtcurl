/*
 * =====================================================================================
 *
 *    Filename   :  dtcurl_priv.h
 *    Description:
 *    Version    :  1.0
 *    Created    :  2015年10月29日 15时09分28秒
 *    Revision   :  none
 *    Compiler   :  gcc
 *    Author     :  peter-s (), peter_future@outlook.com
 *    Company    :  dt
 *
 * =====================================================================================
 */

#include "dtcurl_macro.h"
#include "dtcurl_log.h"
#include "dtcurl_buffer.h"
#include "dtcurl_wrapper.h"

typedef enum dtcurl_proto {
    CURL_PROTO_HTTP  = 0x001,
    CURL_PROTO_HTTPS = 0x002,
    CURL_PROTO_FTP   = 0x003,
    CURL_PROTO_FILE  = 0x004
} dtcurl_proto_t;

typedef struct dtcurl_session {
    char *uri;
    dtcurl_proto_t proto;
    dtcurl_wrapper_t dtcurl_wrapper;
    dtcurl_buffer_t cache;
} dtcurl_session_t;
