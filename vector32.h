/* 
 * (c) 2017 fireice-uk
 * All rights reserved.
 * 
 */

#pragma once

#include <string.h>

#ifdef _MSC_VER
#include <intrin.h>
#else
#pragma GCC target ("aes,sse4.1")
#include <x86intrin.h>
#endif

extern "C" void s_memzero32(void* p);
extern "C" void generate_random_bytes(uint8_t* data, size_t len);

inline char hf_bin2hex(unsigned char c)
{
	if (c <= 0x9)
		return '0' + c;
	else
		return 'a' - 0xa + c;
}

inline void bin2hex(const unsigned char* in, unsigned int len, char* out)
{
	for (unsigned int i = 0; i < len; i++)
	{
		out[i * 2] = hf_bin2hex((in[i] & 0xF0) >> 4);
		out[i * 2 + 1] = hf_bin2hex(in[i] & 0x0F);
	}
}

/*
   Varint encoding / decoding is based on the public domain code
   by Lasse Collin <lasse.collin@tukaani.org> and Igor Pavlov.
   Original source - The .xz File Format
*/

size_t varint_encode(uint8_t* buf, uint64_t num)
{
    if (num > UINT64_MAX / 2)
        return 0;

    size_t i = 0;

    while (num >= 0x80) {
        buf[i++] = (uint8_t)(num) | 0x80;
        num >>= 7;
    }

    buf[i++] = (uint8_t)(num);

    return i;
}

struct v32
{
	v32() {}
	~v32() { s_memzero32(data); }

	v32(const uint8_t* src) { memcpy(data, src, 32); }
	v32(const v32& o)
	{
		__m128i r1 = _mm_load_si128((const __m128i*)o.data);
		__m128i r2 = _mm_load_si128((const __m128i*)(o.data+16));
		_mm_store_si128((__m128i*)data, r1);
		_mm_store_si128((__m128i*)(data+16), r2);
	}

	v32& operator= (const v32& o)
	{
		__m128i r1 = _mm_load_si128((const __m128i*)o.data);
		__m128i r2 = _mm_load_si128((const __m128i*)(o.data+16));
		_mm_store_si128((__m128i*)data, r1);
		_mm_store_si128((__m128i*)(data+16), r2);
		return *this;
	}

	v32& operator= (const uint8_t* src)
	{
		memcpy(data, src, v32::size);
		return *this;
	}

	bool operator==(const v32& o) const
	{
		__m128i r;
		r = _mm_xor_si128(_mm_load_si128((const __m128i*)data),
			_mm_load_si128((const __m128i*)o.data));

		if(_mm_testz_si128(r, r) == 0)
			return false;

		r = _mm_xor_si128(_mm_load_si128((const __m128i*)(data + 16)),
			_mm_load_si128((const __m128i*)(o.data + 16)));

		return _mm_testz_si128(r, r) == 1;
	}

	void to_string(char* out) const { bin2hex(data, 32, out); out[64] = '\0'; }

	bool operator!=(const v32& o) const { return !(*this == o); }

	operator const uint8_t*() const { return data; }
	operator uint8_t*() { return data; }

	static constexpr size_t size = 32;
	alignas(16) uint8_t data[32];
};

typedef v32 kchash;
typedef v32 eckey;
typedef v32 pubkey;
typedef v32 prvkey;
