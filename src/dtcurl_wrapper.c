/*
 * =====================================================================================
 *
 *    Filename   :  dtcurl_wrapper.c
 *    Description:
 *    Version    :  1.0
 *    Created    :  2015年10月30日 10时43分06秒
 *    Revision   :  none
 *    Compiler   :  gcc
 *    Author     :  peter-s
 *    Email      :  peter_future@outlook.com
 *    Company    :  dt
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include <string.h>

#include "dtcurl_wrapper.h"
#include "dtcurl_log.h"
#include "dtcurl_macro.h"

static int curl_wrapper_setopt(dtcurl_wrapper_t *wrapper)
{
    int ret = CURL_ERROR_UNKOWN;
    if (!wrapper) {
        return ret;
    }
    CURLcode code = curl_easy_setopt(wrapper->curl_handle, CURLOPT_ERRORBUFFER, wrapper->curl_error_buf);
    if (code != CURLE_OK) {
        CURL_LOG("curl easy setopt CURLOPT_ERRORBUFFER failed \n");
        return ret;
    }
    code = curl_easy_setopt(wrapper->curl_handle, CURLOPT_CONNECTTIMEOUT, CURL_MAX_CONNECTION_TIMEOUT);
    if (code != CURLE_OK) {
        CURL_LOG("Error: %s \n", wrapper->curl_error_buf);
        return ret;
    }
#if 0
    code = curl_easy_setopt(wrapper->curl_handle, CURLOPT_BUFFERSIZE, CURL_MAX_TIMEOUT);
    if (code != CURLE_OK) {
        CURL_LOG("Error: %s \n", wrapper->curl_error_buf);
        return ret;
    }
#endif
    code = curl_easy_setopt(wrapper->curl_handle, CURLOPT_FORBID_REUSE, 1L);
    if (code != CURLE_OK) {
        CURL_LOG("Error: %s \n", wrapper->curl_error_buf);
        return ret;
    }
    code = curl_easy_setopt(wrapper->curl_handle, CURLOPT_URL, wrapper->uri);
    if (code != CURLE_OK) {
        CURL_LOG("Error: %s \n", wrapper->curl_error_buf);
        return ret;
    }
    code = curl_easy_setopt(wrapper->curl_handle, CURLOPT_MAXREDIRS, CURL_MAX_REDIRECTS);
    if (code != CURLE_OK) {
        CURL_LOG("Error: %s \n", wrapper->curl_error_buf);
        return ret;
    }

    CURL_LOG("curl easy setopt success \n");
    return CURL_ERROR_NONE;
}

int dtcurl_wrapper_init(dtcurl_wrapper_t *wrapper, const char *uri)
{
    int ret = CURL_ERROR_NONE;
    memset(wrapper, 0, sizeof(dtcurl_wrapper_t));
    wrapper->uri = strdup(uri);
    if (!wrapper->curl_handle) {
        wrapper->curl_handle = curl_easy_init();
        if (wrapper->curl_handle) {
            CURL_LOG("curl easy init ok \n");
        }
    }
    ret = dtcurl_buf_init(&wrapper->cache, CURL_MAX_CACHE_SIZE);
    if (ret < 0) {
        CURL_LOG("dtcurl cache init failed \n");
        return ret;
    }

    // protocal check
    if (dtcurl_stristart(uri, "http://", NULL)) {
        wrapper->proto = CURL_PROTO_HTTP;
    } else if (dtcurl_stristart(uri, "httpis://", NULL)) {
        wrapper->proto = CURL_PROTO_HTTPS;
    }

    if (wrapper->proto == CURL_PROTO_HTTP || wrapper->proto == CURL_PROTO_HTTPS) {
        ret = curl_wrapper_setopt(wrapper);
        if (ret < 0) {
            return ret;
        }
    }

    return CURL_ERROR_NONE;
}

static void curl_download_loop(void *priv)
{
    CURL_LOG("download loop enter\n");
    dtcurl_wrapper_t *wrapper = (dtcurl_wrapper_t *)priv;

    CURL_LOG("download loop exit\n");
}

int dtcurl_wrapper_start(dtcurl_wrapper_t *wrapper)
{
    int ret = CURL_ERROR_UNKOWN;
    CURL_LOG("create download thread\n");
    ret = pthread_create(&wrapper->download_pid, curl_download_loop, wrapper);
    return CURL_ERROR_NONE;
}
