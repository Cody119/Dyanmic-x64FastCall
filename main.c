#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>

typedef int64_t llT;

typedef void (*funcT)(llT, llT, llT, llT, llT, llT, llT);

void printTest(llT t, llT c, llT d, llT f, llT g, llT x, llT y) {
	printf("%"PRId64" %"PRId64" %"PRId64" %"PRId64" %"PRId64" %"PRId64" %"PRId64"\n", t, c, d, f, g, x, y);
}

int main(int argc, char const *argv[]){
	llT x[] = {1, 2, 3, 4, 6, 7, 8};
	x64fastcall_invoke(&test, 7, x);
	return 0;
}