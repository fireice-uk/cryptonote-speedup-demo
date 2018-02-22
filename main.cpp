/* 
 * (c) 2018 fireice-uk
 * All rights reserved.
 * 
 */

#include <iostream>
#include <chrono>
#include "ecops32/crypto-ops.h"
#include "ecops64/ecops64.h"
#include "crypt-aux/keccak.h"
#include "vector32.h"

template<size_t n>
void mock_check_outs32(const v32& tx_r, const v32& pub_spend, const v32& prv_view, v32 out_p[n])
{
	ge_p3 sR, sB;
	if(ge_frombytes_vartime(&sR, tx_r) != 0 || ge_frombytes_vartime(&sB, pub_spend) != 0)
		throw std::runtime_error("Invalid blockchain input");

	ge_p1p1 p1p;
	ge_p2 p2p;
	ge_scalarmult(&p2p, prv_view, &sR);
	ge_mul8(&p1p, &p2p);
	ge_p1p1_to_p2(&p2p, &p1p);

	v32 data;
	ge_tobytes(data, &p2p);

	kchash hval;
	//bool has_outs = false;
	for(uint64_t idx=0; idx < n; idx++)
	{
		uint8_t vint[9];
		keccak_ctx ctx;

		size_t l = varint_encode(vint, idx);

		keccak_init(&ctx);
		keccak_update(&ctx, data, v32::size);
		keccak_update(&ctx, vint, l);
		keccak_final(&ctx, hval);

		sc_reduce32(hval);

		ge_p3 p2;
		ge_cached p3;

		eckey calcP;
		ge_scalarmult_base(&p2, hval);
		ge_p3_to_cached(&p3, &p2);
		ge_add(&p1p, &sB, &p3);
		ge_p1p1_to_p2(&p2p, &p1p);
		ge_tobytes(calcP, &p2p);

		out_p[idx] = calcP;
		/*if(calcP == txp)
			has_outs = true;*/
	}

	//return has_outs;
}

template<size_t n>
bool mock_check_outs64(const v32& tx_r, const v32& pub_spend, const v32& prv_view, v32 out_p[n])
{
	ge64_p3 sR, sB;
	ge64_p1p1 p1p;
	ge64_p2 p2p;
	v32 data, P;
	kchash hash;
	
	if(ge64_frombytes_vartime(&sR, tx_r) != 0 || ge64_frombytes_vartime(&sB, pub_spend) != 0)
		throw std::runtime_error("Invalid blockchain input");

	ge64_scalarmult(&p2p, prv_view, &sR);
	ge64_mul8(&p1p, &p2p);
	ge64_p1p1_to_p2(&p2p, &p1p);
	ge64_tobytes(data, &p2p);

	//bool has_outs = false;
	for(uint64_t idx=0; idx < n; idx++)
	{
		uint8_t vint[9];
		keccak_ctx ctx;
		ge64_p3 p3;

		size_t l = varint_encode(vint, idx);
		keccak_init(&ctx);
		keccak_update(&ctx, data, v32::size);
		keccak_update(&ctx, vint, l);
		keccak_final(&ctx, hash);

		sc64_reduce32(hash);

		ge64_scalarmult_base(&p3, hash);
		ge64_add(&p1p, &sB, &p3);
		ge64_p1p1_to_p2(&p2p, &p1p);
		ge64_tobytes(P, &p2p);

		out_p[idx] = P;
		/*if(P == txp)
			has_outs = true;*/
	}

	//return has_outs;
}

void make_rand_pub(v32& key)
{
	v32 rnd;
	ge_p3 point;
	
	generate_random_bytes(rnd, 32);
	sc_reduce32(rnd);
	ge_scalarmult_base(&point, rnd);
	ge_p3_tobytes(key, &point);
}

int main(int argc, char **argv)
{
	constexpr size_t test_transactions = 50000;
	constexpr size_t test_outs_per_tx = 4;
	
	v32* tx_r = new v32[test_transactions];
	v32* pub_spend = new v32[test_transactions];
	v32* prv_view = new v32[test_transactions];
	v32* output_ps_32 = new v32[test_transactions * test_outs_per_tx];
	v32* output_ps_64 = new v32[test_transactions * test_outs_per_tx];
	
	std::cout << "Generating random test data." << std::endl;

	for(size_t i=0; i < test_transactions; i++)
	{
		generate_random_bytes(prv_view[i], 32);
		sc_reduce32(prv_view[i]);
		make_rand_pub(tx_r[i]);
		make_rand_pub(pub_spend[i]);
	}

	std::cout << "Testing 32 bit code." << std::endl;
	
	auto start = std::chrono::steady_clock::now();
	for(size_t i=0; i < test_transactions; i++)
	{
		mock_check_outs32<test_outs_per_tx>(tx_r[i], pub_spend[i], prv_view[i], output_ps_32 + i * test_outs_per_tx);
	}
	auto delta_time = std::chrono::steady_clock::now() - start;
	
	std::cout << "32-bit code " << std::chrono::duration <double, std::milli> (delta_time).count() << " ms" << std::endl;
	
	std::cout << "Testing 64 bit code." << std::endl;

	start = std::chrono::steady_clock::now();
	for(size_t i=0; i < test_transactions; i++)
	{
		mock_check_outs64<test_outs_per_tx>(tx_r[i], pub_spend[i], prv_view[i], output_ps_64 + i * test_outs_per_tx);
	}
	delta_time = std::chrono::steady_clock::now() - start;
	
	std::cout << "64-bit code " << std::chrono::duration <double, std::milli> (delta_time).count() << " ms" << std::endl;
	
	std::cout << "Verifying test data." << std::endl;
	
	for(size_t i=0; i < test_transactions * test_outs_per_tx; i++)
	{
		if(output_ps_32[i] != output_ps_64[i])
		{
			std::cout << "Verification failed!" << std::endl;
			return 0;
		}
	}

	std::cout << "Verification OK!" << std::endl;
    	return 0;
}
