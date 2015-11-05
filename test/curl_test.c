/*
 * =====================================================================================
 *
 *    Filename   :  curl_test.c
 *    Description:
 *    Version    :  1.0
 *    Created    :  2015年10月29日 14时40分43秒
 *    Revision   :  none
 *    Compiler   :  gcc
 *    Author     :  peter-s
 *    Email      :  peter_future@outlook.com
 *    Company    :  dt
 *
 * =====================================================================================
 */

#include <stdio.h>
#include "dtcurl_api.h"

int main(int argc, char **argv)
{
    void *curl_ctx;
    const char *uri = "http://devimages.apple.com/iphone/samples/bipbop/gear1/prog_index.m3u8";
    //const char *uri = "http://www.modrails.com/videos/passenger_nginx.mov";
    dtcurl_init(&curl_ctx, uri);

    char buf[1024];
    int ret = 0;
    while (1) {
        ret = dtcurl_read(curl_ctx, buf, 1024);
        if (ret > 0) {
            printf("read %d bytes \n", ret);
        }

        if (ret < 0) {
            break;
        }
        usleep(10000);
    }
    dtcurl_seek(curl_ctx, 20, SEEK_SET);
    while (1) {
        ret = dtcurl_read(curl_ctx, buf, 1024);
        if (ret > 0) {
            printf("read %d bytes \n", ret);
        }

        if (ret < 0) {
            break;
        }
        usleep(10000);
    }
    dtcurl_close(curl_ctx);
    printf("test end \n");
    return 0;
}
