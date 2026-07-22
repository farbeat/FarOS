#ifndef _HW_CDEFS_H_
#define _HW_CDEFS_H_

#if defined(__cplusplus)
#define __BEGIN_DECLS extern "C" {
#define __END_DECLS }
#else
#define __BEGIN_DECLS
#define __END_DECLS
#endif

#define __packed __attribute__((__packed__))
#define __aligned(x) __attribute__((__aligned__(x)))

#define __LIBC_HIDDEN__ __attribute__((visibility("hidden")))

#define __FBSDID(_s)

#define __min_size(x) static (x)

#endif
