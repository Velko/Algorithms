Sieve of Eratosthenes
=====================

Sieve of Eratosthenes is an algorithm for finding prime numbers. On top on the classic one, there are few modernizations.


Simple sieve
------------

The simplest version of the algorithm is quite small:

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define UPPER_LIMIT     16777216

void simple_sieve(void) {

    // allocate huge array
    bool *candidates = malloc(sizeof(bool) * UPPER_LIMIT);

    // initialize (almost) everything to "possible prime"
    candidates[0] = candidates[1] = false;
    for (size_t i = 2; i < UPPER_LIMIT; ++i)
        candidates[i] = true;

    // consider every number
    for (size_t p = 2; p < UPPER_LIMIT; ++p) {
        if (!candidates[p]) continue; // already marked as non-prime

        printf("%d\n", p); // found one

        // mark all the multiples of it as a non-prime
        for (size_t m = p * p; m < UPPER_LIMIT; m += p)
            candidates[m] = false;
    }

    // remember to release the memory ;-)
    free(candidates);
}
```

It is also reasonably fast, but the downside is its requirement of large memory area. The sample above calculates primes up to 16+ million
and requires a byte for each number considered, was it a prime or not. To be frank, nowadays 16 MiB of memory consumption does not seem
that much, but still is wasteful.


Segmented sieve
---------------

To address the issue of the large memory consumption, there's a variant that only keeps parts of the required array in memory.

The whole range is split into segments, each being a square root of the total size. Instead of allocating memory for the whole 16+ million,
we can get away with segments of just 4096.

The algorithm begins with calculating a _segment 0_. The segment is small enough for us to use regular _Simple sieve_. There's only a small
difference, that instead of booleans, we're using integers to keep _largest encountered multiple_ of the prime.

So far, almost the same:
```c
#define SEGMENT_SIZE    4096

size_t segment_0[SEGMENT_SIZE];

void prepare_segment_0(void) {

    // initialize (almost) everything to "possible prime"
    segment_0[0] = segment_0[1] = 0;
    for (size_t i = 2; i < SEGMENT_SIZE; ++i)
        segment_0[i] = i;

    // consider every number
    for (size_t p = 2; p < SEGMENT_SIZE; ++p) {
        if (segment_0[p] == 0) continue; // already marked as non-prime

        printf("%d\n", p); // found one

        // mark all the multiples of it as a non-prime
        size_t m = p * p;
        for (; m < SEGMENT_SIZE; m += p)
            segment_0[m] = 0;

        // m contains next multiple of prime, that does not fit in segment_0
        // and must belong to a next one. It will become useful later
        segment_0[p] = m;
    }
}
```

Also note, that because of the smaller memory requirements, we do not need to allocate memory dynamically anymore.

After that we should continue with calculating next segments, up to the required limit. We're beginning with the
segment just after the _segment 0_, and advancing to the next ones by same size.

```c
#define UPPER_LIMIT     16777216

void segmented_sieve(void) {

    prepare_segment_0();

    // start at _segment 1_ and advance by the segment size
    for (size_t seg_start = 1 * SEGMENT_SIZE; seg_start < UPPER_LIMIT; seg_start += SEGMENT_SIZE) {
        filter_segment_n(seg_start);
    }
}
```

The real "magic", however happens inside the `filter_segment_n()`:

```c
bool segment_n[SEGMENT_SIZE];

void filter_segment_n(size_t seg_start) {

    // mark to "possible prime"
    for (size_t i = 0; i < SEGMENT_SIZE; ++i)
        segment_n[i] = true;

    // look up the known prime/multiple in _segment_0_
    for (size_t p = 2; p < SEGMENT_SIZE; ++p) {
        if (segment_0[p] == 0) continue;

        // convert it to the index in *this* segment
        size_t m_index = segment_0[p] - seg_start;

        // mark all the multiples again
        for (; m_index < SEGMENT_SIZE; m_index += p)
            segment_n[m_index] = false;

        // and store it back for the next segment(s)
        segment_0[p] = m_index + seg_start;
    }

    // and finally, print out the primes
    for (size_t i = 0; i < SEGMENT_SIZE; ++i)
    {
        if (segment_n[i]) {
            size_t prime = i + seg_start;
            printf("%d\n", prime);
        }
    }
}
```

Here we're back to the bool array again, just a smaller one.



Incremental sieve
-----------------

