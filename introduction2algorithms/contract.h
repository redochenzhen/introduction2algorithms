/*
 * tree.c
 *
 *  Created on: 2014年4月1日
 *      Author: Coeus
 */

#ifndef CONTRACT_H_
#define CONTRACT_H_

#ifdef _WIN32
#define NIL 0
#else
#define NIL ((void*)0)
#ifndef INT_MAX
#define INT_MAX 0x7FFFFFFF
#endif
#ifndef INT_MIN
#define INT_MIN 0x80000000
#endif
#define VOID_INT_MAX ((void*)INT_MAX)
#define VOID_INT_MIN ((void*)INT_MIN)

typedef enum{FALSE=0,TRUE=1}BOOL;
#endif

#endif /* CONTRACT_H_ */