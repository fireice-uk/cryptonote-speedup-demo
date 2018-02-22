/* 
 * (c) 2017 fireice-uk
 * All rights reserved.
 * 
 */

// Parts of this file are originally copyright (c) 2015 Markku-Juhani O. Saarinen

#include "keccak.h"

#ifndef _MSC_VER
static inline  __attribute__((always_inline)) uint64_t _rotl64(uint64_t x, unsigned n)
{
	return (x << n) | (x >> (64 - n));
}
#endif // _MSC_VER

const uint64_t keccakf_rndc[24] = {
	0x0000000000000001, 0x0000000000008082, 0x800000000000808a,
	0x8000000080008000, 0x000000000000808b, 0x0000000080000001,
	0x8000000080008081, 0x8000000000008009, 0x000000000000008a,
	0x0000000000000088, 0x0000000080008009, 0x000000008000000a,
	0x000000008000808b, 0x800000000000008b, 0x8000000000008089,
	0x8000000000008003, 0x8000000000008002, 0x8000000000000080,
	0x000000000000800a, 0x800000008000000a, 0x8000000080008081,
	0x8000000000008080, 0x0000000080000001, 0x8000000080008008
};

void keccak_keccakf(uint64_t st[25])
{
	for (size_t round = 0; round < 24; round++)
	{
		uint64_t t0, t1, bc0, bc1, bc2, bc3, bc4;
		// Theta
		bc0 = st[0] ^ st[5] ^ st[10] ^ st[15] ^ st[20];
		bc1 = st[1] ^ st[6] ^ st[11] ^ st[16] ^ st[21];
		bc2 = st[2] ^ st[7] ^ st[12] ^ st[17] ^ st[22];
		bc3 = st[3] ^ st[8] ^ st[13] ^ st[18] ^ st[23];
		bc4 = st[4] ^ st[9] ^ st[14] ^ st[19] ^ st[24];

		t0 = bc0;
		t1 = bc1;
		bc0 ^= _rotl64(bc2, 1);
		bc1 ^= _rotl64(bc3, 1);
		bc2 ^= _rotl64(bc4, 1);
		bc3 ^= _rotl64(t0 , 1);
		bc4 ^= _rotl64(t1 , 1);

		// Rho Pi
		t0 = st[1] ^ bc0;
		st[ 0] ^= bc4;
		st[ 1] = _rotl64(st[ 6] ^ bc0, 44);
		st[ 6] = _rotl64(st[ 9] ^ bc3, 20);
		st[ 9] = _rotl64(st[22] ^ bc1, 61);
		st[22] = _rotl64(st[14] ^ bc3, 39);
		st[14] = _rotl64(st[20] ^ bc4, 18);
		st[20] = _rotl64(st[ 2] ^ bc1, 62);
		st[ 2] = _rotl64(st[12] ^ bc1, 43);
		st[12] = _rotl64(st[13] ^ bc2, 25);
		st[13] = _rotl64(st[19] ^ bc3,  8);
		st[19] = _rotl64(st[23] ^ bc2, 56);
		st[23] = _rotl64(st[15] ^ bc4, 41);
		st[15] = _rotl64(st[ 4] ^ bc3, 27);
		st[ 4] = _rotl64(st[24] ^ bc3, 14);
		st[24] = _rotl64(st[21] ^ bc0,  2);
		st[21] = _rotl64(st[ 8] ^ bc2, 55);
		st[ 8] = _rotl64(st[16] ^ bc0, 45);
		st[16] = _rotl64(st[ 5] ^ bc4, 36);
		st[ 5] = _rotl64(st[ 3] ^ bc2, 28);
		st[ 3] = _rotl64(st[18] ^ bc2, 21);
		st[18] = _rotl64(st[17] ^ bc1, 15);
		st[17] = _rotl64(st[11] ^ bc0, 10);
		st[11] = _rotl64(st[ 7] ^ bc1,  6);
		st[ 7] = _rotl64(st[10] ^ bc4,  3);
		st[10] = _rotl64(t0, 1);

		//  Chi
		bc0 = st[ 0];
		bc1 = st[ 1];
		st[ 0] ^= (~st[ 1]) & st[ 2];
		st[ 1] ^= (~st[ 2]) & st[ 3];
		st[ 2] ^= (~st[ 3]) & st[ 4];
		st[ 3] ^= (~st[ 4]) & bc0;
		st[ 4] ^= (~bc0) & bc1;

		bc0 = st[ 5];
		bc1 = st[ 6];
		st[ 5] ^= (~st[ 6]) & st[ 7];
		st[ 6] ^= (~st[ 7]) & st[ 8];
		st[ 7] ^= (~st[ 8]) & st[ 9];
		st[ 8] ^= (~st[ 9]) & bc0;
		st[ 9] ^= (~bc0) & bc1;

		bc0 = st[10];
		bc1 = st[11];
		st[10] ^= (~st[11]) & st[12];
		st[11] ^= (~st[12]) & st[13];
		st[12] ^= (~st[13]) & st[14];
		st[13] ^= (~st[14]) & bc0;
		st[14] ^= (~bc0) & bc1;

		bc0 = st[15];
		bc1 = st[16];
		st[15] ^= (~st[16]) & st[17];
		st[16] ^= (~st[17]) & st[18];
		st[17] ^= (~st[18]) & st[19];
		st[18] ^= (~st[19]) & bc0;
		st[19] ^= (~bc0) & bc1;

		bc0 = st[20];
		bc1 = st[21];
		bc2 = st[22];
		bc3 = st[23];
		bc4 = st[24];

		st[20] ^= (~bc1) & bc2;
		st[21] ^= (~bc2) & bc3;
		st[22] ^= (~bc3) & bc4;
		st[23] ^= (~bc4) & bc0;
		st[24] ^= (~bc0) & bc1;

		//  Iota
		st[0] ^= keccakf_rndc[round];
	}
}

// update state with more data
void keccak_update(keccak_ctx *c, const void *data, size_t len)
{
	size_t i, j;

	j = c->pt;
	for (i = 0; i < len; i++) {
		c->st.b[j++] ^= ((const uint8_t *) data)[i];
		if (j >= 136) {
			keccak_keccakf(c->st.q);
			j = 0;
		}
	}
	c->pt = j;
}

// finalize and output a hash
void keccak_final(keccak_ctx *c, void *md)
{
	c->st.b[c->pt] ^= 0x01;
	c->st.b[135] ^= 0x80;
	keccak_keccakf(c->st.q);

	memcpy(md, c->st.b, 32);
}

void *keccak(const void *in, size_t inlen, void *md)
{
	keccak_ctx keccak = { 0 };

	keccak_update(&keccak, in, inlen);
	keccak_final(&keccak, md);

	return md;
}

void hmac_keccak(const uint8_t* in, size_t inlen, const uint8_t* key, size_t keylen, void* md)
{
	keccak_ctx ctx = { 0 };
	uint8_t hash[32];

	//Slightly modified keccak_update, always do a full round
	for(size_t i=0; i < 136; i++)
	{
		if(i < keylen)
			ctx.st.b[i] = key[i] ^ 0x36;
		else
			ctx.st.b[i] = 0x36;
	}
	keccak_keccakf(ctx.st.q);

	//Feeed in the message
	ctx.pt = 0;
	keccak_update(&ctx, in, inlen);
	keccak_final(&ctx, hash);

	memset(&ctx, 0, sizeof(keccak_ctx));
	for(size_t i=0; i < 136; i++)
	{
		if(i < keylen)
			ctx.st.b[i] = key[i] ^ 0x5c;
		else
			ctx.st.b[i] = 0x5c;
	}
	keccak_keccakf(ctx.st.q);

	ctx.pt = 0;
	keccak_update(&ctx, hash, 32);
	keccak_final(&ctx, md);
}

#define SWAP_UINT32(x) (((x) >> 24) | (((x) & 0x00FF0000) >> 8) | (((x) & 0x0000FF00) << 8) | ((x) << 24))

void pbkdf2_keccak_128(const char* pwd, uint64_t salt, void* dk)
{
	uint64_t U[4], Un[4]; //Hash outputs
	uint32_t iter = SWAP_UINT32(1);
	uint8_t saltiter[12]; //salt with iter for the first hmac
	size_t pwdlen = strlen(pwd);

	memcpy(saltiter, &salt, sizeof(salt));
	memcpy(saltiter+8, &iter, sizeof(iter));

	hmac_keccak((uint8_t*)pwd, pwdlen, saltiter, 12, U);
	for(size_t n = 2; n < 32768; n++)
	{
		hmac_keccak((uint8_t*)pwd, pwdlen, (uint8_t*)U, 32, Un);

		U[0] ^= Un[0];
		U[1] ^= Un[1];
		U[2] ^= Un[2];
		U[3] ^= Un[3];
	}

	memcpy(dk, U, 16);
}
