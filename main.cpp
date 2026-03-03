
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
		char* p = new char; 
		ESCAPE(p);
		delete p;
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
	printf("new + delete | ns/op: %.2f | total_ops: %llu\n",
			ns, (unsigned long long)total);
	return 0;
}
