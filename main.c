#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "dynamicInvoke.h"

void printTest(int64_t t, int64_t c, int64_t d, int64_t f, int64_t g, int64_t x, int64_t y) {
	printf("%"PRIx64" %"PRIx64" %"PRIx64" %"PRIx64" %"PRIx64" %"PRIx64" %"PRIx64"\n", t, c, d, f, g, x, y);
}

int add(int x, int y) {
	printf("%d\n", x + y);
	return x + y;
}

int main(int argc, char const *argv[]){
	//Test 1
	int64_t x[] = {0xFFAABBCCCCBBAAFF, 0xCCBBAAFFFFAABBCC, 0xCCBBAAFF, 4, 6, 0xFFAABBCCCCBBAAFF, 0xCCBBAAFFFFAABBCC};
	x64fastcall_invoke(&printTest, 7, x);
	
	//Test 2
	int64_t y[] = {10, 16};
	int64_t w = x64fastcall_invoke(&add, 2, y);
	printf("%"PRId64"\n", w);
	return 0;
}