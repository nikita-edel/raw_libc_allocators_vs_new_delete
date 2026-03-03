#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

static double now_sec(void) {
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9;
}

#define FENCE()      __asm__ volatile("" ::: "memory")
#define ESCAPE(p)    __asm__ volatile("" : : "r,m"(p) : "memory")

static uint64_t bench(uint64_t n) {
	for (uint64_t i = 0; i < n; ++i) {
		char* p = malloc(1);
		//as far as i know, new throws bad_alloc. but i compile the cpp file
		//with -fno-exceptions, so it should be aquivalent to this:
		//(calling std::terminate) but im pretty sure that is 1:1 aquivalent
		//the cpu can just very easily predict the branches? maybe not why would
		//it be any difference than a new call that has a branch...?
		if(!p) abort();
		ESCAPE(p);
		free(p);
		FENCE();
	}
	return n;
}

int main(void) {
	const double maxSec = 3.0;
	uint64_t batch = 8, total = 0;
	double elapsed = 0.0;
	double t0 = now_sec();

	while (elapsed < maxSec) {
		total  += bench(batch);
		elapsed = now_sec() - t0;
		double npo  = total ? elapsed * 1e9 / (double)total : 1.0;
		uint64_t want = (uint64_t)(5e6 / (npo > 0.0 ? npo : 1.0));
		if (want < 8)        want = 8;
		if (want > (1u<<20)) want = 1u<<20;
		batch = want;
	}

	double ns = total ? elapsed * 1e9 / (double)total : 0.0;
	printf("malloc + free | ns/op: %.2f | total_ops: %llu\n",
			ns, (unsigned long long)total);
	return 0;
}
