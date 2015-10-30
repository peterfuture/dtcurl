/*
 * =====================================================================================
 *
 *    Filename   :  dtcurl_session.c
 *    Description:
 *    Version    :  1.0
 *    Created    :  2015年10月29日 15时01分59秒
 *    Revision   :  none
 *    Compiler   :  gcc
 *    Author     :  peter-s
 *    Email      :  peter_future@outlook.com
 *    Company    :  dt
 *
 * =====================================================================================
 */

#include <stdio.h>
#include "dtcurl_priv.h"

int dtcurl_session_open(dtcurl_session_t *session, const char *uri)
{
    int ret = CURL_ERROR_NONE;
    memset(session, 0, sizeof(*session));
    session->uri = strdup(uri);
    CURL_LOG("hls url:%s \n", session->uri);
    ret = dtcurl_buf_init(&session->cache, CURL_MAX_CACHE_SIZE);
    if (ret < 0) {
        CURL_LOG("dtcurl cache init failed \n");
        return ret;
    }

    // protocal check
    if (dtcurl_stristart(uri, "http://", NULL)) {
        session->proto = CURL_PROTO_HTTP;
    } else if (dtcurl_stristart(uri, "httpis://", NULL)) {
        session->proto = CURL_PROTO_HTTPS;
    }

    ret = dtcurl_wrapper_init(&session->dtcurl_wrapper, uri);
    if (ret < 0) {
        return ret;
    }
    CURL_LOG("dtcurl session open success \n");
    return CURL_ERROR_NONE;
}

int dtcurl_session_close(dtcurl_session_t *session)
{
    if (!session) {
        return CURL_ERROR_UNKOWN;
    }
    if (session->uri) {
        free(session->uri);
    }
    dtcurl_buf_release(&session->cache);
    CURL_LOG("dtcurl session closed \n");
    return CURL_ERROR_NONE;
}
