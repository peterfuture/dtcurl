/*
 * =====================================================================================
 *
 *    Filename   :  dtcurl_session.h
 *    Description:
 *    Version    :  1.0
 *    Created    :  2015年11月06日 15时24分24秒
 *    Revision   :  none
 *    Compiler   :  gcc
 *    Author     :  peter-s (), peter_future@outlook.com
 *    Company    :  dt
 *
 * =====================================================================================
 */

#ifndef DTCURL_SESSION_H
#define DTCURL_SESSION_H

int dtcurl_session_open(dtcurl_session_t *session, const char *uri);
int dtcurl_session_read(dtcurl_session_t *session, char *buf, int size);
int dtcurl_session_seek(dtcurl_session_t *session, int64_t pos, int whence);
int64_t dtcurl_session_get_filesize(dtcurl_session_t *session);
const char *dtcurl_session_get_location(dtcurl_session_t *session);
int dtcurl_session_close(dtcurl_session_t *session);

#endif
