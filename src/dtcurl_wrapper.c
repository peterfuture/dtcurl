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
#include <unistd.h>

#include "dtcurl_wrapper.h"
#include "dtcurl_log.h"
#include "dtcurl_macro.h"

static size_t write_function(void *buffer, size_t size, size_t nmemb, void *userp)
{
    dtcurl_wrapper_t *wrapper = (dtcurl_wrapper_t *)userp;

    //CURL_LOG("get data size:%d nmeb:%d\n", size, nmemb);
    //CURL_LOG("content:%s\n", buffer);

    int total = size * nmemb;
    int space = dtcurl_buf_space(&wrapper->cache);
    while (space < total) {
        usleep(10 * 1000);
        CURL_LOG("space:%d total:%d\n", space, total);
    }

    int ret = dtcurl_buf_put(&wrapper->cache, buffer, total);
    if (ret != total) {
        CURL_LOG("Error, write failed. total:%d ret:%d \n", total, ret);
    }
    return nmemb * size;
}

static int process_line(http_context_t *http, char *line)
{
    char *ptr = line;

    if (line[0] == '\0') {
        http->end_header = 1;
        return 0;
    }

    if (!strncasecmp(line, "HTTP", 4)) {
        while (!isspace(*ptr) && *ptr != '\0') {
            ptr++;
        }
        http->http_code = (int)strtol(ptr, NULL, 10);
        CURL_LOG("response code:%d \n", http->http_code);
        return 0;
    }

    if (http->http_code == 200) {
        if (!strncasecmp(line, "Content-Length", 14)) {
            while (*ptr != '\0' && *ptr != ':') {
                ptr++;
            }
            ptr++;
            while (isspace(*ptr)) {
                ptr++;
            }
            http->filesize = atoll(ptr);
        }

        if (!strncasecmp(line, "Content-Range", 13)) {
            const char * slash = NULL;
            if ((slash = strchr(ptr, '/')) && strlen(slash) > 0) {
                http->chunksize = atoll(slash + 1);
            }
        }

        if (!strncasecmp(line, "Transfer-Encoding", 17) && strstr(line, "chunked")) {
            http->filesize = 0;
            http->chunksize = 0;
        }
        return 0;
    }

    if (206 == http->http_code) {
        if (!strncasecmp(line, "Content-Range", 13)) {
            const char * slash = NULL;
            if ((slash = strchr(ptr, '/')) && strlen(slash) > 0) {
                http->chunksize = atoll(slash + 1);
            }
        }
        return 0;
    }

    // redirect
    if (http->http_code > 300 && http->http_code < 400) {
        if (!strncasecmp(line, "Location", 8)) {
            while (*ptr != '\0' && *ptr != ':') {
                ptr++;
            }
            ptr++;
            while (isspace(*ptr)) {
                ptr++;
            }
            http->location = strdup(ptr);
        }
        return 0;
    }

    //ERR CASE
    if (http->http_code > 400) {
        http->end_header = 1;
        return CURL_ERROR_UNKOWN;
        //return ERROR;
    }

    if (strcasestr(line, "Octoshape-Ondemand")) {
        http->is_streamed = 1;
        return 0;
    }

    return 0;
}

static size_t header_function(void *buffer, size_t size, size_t nmemb, void *userp)
{
    dtcurl_wrapper_t *wrapper = (dtcurl_wrapper_t *)userp;
    http_context_t *http = &wrapper->http;
    //CURL_LOG("response size:%d nmeb:%d\n", size, nmemb);
    char *line = (char *)buffer;
    CURL_LOG("process line:%s\n", line);
    int ret = process_line(http, line);
    if (ret < 0) {
        return ret;
    }
    return nmemb * size;
}

static int curl_wrapper_setopt(dtcurl_wrapper_t *wrapper)
{
    int ret = CURL_ERROR_UNKOWN;
    if (!wrapper) {
        return ret;
    }
    CURLcode code;
#if 0
    code = curl_easy_setopt(wrapper->curl_handle, CURLOPT_ERRORBUFFER, wrapper->curl_error_buf);
    if (code != CURLE_OK) {
        CURL_LOG("curl easy setopt CURLOPT_ERRORBUFFER failed \n");
        return ret;
    }
    code = curl_easy_setopt(wrapper->curl_handle, CURLOPT_CONNECTTIMEOUT, CURL_MAX_CONNECTION_TIMEOUT);
#endif
#if 0
    code = curl_easy_setopt(wrapper->curl_handle, CURLOPT_BUFFERSIZE, CURL_MAX_TIMEOUT);
    code = curl_easy_setopt(wrapper->curl_handle, CURLOPT_FORBID_REUSE, 1L);
#endif
    code = curl_easy_setopt(wrapper->curl_handle, CURLOPT_URL, wrapper->uri);
    code = curl_easy_setopt(wrapper->curl_handle, CURLOPT_MAXREDIRS, CURL_MAX_REDIRECTS);
    code = curl_easy_setopt(wrapper->curl_handle, CURLOPT_WRITEFUNCTION, write_function);
    code = curl_easy_setopt(wrapper->curl_handle, CURLOPT_WRITEDATA, (void *)wrapper);
    code = curl_easy_setopt(wrapper->curl_handle, CURLOPT_HEADERFUNCTION, header_function);
    code = curl_easy_setopt(wrapper->curl_handle, CURLOPT_WRITEHEADER, (void *)wrapper);

    //curl_easy_setopt(wrapper->curl_handle, CURLOPT_VERBOSE, 1L);
    //curl_easy_setopt(wrapper->curl_handle, CURLOPT_DEBUGFUNCTION, debug_function);
    curl_easy_setopt(wrapper->curl_handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_0);
    curl_easy_setopt(wrapper->curl_handle, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(wrapper->curl_handle, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_multi_setopt(wrapper->curl_handle, CURLMOPT_PIPELINING, CURLPIPE_MULTIPLEX);
    curl_easy_setopt(wrapper->curl_handle, CURLOPT_PIPEWAIT, 1L);
    CURL_LOG("curl easy setopt success \n");
    return CURL_ERROR_NONE;
}

int dtcurl_wrapper_init(dtcurl_wrapper_t *wrapper, const char *uri)
{
    int ret = CURL_ERROR_NONE;
    memset(wrapper, 0, sizeof(dtcurl_wrapper_t));
    wrapper->uri = strdup(uri);
    wrapper->multi_handle = curl_multi_init();
    if (!wrapper->multi_handle) {
        CURL_LOG("curl multi init failed \n");
        return CURL_ERROR_UNKOWN;
    }
    wrapper->curl_handle = curl_easy_init();
    if (!wrapper->curl_handle) {
        CURL_LOG("curl easy init failed \n");
        return CURL_ERROR_UNKOWN;
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
    } else {
        CURL_LOG("Protocal not support yet \n");
        return CURL_ERROR_UNKOWN;
    }

    return CURL_ERROR_NONE;
}

static void curl_download_loop(void *priv)
{
    CURL_LOG("download loop enter\n");
    dtcurl_wrapper_t *wrapper = (dtcurl_wrapper_t *)priv;
    //curl_easy_perform(wrapper->curl_handle);
    CURLcode code = curl_multi_add_handle(wrapper->multi_handle, wrapper->curl_handle);
    if (code != CURLM_OK) {
        return CURL_ERROR_UNKOWN;
    }
    int running_handle_cnt = 0;
    int select_zero_cnt = 0;
    int select_breakout_flag = 0;

    curl_multi_perform(wrapper->multi_handle, &running_handle_cnt);
    CURL_LOG("handle cnt:%d \n", running_handle_cnt);
    while (running_handle_cnt) {
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 200 * 1000;

        struct timeval timeout;
        int rc;
        CURLMcode mc;
        fd_set fdread;
        fd_set fdwrite;
        fd_set fdexcept;
        int maxfd = -1;

        long curl_timeo = -1;

        FD_ZERO(&fdread);
        FD_ZERO(&fdwrite);
        FD_ZERO(&fdexcept);

        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        curl_multi_timeout(wrapper->multi_handle, &curl_timeo);
        if (curl_timeo >= 0) {
            timeout.tv_sec = curl_timeo / 1000;
            if (timeout.tv_sec > 1) {
                timeout.tv_sec = 1;
            } else {
                timeout.tv_usec = (curl_timeo % 1000) * 1000;
            }
        }

        mc = curl_multi_fdset(wrapper->multi_handle, &fdread, &fdwrite, &fdexcept, &maxfd);

        if (mc != CURLM_OK) {
            CURL_LOG("curl_multi_fdset() failed, code %d.\n", mc);
            break;
        }

        rc = select(maxfd + 1, &fdread, &fdwrite, &fdexcept, &timeout);
        switch (rc) {
        case -1:
            CURL_LOG("select error!\n");
            break;
        case 0:
            //select_zero_cnt++;
            //CURL_LOG("select return 0 cnt:%d running_handle:%d \n", select_zero_cnt, running_handle_cnt);
            //break;
        default:
            select_zero_cnt = 0;
            curl_multi_perform(wrapper->multi_handle, &running_handle_cnt);
            CURL_LOG("runing_handle_count : %d rc:%d\n", running_handle_cnt, rc);
            break;
        }
        CURL_LOG("select cnt:%d running_handle:%d\n", select_zero_cnt, running_handle_cnt);
        if (select_zero_cnt == CURL_MAX_SELECT_RETRY_TIME) {
            select_breakout_flag = 1;
            break;
        }
    }

    if (select_breakout_flag) {
        CURL_LOG("select breakout\n");
        return CURL_ERROR_SELECT_ERROR;
    }

    int msgs_left;
    CURLMsg *msg;
    CURLcode retcode;
    int ret = CURL_ERROR_NONE;
    while ((msg = curl_multi_info_read(wrapper->multi_handle, &msgs_left))) {
        if (msg->msg == CURLMSG_DONE) {
            if (msg->easy_handle == wrapper->curl_handle) {
                break;
            }
            CURL_LOG("download completed with status: [%d]\n", msg->data.result);
            long arg = 0;
            char * info = NULL;
            retcode = curl_easy_getinfo(wrapper->curl_handle, CURLINFO_RESPONSE_CODE, (long *)(&info));
            if (CURLE_OK == retcode) {
                CURL_LOG("response_code: [%ld]\n", info);
            }
            double speed = 0.0;
            retcode = curl_easy_getinfo(wrapper->curl_handle, CURLINFO_SPEED_DOWNLOAD, (double *)&speed);
            if (CURLE_OK == retcode) {
                CURL_LOG("download speed is: %0.2f kbps\n", (speed * 8) / 1024.00);
            }
        }
    }
    CURL_LOG("download loop exit ret:%d\n", ret);
}

int dtcurl_wrapper_start(dtcurl_wrapper_t *wrapper)
{
    int ret = CURL_ERROR_UNKOWN;
    pthread_t tid;
    CURL_LOG("create download thread\n");
    ret = pthread_create(&tid, NULL, curl_download_loop, wrapper);
    pthread_setname_np(tid, "curl_download_thread");
    wrapper->download_pid = tid;
    return CURL_ERROR_NONE;
}

int dtcurl_wrapper_read(dtcurl_wrapper_t *wrapper, char *buf, int size)
{
    int level = dtcurl_buf_level(&wrapper->cache);
    int rsize = CURL_MIN(size, level);
    if (rsize > 0) {
        dtcurl_buf_get(&wrapper->cache, buf, rsize);
    }
    CURL_LOG("read rsize:%d level:%d size:%d\n", rsize, level, size);
    return rsize;
}

int dtcurl_wrapper_seek(dtcurl_wrapper_t *wrapper, int64_t pos, int whence)
{
    return CURL_ERROR_NONE;
}
