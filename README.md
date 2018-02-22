# Optimised 64-bit Cryptonote elliptic curve library

## What is this?

Elliptic curve cryptography is the "guts" of a cryptonote coin. This particular version is the first to be easliy portable across operating systems. The monero project has been trying to come up with something like it for the last 6 months [https://github.com/monero-project/monero/pull/2317] (Note for beginners, most of what vtnerd says there is wrong).

The code is fairly plug-and-play and will speed up any coin by a factor of just below x3. Much more significant speed-ups are possible but I will keep that code private for now.

## What's the licence? What is "All rights reserved."?

There is no licence. All rights reserved means that you can only look. Please [https://www.reddit.com/user/fireice_uk/](contact me) if you want to use the code. 

## Is this code constant time? What is constant time?

Dr. Bernstein has put significant effornt into making the code run in constant time. This arguably improves security in use cases where CPU is shared across many users (VMs in particular). However, the recent [https://en.wikipedia.org/wiki/Spectre_(security_vulnerability)](SPECTRE) vulnerability has rendered that into a moot feature unfortunately.

This code was written before the vulnerability was discovered and runs in constant time.

## What else is possible?

1. Even faster x5 - x10 EC crypto based on new features in Broadwell+ and Ryzen CPUs
2. 1 + Bulletproofs + 4 to enable truly anonymous ring signatures (sizes 100+), instead of plausible deniability (size 5)
3. Cryptonote blockchain compression, down into below 1 GB sizes for Monero
4. Combination of 1 and 3 and a specially tuned server to create Electrum-like wallets.
5. Proper mathematical solution to Attack III described in [http://www.comp.nus.edu.sg/~shruti90/papers/monero-analysis.pdf](here).
6. Tracking of Monero using Attack III (their fix is fairly sub-par especially if some assumptions can be made about the wallet) and graph analysis.


If anything on this list sounds like something you would like to have in your coin [https://www.reddit.com/user/fireice_uk/](contact me)
