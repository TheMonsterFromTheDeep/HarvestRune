#ifndef MEMTRACK_H_
#define MEMTRACK_H_

#define STR2(x) #x
#define STR1(x) STR2(x)

extern void *memtrack_malloc(size_t size, const char* func);
extern void memtrack_free(void* ptr, const char *func);

//#define MEMTRACK

#ifdef MEMTRACK
	#define malloc(x) memtrack_malloc(x, __func__)
	#define free(x) memtrack_free(x, __func__)
#endif

#endif