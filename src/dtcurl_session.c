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
#include "dtcurl_wrapper.h"

int dtcurl_session_open(dtcurl_session_t *session, const char *uri)
{
    int ret = CURL_ERROR_NONE;
    memset(session, 0, sizeof(*session));
    session->uri = strdup(uri);
    CURL_LOG("url:%s \n", session->uri);

    ret = dtcurl_wrapper_open(&session->dtcurl_wrapper, uri);
    if (ret < 0) {
        return ret;
    }

    CURL_LOG("dtcurl session open success \n");
    return CURL_ERROR_NONE;
}

int dtcurl_session_read(dtcurl_session_t *session, char *buf, int size)
{
    return dtcurl_wrapper_read(&session->dtcurl_wrapper, buf, size);
}


int64_t dtcurl_session_seek(dtcurl_session_t *session, int64_t pos, int whence)
{
    return dtcurl_wrapper_seek(&session->dtcurl_wrapper, pos, whence);
}

int64_t dtcurl_session_get_filesize(dtcurl_session_t *session)
{
    return dtcurl_wrapper_get_filesize(&session->dtcurl_wrapper);
}

const char *dtcurl_session_get_location(dtcurl_session_t *session)
{
    return dtcurl_wrapper_get_location(&session->dtcurl_wrapper);
}

int dtcurl_session_close(dtcurl_session_t *session)
{
    CURL_LOG("dtcurl session close\n");
    if (!session) {
        return CURL_ERROR_UNKOWN;
    }
    if (session->uri) {
        free(session->uri);
    }
    return dtcurl_wrapper_close(&session->dtcurl_wrapper);
}
