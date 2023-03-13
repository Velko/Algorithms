#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define UPPER_LIMIT     16777216
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


void segmented_sieve(void) {

    prepare_segment_0();

    // start at _segment_1_ and advance by the segment size
    for (size_t seg_start = 1 * SEGMENT_SIZE; seg_start < UPPER_LIMIT; seg_start += SEGMENT_SIZE) {
        filter_segment_n(seg_start);
    }
}

int main() {
    segmented_sieve();

    return 0;
}