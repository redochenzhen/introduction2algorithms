/*
 * hash_func.h
 *
 *  Created on: 2014年4月20日
 *      Author: Coeus
 */

#ifndef HASH_FUNC_H_
#define HASH_FUNC_H_

#include "../contract.h"
#include <stdint.h>
#define PHI_16 40503
#define PHI_32 2654435769
#define PHI_64 11400714819323198485UL

ulong fibonacci_hash(ulong key,ushort bits);
ulong shift_hash(ulong keys);
uint32_t bob_jenkins_32(uint32_t a);
uint32_t bob_jenkins_96(uint32_t a,uint32_t b,uint32_t c);
uint crc32_hash(const void *key,uint len);

#endif /* HASH_FUNC_H_ */
