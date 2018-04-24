#include <stdlib.h>
#include <stdio.h>

void *memtrack_malloc(size_t size, const char* func) {
	void *ptr = malloc(size);
	printf("Allocated %d bytes in [%s] -> %p\n", size, func, ptr);
	return ptr;
}

void memtrack_free(void* ptr, const char *func) {
	printf("Freed %p through [%s]\n", ptr, func);
	free(ptr);
}