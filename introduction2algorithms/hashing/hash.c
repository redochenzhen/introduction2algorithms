/*
 * hash.c
 *
 *  Created on: 2014年4月20日
 *      Author: Coeus
 */

#include "hash_func.h"
#include "hash.h"
#include <string.h>
#include <stdlib.h>

static void search_slot(hash_tbl_cpt table,_key_t key,search_info_t *info);

slot_pt new_slot(_key_t key,elem_t satellite,hash_func_t hash_func){
	slot_pt slot=(slot_pt)malloc(sizeof(slot_t));
	slot->hashcode=hash_func(key);
	slot->key=key;
	slot->satellite=satellite;
	slot->next=NIL;
	return slot;
}

slot_pt new_slot_no_calculate(_key_t key,ulong hashcode,elem_t satellite){
	slot_pt slot=(slot_pt)malloc(sizeof(slot_t));
	slot->hashcode=hashcode;
	slot->key=key;
	slot->satellite=satellite;
	slot->next=NIL;
	return slot;
}

hash_tbl_pt new_hash_tbl(hash_type_t type,hash_func_t hash_func,equal_func_t equal_func){
	hash_tbl_pt table=(hash_tbl_pt)malloc(sizeof(hash_tbl_t));
	table->size=DEFAULT_TBL_SIZE;
	table->count=0;
	table->slot_arr=(slot_pt*)malloc(sizeof(void*)*DEFAULT_TBL_SIZE);
	memset(table->slot_arr,0,sizeof(void*)*DEFAULT_TBL_SIZE);
	switch(type){
	case HASH_CUSTOM:
		table->hash=hash_func;
		table->is_equal=equal_func;
		break;
	case HASH_STRING:
		table->hash=string_hash;
		table->is_equal=string_equal;
		break;
	case HASH_INTEGER:
	default:
		table->hash=integer_hash;
		table->is_equal=integer_equal;
		break;
	}
#ifdef Debug
	table->SLOT_DEEP_ARR=(ulong*)malloc(sizeof(ulong)*table->size);
	memset(table->SLOT_DEEP_ARR,0,sizeof(ulong)*table->size);
#endif
	return table;
}

bool hash_insert(hash_tbl_pt table,_key_t key,elem_t satellite){
	search_info_t info;
	search_slot(table,key,&info);
	if(info.slot!=NIL){
		return false;
	}
	slot_pt newslot=new_slot_no_calculate(key,info.hashcode,satellite);
	slot_pt *slot_arr=table->slot_arr;
	newslot->next=slot_arr[info.loc];
	slot_arr[info.loc]=newslot;
	table->count++;
#ifdef Debug
	table->SLOT_DEEP_ARR[info.loc]++;
#endif
	return true;
}

bool inline hash_exist(hash_tbl_cpt table,_key_t key){
	search_info_t info;
	search_slot(table,key,&info);
	return info.slot!=NIL;
}

elem_t inline hash_search(hash_tbl_cpt table,_key_t key){
	search_info_t info;
	search_slot(table,key,&info);
	return info.slot==NIL?NIL:info.slot->satellite;
}

elem_t hash_delete(hash_tbl_pt table,_key_t key){
	elem_t elem=NIL;
	search_info_t info;
	search_slot(table,key,&info);
	if(info.slot!=NIL){
		elem=info.slot->satellite;
		if(info.prior==NIL){
			table->slot_arr[info.loc]=info.slot->next;
		}else{
			info.prior->next=info.slot->next;
		}
		free(info.slot);
		table->count--;
	}
	return elem;
}

void free_hash_tbl(hash_tbl_pt table){
	ulong loc=0;
	slot_pt slot,next;
	for(;loc<table->size;++loc){
		slot=table->slot_arr[loc];
		while(slot!=NIL){
			next=slot->next;
			free(slot);
			slot=next;
		}
	}
	free(table->slot_arr);
#ifdef Debug
	free(table->SLOT_DEEP_ARR);
#endif
	free(table);
}

void search_slot(hash_tbl_cpt table,_key_t key,search_info_t *info){
	ulong hashcode=table->hash(key);
	ulong loc=MOD_POW2(hashcode,table->size);
	slot_pt* slot_arr=table->slot_arr;
	slot_pt slot;
	slot_pt prior=NIL;
	slot=slot_arr[loc];
	while(slot!=NIL){
		if(hashcode==slot->hashcode&&table->is_equal(key,slot->key)){
			break;
		}else{
			prior=slot;
			slot=slot->next;
		}
	}
	info->hashcode=hashcode;
	info->loc=loc;
	info->slot=slot;
	info->prior=prior;
}

ulong inline integer_hash(_key_t key){
	return fibonacci_hash((ulong)key,16);
	//return bob_jenkins_32((ulong)key);
	//return shift_hash((ulong)key);
}

bool inline integer_equal(_key_t x,_key_t y){
	return x==y;
}

ulong inline string_hash(_key_t key){
	return crc32_hash(key,strlen(key));
}
bool inline string_equal(_key_t x,_key_t y){
	return strcmp(x,y)==0;
}

