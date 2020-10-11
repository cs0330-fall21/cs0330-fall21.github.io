#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

#define ASIZE (4096*1024)

void minmax1(long *a, long *b, long n) {
  long i;
  for (i=0; i<n; i++) {
    if (a[i] > b[i]) {
      long t = a[i];
      a[i] = b[i];
      b[i] = t;
    }
  }
}



long Random1[ASIZE];
long Random2[ASIZE];
long Small1[ASIZE];
long Small2[ASIZE];
long Large[ASIZE];

void init() {
	long i;

	for(i=0; i<ASIZE; i++) {
		Random1[i] = rand();
		Random2[i] = rand();
		long x = rand();
		Small1[i] = x;
		Small2[i] = x;
		Large[i] = x+1;
	}
}

void printtime(char annotation[], struct timespec *start, struct timespec *end) {
	long secs, nsecs;
	nsecs = end->tv_nsec - start->tv_nsec;
	secs = end->tv_sec - start->tv_sec;
	if (nsecs < 0) {
		nsecs += 1000000000L;
		secs -= 1L;
	}
	double TimePerIter = (double)secs * 1000000000E0 + (double)nsecs/(double)ASIZE;
	printf("%s: %.3f nsecs\n", annotation, TimePerIter);
}

int main() {
	struct timespec sts, ets;

	init();

	if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &sts) == -1) {
		perror("clock_gettime");
		exit(1);
	}

	minmax1(Random1, Random2, ASIZE);
	if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ets) == -1) {
		perror("clock_gettime");
		exit(1);
	}

	printtime("minmax1 random", &sts, &ets);
	
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &sts);
	minmax1(Small1, Large, ASIZE);
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ets);
	printtime("minmax1 small/large", &sts, &ets);

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &sts);
	minmax1(Large, Small2, ASIZE);
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ets);
	printtime("minmax1 large/small", &sts, &ets);


	return 0;
}