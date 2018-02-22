/* 
 * (c) 2017 fireice-uk
 * All rights reserved.
 * 
 */

// Parts of this file are originally copyright (c) 2015 Markku-Juhani O. Saarinen

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// state context
typedef struct {
    union {                                 // state:
        uint8_t b[200];                     // 8-bit bytes
        uint64_t q[25];                     // 64-bit words
    } st;
    size_t pt;
} keccak_ctx;

// Compression function.
void keccakf(uint64_t st[25]);

inline void keccak_init(keccak_ctx *c) { memset(c, 0, sizeof(keccak_ctx)); }
void keccak_update(keccak_ctx *c, const void *data, size_t len);
void keccak_final(keccak_ctx *c, void *md);

void *keccak(const void *in, size_t inlen, void *md);

void hmac_keccak(const uint8_t* in, size_t inlen, const uint8_t* key, size_t keylen, void* md);
void pbkdf2_keccak_128(const char* pwd, uint64_t salt, void* dk);

#ifdef __cplusplus
}
#endif // __cplusplus


