#include <inttypes.h>



//typedef long long int64_t;

void x64fastcall_invoke(funcT func, int count, void *args) {
	//(*func)(1,2,3,4);
	int x;
/*
	asm volatile (
		
		"movq $1, %%rcx\n\t"
		"movq $2, %%rdx\n\t"
		"movq $3, %%r8\n\t"
		"movq $4, %%r9\n\t"
		"subq	$32, %%rsp\n\t"
		"movq %1, %%rax\n\t"
		"call *%%rax\n\t"
		"addq	$32, %%rsp\n\t"
		"movl %%eax, %0"
		: "=rm" (x) : "rm" (func):
	);
*/
	int size;
	if (count > 4) {
		size = 8*count;
		void *stack;
		asm volatile (
			"subq	%1, %%rsp\n\t"
			"movq %%rsp, %0\n\t"
			: "=rm" (stack) : "rm" (size) :
		);
		int i = 4;
		for (;i < count; i++) {
			((int64_t*)stack)[i] = ((int64_t*)args)[i];
		}
	} else {
		size = 32;
		asm volatile ("subq	$32, %%rsp\n\t":::);
	}

	switch (count) {
CASE:	case 4:
			asm volatile ( "movq %0, %%r9" :: "rm" (((int64_t*)args)[3]):);
		case 3:
			asm volatile ( "movq %0, %%r8" :: "rm" (((int64_t*)args)[2]):);
		case 2:
			asm volatile ( "movq %0, %%rdx" :: "rm" (((int64_t*)args)[1]):);
		case 1:
			asm volatile ( "movq %0, %%rcx" :: "rm" (((int64_t*)args)[0]):);
		case 0:
			break;
		default:
			goto CASE;
	}

	count = count < 4 ? 32 : count*8;
	//printf("%d %d\n", count, size);
	asm volatile (
		"movq %[func], %%rax\n\t"
		"call *%%rax\n\t"
		"nop\n\t"
		"addq %[size], %%rsp\n\t"
		//"subq %[size], %%rsp\n\t"
		//"addq $32, %%rsp\n\t"
		//"movq %[size], %%rax\n\t"
		//"movq %%rax, %[x]\n\t"
		"movl %%eax, %[ret]"
		: [ret] "=rm" (x) : [func] "rm" (func), [size] "rm" (count)/*, [x] "rm" (x)*/:
	);

	//return x;
}