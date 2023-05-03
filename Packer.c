
/*
 * 
 *  Created by souzomain
 *
 * */

#include "Packer.h"


#include <stdlib.h>


/*Custom functions to increase speed */

#define PackerZero(x, y) packer_memset(x, 0, y)

uint32_t packer_ntohl(uint32_t value) {
    return ((value & 0xFF) << 24) | ((value >> 8 & 0xFF) << 16) | ((value >> 16 & 0xFF) << 8) | ((value >> 24) & 0xFF);
}

uint64_t packer_ntohll(uint64_t value) {
    uint32_t high = packer_ntohl(value & 0xFFFFFFFF);
    uint32_t low = packer_ntohl(value >> 32);
    return ((uint64_t)high << 32) | low;
}

uint32_t packer_htonl(uint32_t value) {
    return ((value & 0xFF) << 24) | ((value >> 8 & 0xFF) << 16) | ((value >> 16 & 0xFF) << 8) | ((value >> 24) & 0xFF);
}

uint64_t packer_htonll(uint64_t value) {
    uint32_t high = value >> 32;
    uint32_t low = value & 0xFFFFFFFF;
    return ((uint64_t)packer_htonl(high) << 32) | packer_htonl(low);
}

size_t packer_strlen(const char *str){
    size_t len = 0;
    while(*str++)
        ++len;
    return len;
}

void *packer_memset(void *ptr, uint64_t value, uint64_t num) {
    uint8_t *byte_ptr = (uint8_t *)ptr;
    for (size_t i = 0; i < num; i++) 
        byte_ptr[i] = (uint8_t)value;
    
    return ptr;
}

void *packer_memcpy(void *dest, const void *src, size_t n){
    char *cdest = (char *)dest;
    const char *csrc = (const char *)src;
    for(size_t i = 0; i < n; ++i)
        cdest[i] = csrc[i];
    return dest;
}

// permit you change to add custom allocations memory

void *packer_malloc(size_t size){
    return malloc(size);
}

void *packer_calloc(size_t nmemb, size_t size) {
    size_t total_size = nmemb * size;
    void *ptr = packer_malloc(total_size);
    if (ptr != NULL) 
        PackerZero(ptr, total_size);
    
    return ptr;
}

void *packer_realloc(void *ptr, size_t size){
    if(size <= 0) {free(ptr); return NULL;}
    return realloc(ptr, size);
}

/*End custom*/

PPACKER packer_init() {
    PPACKER buf = (PPACKER)packer_calloc(sizeof(PACKER), 1);
    if(buf == NULL) return NULL;

    buf->buffer = NULL;
    buf->size = 0;
    buf->offset = 0;
    return buf;
}

void packer_free(PPACKER buf) {
    if(!buf) return;
    PackerZero(buf->buffer, buf->size);
    free(buf->buffer);
    buf->buffer = NULL;
    buf->size = 0;
    buf->offset = 0;
    PackerZero(buf, sizeof(PACKER));
    free(buf);
}

bool packer_resize_buffer(PPACKER buf, size_t new_size) {
    if (new_size > buf->size) {
        void *ptr = packer_realloc(buf->buffer, new_size);
        if(!ptr)
            return false;
        
        buf->size = new_size;
        buf->buffer = ptr;
    }
    return true;
}

uint64_t packer_get_size(PPACKER buf){
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
    value = packer_htonl(value);
    *(int32_t *)(buf->buffer + buf->offset) = value;
    buf->offset += sizeof(int32_t);
}

int32_t packer_get_int32(const uint8_t *buffer, uint64_t *offset) {
    if(!buffer || !offset) return 0;
    int32_t value = *(int32_t *)(buffer + *offset);
    *offset += sizeof(int32_t);
    return packer_ntohl(value);
}

void packer_add_uint32(PPACKER buf, uint32_t value){
    if(!buf) return;
    if(!packer_resize_buffer(buf, buf->offset + sizeof(uint32_t))) return;
    value = packer_htonl(value);
    *(uint32_t *)(buf->buffer + buf->offset) = value;
    buf->offset += sizeof(uint32_t);
}

uint32_t packer_get_uint32(const uint8_t *buffer, uint64_t *offset){
    if(!buffer || !offset) return 0;
    uint32_t value = *(uint32_t *)(buffer + *offset);
    *offset += sizeof(uint32_t);
    return packer_ntohl(value);
}

void packer_add_int64(PPACKER buf, int64_t value) {
    if(!buf) return;
    if(!packer_resize_buffer(buf, buf->offset + sizeof(int64_t))) return;
    value = packer_htonll(value);
    *(int64_t *)(buf->buffer + buf->offset) = value;
    buf->offset += sizeof(int64_t);
}

int64_t packer_get_int64(const uint8_t *buffer, uint64_t *offset) {
    if(!buffer || !offset) return 0;
    int64_t value = *(int64_t *)(buffer + *offset);
    *offset += sizeof(int64_t);
    return packer_ntohll(value);
}

void packer_add_uint64(PPACKER buf, uint64_t value) {
    if(!buf) return;
    if(!packer_resize_buffer(buf, buf->offset + sizeof(uint64_t))) return;
    value = packer_htonll(value);
    *(uint64_t *)(buf->buffer + buf->offset) = value;
    buf->offset += sizeof(uint64_t);
}

uint64_t packer_get_uint64(const uint8_t *buffer, uint64_t *offset) {
    if(!buffer || !offset) return 0;
    uint64_t value = *(uint64_t *)(buffer + *offset);
    *offset += sizeof(uint64_t);
    return packer_ntohll(value);
}

void packer_add_double(PPACKER buf, double value) {
    if (!buf) return;
    if (!packer_resize_buffer(buf, buf->offset + sizeof(double))) return;
    uint64_t value_uint64 = packer_htonll(*((uint64_t*)&value));
    *(uint64_t *)(buf->buffer + buf->offset) = value_uint64;
    buf->offset += sizeof(double);
}

double packer_get_double(const uint8_t* buffer, size_t* offset) {
    if (!buffer || !offset) return 0;
    uint64_t value_uint64 = *(uint64_t*)(buffer + *offset);
    *offset += sizeof(double);
    uint64_t value_uint64_net = packer_ntohll(value_uint64);
    double value = *((double*)&value_uint64_net);
    return value;
}

void packer_add_bool(PPACKER buf, bool value) {
    if (!buf) return;
    if (!packer_resize_buffer(buf, buf->offset + sizeof(bool))) return;
    *(bool*)(buf->buffer + buf->offset) = value;
    buf->offset += sizeof(bool);
}

bool packer_get_bool(const uint8_t* buffer, size_t* offset) {
    if (!buffer || !offset) return false;
    bool value = *(bool*)(buffer + *offset);
    *offset += sizeof(bool);
    return value;
}

#ifdef FLOAT_SUPPORT
void packer_add_float(PPACKER buf, float value) {
    if (!buf) return;
    if (!packer_resize_buffer(buf, buf->offset + sizeof(float))) return;
    uint32_t netValue = packer_htonl(*((uint32_t*)&value));
    *(uint32_t *)(buf->buffer + buf->offset) = netValue;
    buf->offset += sizeof(float);
}

float packer_get_float(const uint8_t* buffer, size_t* offset) {
    if (!buffer || !offset) return 0.0f;
    uint32_t netValue = *(uint32_t *)(buffer + *offset);
    *offset += sizeof(uint32_t);
    return *((float*)&netValue);
}
#endif

void packer_add_data(PPACKER buf, const void *data, uint32_t data_len) {
    if(!buf || !data ) return;
    packer_add_uint32(buf, (int64_t)data_len);
    if(!packer_resize_buffer(buf, buf->offset + data_len)) return;
    packer_memcpy(buf->buffer + buf->offset, data, data_len);
    buf->offset += data_len;
}

void* packer_get_data(const uint8_t *buffer, uint32_t* data_len, uint64_t *offset) {
    if(!buffer || !data_len || !offset) return NULL;

    uint32_t len = packer_get_uint32(buffer, offset);

    void *data = (void *)(buffer + *offset);
    *data_len = len;
    *offset += len;
    return data;
}

void packer_add_data64(PPACKER buf, const void *data, uint64_t data_len){
    if(!buf || !data ) return;
    packer_add_uint64(buf, (int64_t)data_len);
    if(!packer_resize_buffer(buf, buf->offset + data_len)) return;
    packer_memcpy(buf->buffer + buf->offset, data, data_len);
    buf->offset += data_len;
}

void* packer_get_data64(const uint8_t *buffer, uint64_t* data_len, uint64_t *offset){
    if(!buffer || !data_len || !offset) return NULL;
    uint64_t len = packer_get_uint64(buffer, offset);
    void *data = (void *)(buffer + *offset);
    *data_len = len;
    *offset += len;
    return data;
}

void packer_add_string(PPACKER buf, const char *str){
    if(!buf || !str) return;
    size_t str_len = packer_strlen(str);
    packer_add_uint32(buf, (uint32_t)str_len);
    if(!packer_resize_buffer(buf, buf->offset + str_len))  return;
    packer_memcpy(buf->buffer + buf->offset, str, str_len);
    buf->offset += str_len;
}

char* packer_get_string(const uint8_t* buffer, uint64_t *offset) {
    if (!buffer || !offset) return NULL;
    uint32_t len = packer_get_uint32(buffer, offset);
    char *str = (char *)(buffer + *offset);
    *offset += len;
    return str;
}

void packer_add_string64(PPACKER buf, const char * str){
    if(!buf || !str) return;
    size_t str_len = packer_strlen(str);
    packer_add_uint64(buf, (uint64_t)str_len);
    if(!packer_resize_buffer(buf, buf->offset + str_len))  return;
    packer_memcpy(buf->buffer + buf->offset, str, str_len);
    buf->offset += str_len;
}

char* packer_get_string64(const uint8_t* buffer, uint64_t *offset){
    if (!buffer || !offset) return NULL;
    uint64_t len = packer_get_uint64(buffer, offset);
    char *str = (char *)(buffer + *offset);
    *offset += len;
    return str;
}
