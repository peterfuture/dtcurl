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
#include "dtcurl_priv.h"

int main(int argc, char **argv)
{
    void *curl_ctx;
    const char *uri = "http://devimages.apple.com/iphone/samples/bipbop/gear1/prog_index.m3u8";
    dtcurl_init(&curl_ctx, uri);
    return 0;
}
