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

    ret = dtcurl_wrapper_init(&session->dtcurl_wrapper, uri);
    if (ret < 0) {
        return ret;
    }

    ret = dtcurl_wrapper_start(&session->dtcurl_wrapper);
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
    CURL_LOG("dtcurl session closed \n");
    return CURL_ERROR_NONE;
}
