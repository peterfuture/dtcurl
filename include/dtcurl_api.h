/*
 * =====================================================================================
 *
 *    Filename   :  dtcurl_api.h
 *    Description:
 *    Version    :  1.0
 *    Created    :  2015年11月05日 17时25分00秒
 *    Revision   :  none
 *    Compiler   :  gcc
 *    Author     :  peter-s (), peter_future@outlook.com
 *    Company    :  dt
 *
 * =====================================================================================
 */

#ifndef DTCURL_API_H
#define DTCURL_API_H

#include <stdint.h>

enum keys {
    // GET
    KEY_CURL_GET_FILESIZE = 0,
    KEY_CURL_GET_LOCATION = 1,
    KEY_CURL_GET_MAX,
    // SET
    KEY_CURL_SET_MAX
};

int dtcurl_init(void **priv, const char *uri);
int dtcurl_read(void *priv, char *buf, int size);
int dtcurl_seek(void *priv, int64_t off, int whence);
int dtcurl_set_parameter(void *priv, int key, void *value);
int dtcurl_get_parameter(void *priv, int key, void *value);
int dtcurl_close(void *priv);

#endif
