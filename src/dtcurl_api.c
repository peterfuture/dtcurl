/*
 * =====================================================================================
 *
 *    Filename   :  dtcurl_api.c
 *    Description:
 *    Version    :  1.0
 *    Created    :  2015年10月29日 15时33分50秒
 *    Revision   :  none
 *    Compiler   :  gcc
 *    Author     :  peter-s
 *    Email      :  peter_future@outlook.com
 *    Company    :  dt
 *
 * =====================================================================================
 */

#include "dtcurl_priv.h"

int dtcurl_init(void **priv, const char *uri)
{
    if (!uri || strlen(uri) < 1) {
        return -1;
    }
    dtcurl_session_t *session = (dtcurl_session_t *)malloc(sizeof(dtcurl_session_t));
    dtcurl_session_open(session, uri);
    *priv = session;
    return 0;
}

int dtcurl_read(void *priv, char *buf, int size)
{
    dtcurl_session_t *session = (dtcurl_session_t *)priv;
    return dtcurl_session_read(session, buf, size);
}

int dtcurl_seek(void *priv, int64_t off, int whence)
{
    dtcurl_session_t *session = (dtcurl_session_t *)priv;
    return dtcurl_session_seek(session, off, whence);
}

int dtcurl_close(void *priv)
{
    dtcurl_session_t *session = (dtcurl_session_t *)priv;
    dtcurl_session_close(session);
    free(session);
    return CURL_ERROR_NONE;
}
