#ifndef HLS_BUFFER_T
#define HLS_BUFFER_T

#include "dtcurl_lock.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    uint8_t *data;
    int size;
    int level;
    uint8_t *rd_ptr;
    uint8_t *wr_ptr;
    dtcurl_lock_t mutex;
} dtcurl_buffer_t;

int dtcurl_buf_init(dtcurl_buffer_t * dbt, int size);
int dtcurl_buf_reinit(dtcurl_buffer_t * dbt);
int dtcurl_buf_release(dtcurl_buffer_t * dbt);
int dtcurl_buf_space(dtcurl_buffer_t * dbt);
int dtcurl_buf_level(dtcurl_buffer_t * dbt);
int dtcurl_buf_get(dtcurl_buffer_t * dbt, uint8_t * out, int size);
int dtcurl_buf_put(dtcurl_buffer_t * dbt, uint8_t * in, int size);
#endif
