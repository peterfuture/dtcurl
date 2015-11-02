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

static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp)
{
    dtcurl_wrapper_t *wrapper = (dtcurl_wrapper_t *)userp;
    
    CURL_LOG("get data size:%d nmeb:%d\n", size, nmemb);
    //CURL_LOG("content:%s\n", buffer);
    
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
    if (code != CURLE_OK) {
        CURL_LOG("Error: %s \n", wrapper->curl_error_buf);
        return ret;
    }
#endif
#if 0
    code = curl_easy_setopt(wrapper->curl_handle, CURLOPT_BUFFERSIZE, CURL_MAX_TIMEOUT);
    if (code != CURLE_OK) {
        CURL_LOG("Error: %s \n", wrapper->curl_error_buf);
        return ret;
    }
    code = curl_easy_setopt(wrapper->curl_handle, CURLOPT_FORBID_REUSE, 1L);
    if (code != CURLE_OK) {
        CURL_LOG("Error: %s \n", wrapper->curl_error_buf);
        return ret;
    }
#endif
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
    code = curl_easy_setopt(wrapper->curl_handle, CURLOPT_WRITEFUNCTION, write_data);
    if (code != CURLE_OK) {
        CURL_LOG("Error: %s \n", wrapper->curl_error_buf);
        return ret;
    }
    code = curl_easy_setopt(wrapper->curl_handle, CURLOPT_WRITEDATA, &wrapper);
    if (code != CURLE_OK) {
        CURL_LOG("Error: %s \n", wrapper->curl_error_buf);
        return ret;
    }


    curl_easy_setopt(wrapper->curl_handle, CURLOPT_VERBOSE, 1L);
    //curl_easy_setopt(wrapper->curl_handle, CURLOPT_DEBUGFUNCTION, my_trace);
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
    if(!wrapper->multi_handle)
    {
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
    }
    else {
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
    if(code != CURLM_OK)
        return CURL_ERROR_UNKOWN;
    int running_handle_cnt = 0;
    int select_zero_cnt = 0;
    int select_breakout_flag = 0;
    
    curl_multi_perform(wrapper->multi_handle, &running_handle_cnt);
    CURL_LOG("handle cnt:%d \n", running_handle_cnt);
    while(running_handle_cnt)
    {
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 200*1000;

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
        if(curl_timeo >= 0) {
            timeout.tv_sec = curl_timeo / 1000;
            if(timeout.tv_sec > 1)
                timeout.tv_sec = 1;
            else
                timeout.tv_usec = (curl_timeo % 1000) * 1000;
        }

        mc = curl_multi_fdset(wrapper->multi_handle, &fdread, &fdwrite, &fdexcept, &maxfd);

        if(mc != CURLM_OK)
        {
            CURL_LOG("curl_multi_fdset() failed, code %d.\n", mc);
            break;
        }

        rc = select(maxfd + 1, &fdread, &fdwrite, &fdexcept, &timeout);
        switch(rc)
        {
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
        if(select_zero_cnt == CURL_MAX_SELECT_RETRY_TIME)
        {
            select_breakout_flag = 1;
            break;
        }
    }

    if(select_breakout_flag)
    {
        CURL_LOG("select breakout\n");
        return CURL_ERROR_SELECT_ERROR;
    }
   
    int msgs_left;
    CURLMsg *msg;
    CURLcode retcode;
    int ret = CURL_ERROR_NONE;
    while ((msg = curl_multi_info_read(wrapper->multi_handle, &msgs_left))) {
        if (msg->msg == CURLMSG_DONE) {
            if(msg->easy_handle == wrapper->curl_handle)
                break;
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
    while(!wrapper->request_quit)
    {
        usleep(100*1000);
    }
    return CURL_ERROR_NONE;
}
