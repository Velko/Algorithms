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

int main(void) {
    simple_sieve();
    return 0;
}