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

typedef struct dtcurl_session {
    char *uri;
    dtcurl_wrapper_t dtcurl_wrapper;
} dtcurl_session_t;
