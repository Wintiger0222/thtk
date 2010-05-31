/*
 * Redistribution and use in source and binary forms, with
 * or without modification, are permitted provided that the
 * following conditions are met:
 *
 * 1. Redistributions of source code must retain this list
 *    of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce this
 *    list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 */
#ifndef BITS_H_
#define BITS_H_

#include <config.h>
#include <stdio.h>
#include <inttypes.h>

enum io_type {
    BITSTREAM_STREAM,
    BITSTREAM_BUFFER_FIXED,
    /* For writing only. */
    BITSTREAM_BUFFER_GROW
};

struct bitstream {
    enum io_type type;
    union {
        FILE* stream;
        struct {
            unsigned char* buffer;
            unsigned int size;
        } buffer;
    } io;
    unsigned int byte_count;
    unsigned int byte;
    unsigned int bits;
};

void bitstream_init_stream(struct bitstream* b, FILE* stream);
void bitstream_init_fixed(struct bitstream* b, unsigned char* buffer, unsigned int size);
void bitstream_init_growing(struct bitstream* b, unsigned int size);
/* Closes an FD or frees a buffer. */
void bitstream_free(struct bitstream* b);

unsigned int bitstream_read1(struct bitstream* b);
uint32_t bitstream_read(struct bitstream* b, unsigned int bits);

void bitstream_write1(struct bitstream* b, unsigned int bit);
void bitstream_write(struct bitstream* b, unsigned int bits, uint32_t data);
void bitstream_finish(struct bitstream* b);

#endif
