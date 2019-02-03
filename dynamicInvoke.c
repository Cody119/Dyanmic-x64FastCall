#include <inttypes.h>
#include "dynamicInvoke.h"

int64_t x64fastcall_invoke(void *func, int count, void *args) {

	int size;
	
	// Allocate space on the stack for the arguments
	// Fast call requires that 4 bytes be allocated per argument
	// with a minimum of 32bytes.
	// This includes the first 4 arguments, even though they
	// arnt passed on the stack
	if (count > 4) {
		size = 8*count;
		void *stack;
		//Increase the stack and get a pointer to the TOS
		asm volatile (
			"subq	%1, %%rsp\n\t"
			"mov %%rsp, %0\n\t"
			: "=rm" (stack) : "rm" (size) :
		);
		// copy all the arguments (except for the first 4)
		// to the stack
		int i = 4;
		for (;i < count; i++) {
			((int64_t*)stack)[i] = ((int64_t*)args)[i];
		}
	} else {
		// If there are less then four arguments, still need to
		// allocate 32bytes on the stack
		size = 32;
		asm volatile ("subq	$32, %%rsp\n\t":::);
	}


	// Fast call treats the first 4 arguments speacily
	switch (count) {
		case_4:
		case 4:
			asm volatile ( 
				"mov %0, %%r9" 
				:: "rm" (((int64_t*)args)[3]):
			);
		case 3:
			asm volatile ( 
				"mov %0, %%r8" 
				:: "rm" (((int64_t*)args)[2]):
			);
		case 2:
			asm volatile ( 
				"mov %0, %%rdx" 
				:: "rm" (((int64_t*)args)[1]):
			);
		case 1:
			asm volatile (
				"mov %0, %%rcx" 
				:: "rm" (((int64_t*)args)[0]):
			);
		case 0:
			break;
		default:
			goto case_4;
	}

	// Call the function and clean up the stack.
	// The return value is currently stored in rax as
	// per convention, so we dont need to move it as it
	// is also the return value of this function
	count = count < 4 ? 32 : count*8;
	asm volatile (
		"mov %[func], %%rax\n\t"
		"call *%%rax\n\t"
		"nop\n\t"
		"addq %[size], %%rsp\n\t"
		:: [func] "rm" (func), [size] "rm" (count) :
	);
}