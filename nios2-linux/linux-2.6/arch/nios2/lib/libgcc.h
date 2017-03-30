#ifndef _ASM_NIOS2_LIBGCC_H
#define _ASM_NIOS2_LIBGCC_H

typedef int word_type __attribute__((mode(__word__)));

struct DWstruct {
	long low, high;
};

typedef union {
	struct DWstruct s;
	long long ll;
} DWunion;

typedef unsigned int UWtype;
typedef long long DWtype;
typedef unsigned short UHWtype;

#define W_TYPE_SIZE 32

#define __ll_B ((UWtype) 1 << (W_TYPE_SIZE / 2))
#define __ll_lowpart(t) ((UWtype) (t) & (__ll_B - 1))
#define __ll_highpart(t) ((UWtype) (t) >> (W_TYPE_SIZE / 2))

#if !defined (umul_ppmm)
# define umul_ppmm(w1, w0, u, v)					\
	do {								\
		UWtype __x0, __x1, __x2, __x3;				\
		UHWtype __ul, __vl, __uh, __vh;				\
									\
		__ul = __ll_lowpart(u);					\
		__uh = __ll_highpart(u);				\
		__vl = __ll_lowpart(v);					\
		__vh = __ll_highpart(v);				\
									\
		__x0 = (UWtype) __ul * __vl;				\
		__x1 = (UWtype) __ul * __vh;				\
		__x2 = (UWtype) __uh * __vl;				\
		__x3 = (UWtype) __uh * __vh;				\
									\
		__x1 += __ll_highpart(__x0);	/* this can't give carry */		\
		__x1 += __x2;			/* but this indeed can */		\
		if (__x1 < __x2)		/* did we get it? */			\
			__x3 += __ll_B;		/* yes, add it in the proper pos. */	\
									\
		(w1) = __x3 + __ll_highpart(__x1);			\
		(w0) = __ll_lowpart(__x1) * __ll_B + __ll_lowpart(__x0);\
	} while (0)
#endif

#if !defined(__umulsidi3)
# define __umulsidi3(u, v) ({			\
	DWunion __w;				\
	umul_ppmm(__w.s.high, __w.s.low, u, v);	\
	__w.ll; })
#endif

#endif /* _ASM_NIOS2_LIBGCC_H */
