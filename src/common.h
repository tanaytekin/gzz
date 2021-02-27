#ifndef GZZ_COMMON_H
#define GZZ_COMMON_H

#include <stdint.h>
typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef uint64_t  u64;

typedef int8_t    i8;
typedef int16_t   i16;
typedef int32_t   i32;
typedef int64_t   i64;

typedef float     f32;
typedef double    f64;

typedef uint32_t b32;
#define GZZ_TRUE 1
#define GZZ_FALSE 0


#include <stdlib.h>
#define GZZ_MALLOC(size) malloc(size)
#define GZZ_FREE(ptr) free(ptr)

#endif //GZZ_COMMON_H
