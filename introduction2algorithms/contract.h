/*
 * tree.c
 *
 *  Created on: 2014年4月1日
 *      Author: Coeus
 */

#ifndef CONTRACT_H_
#define CONTRACT_H_

#define CPU_BITS sizeof(void*)
#define BITS32 4
#define BITS64 8

typedef unsigned long ulong;
typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;

#ifndef NIL
#ifndef __cplusplus
#define NIL ((void*)0)
#else
#define NIL 0
#endif
#endif

#ifndef INT_MAX
#define INT_MAX 0x7FFFFFFF
#endif
#ifndef INT_MIN
#define INT_MIN 0x80000000
#endif

#ifndef __cplusplus
#ifndef bool
#define bool unsigned char
#define true '\1'
#define false '\0'
#endif
#endif

#endif /* CONTRACT_H_ */
