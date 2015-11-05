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

int dtcurl_init(void **priv, const char *uri);
int dtcurl_read(void *priv, char *buf, int size);
int dtcurl_seek(void *priv, int64_t off, int whence);
int dtcurl_close(void *priv);

#endif
