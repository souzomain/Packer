
/*
 * 
 *  Created by souzomain
 *
 * */

#include "Packer.h"

#include <stdbool.h>

#ifndef _WIN32
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#else
#include <windows.h>
#endif

#ifndef htonll
#if __BYTE_ORDER == __LITTLE_ENDIAN
#define htonll(x) ((uint64_t)htonl((x) & 0xFFFFFFFF) << 32) | htonl((x) >> 32)
#else
#define htonll(x) (x)
#endif
#endif

#ifndef ntohll
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define ntohll(x) __builtin_bswap64(x)
#else
#define ntohll(x) (x)
#endif
#endif


enum {
    INITIAL_BUFFER_SIZE = 1024,
    BUFFER_SIZE_INCREMENT = 1024
};

PPACKER packer_init() {
    PPACKER buf = (PPACKER)malloc(sizeof(PACKER));
    if(buf == NULL) return NULL;

    buf->buffer = calloc(INITIAL_BUFFER_SIZE, 1);
    if(!buf->buffer) { free(buf); return NULL; }
    buf->size = INITIAL_BUFFER_SIZE;
    buf->offset = 0;
    return buf;
}

void packer_free(PPACKER buf) {
    if(!buf) return;
    free(buf->buffer);
    buf->buffer = NULL;
    buf->size = 0;
    buf->offset = 0;
    free(buf);
}

bool packer_resize_buffer(PPACKER buf, size_t new_size) {
    if (new_size > buf->size) {
        size_t size = new_size + BUFFER_SIZE_INCREMENT - (new_size  % BUFFER_SIZE_INCREMENT);
        void *ptr = realloc(buf->buffer, size);
        if(!ptr)
            return false;
        
        buf->size = size;
        buf->buffer = ptr;
    }
    return true;
}

size_t packer_get_size(PPACKER buf){
    if(!buf) return 0;
    return buf->offset;
}

uint8_t* packer_get_buffer(PPACKER buf){
    if(!buf) return NULL;
    return buf->buffer;
}

void packer_add_int32(PPACKER buf, int32_t value) {
    if(!buf) return;
    if(!packer_resize_buffer(buf, buf->offset + sizeof(int32_t))) return;
    value = htonl(value);
    memcpy(buf->buffer + buf->offset, &value, sizeof(int32_t));
    buf->offset += sizeof(int32_t);
}

int32_t packer_get_int32(const uint8_t *buffer, size_t *offset) {
    if(!buffer || !offset) return 0;
    int32_t value = 0;
    memcpy(&value, buffer + *offset, sizeof(int32_t));
    *offset += sizeof(int32_t);
    return ntohl(value);
}

void packer_add_int64(PPACKER buf, int64_t value) {
    if(!buf) return;
    if(!packer_resize_buffer(buf, buf->offset + sizeof(int64_t))) return;
    value = htonll(value);
    memcpy(buf->buffer + buf->offset, &value, sizeof(int64_t));
    buf->offset += sizeof(int64_t);
}

int64_t packer_get_int64(const uint8_t *buffer, size_t *offset) {
    if(!buffer || !offset) return 0;
    int64_t value = 0;
    memcpy(&value, buffer + *offset, sizeof(int64_t));
    *offset += sizeof(int64_t);
    return ntohll(value);
}

void packer_add_data(PPACKER buf, const void *data, size_t data_len) {
    if(!buf || !data ) return;
    if(!packer_resize_buffer(buf, buf->offset + sizeof(int32_t) + data_len)) return;
    packer_add_int32(buf, (int32_t)data_len);
    memcpy(buf->buffer + buf->offset, data, data_len);
    buf->offset += data_len;
}

void* packer_get_data(const uint8_t *buffer, size_t* data_len, size_t *offset) {
    if(!buffer || !data_len || !offset) return NULL;

    int32_t len = packer_get_int32(buffer, offset);

    void *data = (void *)(buffer + *offset);
    *data_len = len;
    *offset += len;
    return data;
}

void packer_add_string(PPACKER buf, const char * str){
    if(!buf || !str) return;
    size_t str_len = strlen(str);
    if(!packer_resize_buffer(buf, buf->offset + sizeof(int32_t) + str_len))  return;
    packer_add_int32(buf, (int32_t)str_len);
    memcpy(buf->buffer + buf->offset, str, str_len);
    buf->offset += str_len;
}

char* packer_get_string(const uint8_t* buffer, size_t *offset) {
    if (!buffer || !offset) return NULL;

    int32_t len = packer_get_int32(buffer, offset);

    char *str = (char *)(buffer + *offset);
    *offset += len;

    return str;
}
