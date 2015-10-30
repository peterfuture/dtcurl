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

typedef struct dtcurl_wrapper {
    char *uri;
    CURL *curl_handle;
} dtcurl_wrapper_t;
