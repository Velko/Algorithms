Sieve of Eratosthenes
=====================

Sieve of Eratosthenes is an algorithm for finding prime numbers. On top on the classic one, there are few modernizations.


Simple sieve
------------

The original version of the algorithm allocates a big array. Starting from smallest prime number (2) mark every multiple of
it as non-prime. Advance to the next prime, mark every multiple of that. Continue until the end of the array. Numbers left in
the array are primes.

[Example implementation in C](simple.md)


Segmented sieve
---------------

To address the issue of the large memory consumption, there's a variant that only keeps parts of the required array in memory.

The whole range is split into segments, each being a square root of the total size. With exception of the first one, only one
segment has to be kept in memory while is being processed.

[Example implementation in C](segmented.md)

Incremental sieve
-----------------

