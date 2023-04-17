
/*
 * 
 * created by souzomain
 *
 * */

#ifndef _PACKER_H
#define _PACKER_H

#include <stdint.h>
#include <stddef.h>


typedef struct {
    uint8_t *buffer;
    size_t size;
    size_t offset;
} PACKER, *PPACKER;


PPACKER     packer_init();
void        packer_free(PPACKER buf);

size_t      packer_get_size(PPACKER buf);
uint8_t*    packer_get_buffer(PPACKER buf);

void        packer_add_int32(PPACKER buf, int32_t value);
int32_t     packer_get_int32(const uint8_t *buffer, size_t *offset);


void        packer_add_int64(PPACKER buf, int64_t value);
int64_t     packer_get_int64(const uint8_t *buffer, size_t *offset);

void        packer_add_data(PPACKER buf, const void *data, size_t data_len);
void*       packer_get_data(const uint8_t *buffer, size_t* data_len, size_t *offset);

void        packer_add_string(PPACKER buf, const char * str);
char*       packer_get_string(const uint8_t* buffer, size_t *offset);

#endif
