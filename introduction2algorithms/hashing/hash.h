/*
 * hash.h
 *
 *  Created on: 2014年4月20日
 *      Author: Coeus
 */

#ifndef HASH_H_
#define HASH_H_

#include "../contract.h"

#define MOD_POW2(x,y) ((x)&((y)-1))	//要求y是2的幂
#define DEFAULT_TBL_SIZE 0x1000		//要求是2的幂
#define DEFAULT_LOAD_FACTOR 1
#define LOAD_FACTOR(h) ((float)(h->count)/(h->size))

#define DEFAULT_SEG_SIZE 16
#define DEFAULT_DIR_SIZE 16
#define D_LOAD_FACTOR(dh) ((float)(dh->count)/(dh->slot_max+1))

typedef enum{HASH_CUSTOM,HASH_INTEGER,HASH_STRING}hash_type_t;
typedef void *elem_t;
typedef const void *_key_t;
typedef struct _slot *slot_pt;
typedef ulong (*hash_func_t)(_key_t);
typedef bool (*equal_func_t)(_key_t,_key_t);

typedef struct _search_info{
	ulong hashcode;
	ulong loc;
	ulong seg_idx;
	slot_pt slot;
	slot_pt prior;
}search_info_t;

typedef struct _slot{
	ulong hashcode;
	_key_t key;
	elem_t satellite;
	slot_pt next;
}slot_t;

typedef struct _hash_tbl{
	slot_pt *slot_arr;
	ulong size;
	ulong count;
	hash_func_t hash;
	equal_func_t is_equal;
#ifdef Debug
	ulong *SLOT_DEEP_ARR;
#endif
}hash_tbl_t,*hash_tbl_pt;

typedef const hash_tbl_t *hash_tbl_cpt;

slot_pt new_slot(_key_t key,elem_t satellite,hash_func_t hash_func);
hash_tbl_pt new_hash_tbl(hash_type_t type,hash_func_t hash_func,equal_func_t equal_func);
bool hash_insert(hash_tbl_pt table,_key_t key,elem_t satellite);
bool hash_exist(hash_tbl_cpt table,_key_t key);
elem_t hash_search(hash_tbl_cpt table,_key_t key);
elem_t hash_delete(hash_tbl_pt table,_key_t key);
void free_hash_tbl(hash_tbl_pt table);

typedef struct _slot_segment{
	slot_pt slot_arr[DEFAULT_SEG_SIZE];
}slot_segment_t,*slot_segment_pt;


typedef struct _dhash_tbl{
	//directory_t *dir;
	slot_segment_pt *seg_arr;
	uint seg_shift;
	ulong dir_size;
	ulong count;
	ulong slot_max;	//当前可用的最大槽号
	ulong low_mask;
	ulong high_mask;
	hash_func_t hash;
	equal_func_t is_equal;
}dhash_tbl_t,*dhash_tbl_pt;

typedef const dhash_tbl_t *dhash_tbl_cpt;

dhash_tbl_pt new_dhash_tbl(hash_type_t type,hash_func_t hash_func,equal_func_t equal_func);
bool dhash_insert(dhash_tbl_pt table,_key_t key,elem_t satellite);
bool dhash_exist(dhash_tbl_pt table,_key_t key);
elem_t dhash_search(dhash_tbl_pt table,_key_t key);
elem_t dhash_delete(dhash_tbl_pt table,_key_t key);

ulong inline integer_hash(_key_t key);
bool inline integer_equal(_key_t x,_key_t y);
ulong inline string_hash(_key_t key);
bool inline string_equal(_key_t x,_key_t y);
#endif /* HASH_H_ */



















