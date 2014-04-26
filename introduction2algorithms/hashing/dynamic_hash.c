/*
 * dynamic_hash.c
 *
 *  Created on: 2014年4月20日
 *      Author: Coeus
 */

#include "hash_func.h"
#include "hash.h"
#include <stdlib.h>
#include <string.h>

extern slot_pt new_slot_no_calculate(_key_t key,ulong hashcode,elem_t satellite);
static void expand_table(dhash_tbl_pt table);
static bool search_slot(dhash_tbl_pt table,_key_t key,ulong* p_hashcode,slot_pt** pp_slot);

static uint _log2(uint n){
	uint i=0;
	while(n>>=1) ++i;
	return i;
}

static ulong locate_slot(dhash_tbl_cpt table,ulong hashcode){
	hashcode&=table->high_mask;
	if(hashcode>table->slot_max){
		hashcode&=table->low_mask;
	}
	return hashcode;
}

static slot_segment_pt inline new_slot_segment(){
	slot_segment_pt seg=(slot_segment_pt)malloc(sizeof(slot_segment_t));
	memset(seg,0,sizeof(slot_segment_t));
	return seg;
}

static void free_slot_segment(slot_segment_pt seg){
	ulong loc=0;
	slot_pt slot,next;
	for(;loc<DEFAULT_SEG_SIZE;++loc){
		slot=seg->slot_arr[loc];
		while(slot!=NIL){
			next=slot->next;
			free(slot);
			slot=next;
		}
	}
	free(seg);
}

static void inline resize_directory(dhash_tbl_pt table,ulong newsize){
	ulong oldsize=table->dir_size;
	table->seg_arr=(slot_segment_pt*)realloc(table->seg_arr,sizeof(void*)*newsize);
	memset(table->seg_arr+oldsize,0,sizeof(void*)*(newsize-oldsize));
	table->dir_size=newsize;
}

dhash_tbl_pt new_dhash_tbl(hash_type_t type,hash_func_t hash_func,equal_func_t equal_func){
	dhash_tbl_pt table=(dhash_tbl_pt)malloc(sizeof(dhash_tbl_t));
	memset(table,0,sizeof(dhash_tbl_t));
	resize_directory(table,DEFAULT_DIR_SIZE);
	table->seg_shift=_log2(DEFAULT_SEG_SIZE);
	table->seg_arr[0]=new_slot_segment();
	table->slot_max=DEFAULT_SEG_SIZE-1;
	table->high_mask=DEFAULT_SEG_SIZE-1;
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
	return table;
}

bool dhash_insert(dhash_tbl_pt table,_key_t key,elem_t satellite){
	ulong hashcode;
	slot_pt* p_slot;
	slot_pt newslot;
	if(search_slot(table,key,&hashcode,&p_slot)){
		return false;
	}
	newslot=new_slot_no_calculate(key,hashcode,satellite);
	newslot->next=*p_slot;
	*p_slot=newslot;
	table->count++;
	return true;
}

bool inline dhash_exist(dhash_tbl_pt table,_key_t key){
	ulong tmp1;
	slot_pt *tmp2;
	return search_slot(table,key,&tmp1,&tmp2);
}

bool inline dhash_search(dhash_tbl_pt table,_key_t key,elem_t* out_elem){
	ulong tmp;
	slot_pt *p_slot;
	if(!search_slot(table,key,&tmp,&p_slot)){
		return false;
	}
	*out_elem=(*p_slot)->satellite;
	return true;
}

elem_t dhash_delete(dhash_tbl_pt table,_key_t key){
	ulong tmp;
	elem_t elem=NIL;
	slot_pt* p_slot;
	slot_pt del;
	if(!search_slot(table,key,&tmp,&p_slot)){
		return NIL;
	}
	del=*p_slot;
	elem=del->satellite;
	*p_slot=del->next;	//or:*p_slot=*p_slot->next;
	free(del);
	table->count--;
	return elem;
}

void free_dhash_tbl(dhash_tbl_pt table){
	ulong idx=0;
	slot_segment_pt seg=table->seg_arr[0];
	while(seg!=NIL){
		free_slot_segment(seg);
		seg=table->seg_arr[++idx];
	}
	free(table->seg_arr);
	free(table);
}

static bool search_slot(dhash_tbl_pt table,_key_t key,ulong* p_hashcode,slot_pt** pp_slot){
	//--------------------------------------------------
	if(D_LOAD_FACTOR(table)>DEFAULT_LOAD_FACTOR){
		expand_table(table);
	}
	//--------------------------------------------------
	ulong hashcode=table->hash(key);
	ulong slot_num=locate_slot(table,hashcode);
	ulong seg_idx=slot_num>>table->seg_shift;
	ulong loc=MOD_POW2(slot_num,DEFAULT_SEG_SIZE);
	slot_pt* p_slot=&table->seg_arr[seg_idx]->slot_arr[loc];
	while(*p_slot!=NIL){
		if(hashcode==(*p_slot)->hashcode&&table->is_equal(key,(*p_slot)->key)){
			break;
		}else{
			p_slot=&(*p_slot)->next;
		}
	}
	*p_hashcode=hashcode;
	*pp_slot=p_slot;
	return *p_slot!=NIL;
}

static void expand_table(dhash_tbl_pt table){
	ulong new_max=table->slot_max+1;
	ulong new_seg_idx=new_max>>table->seg_shift;
	ulong loc=MOD_POW2(new_max,DEFAULT_SEG_SIZE);
	if(new_seg_idx==table->dir_size){
		resize_directory(table,table->dir_size<<1);
	}
	if(table->seg_arr[new_seg_idx]==NIL){
		table->seg_arr[new_seg_idx]=new_slot_segment();
	}
	//如果new_max达到2的幂次方，更新mask
	if(new_max>table->high_mask){
		table->low_mask=table->high_mask;
		table->high_mask|=new_max;
	}
	table->slot_max=new_max;	//slot_max++
	//移动应放入new_max的节点
	ulong old_slot_num=new_max&table->low_mask;
	ulong old_seg_idx=old_slot_num>>table->seg_shift;
	slot_segment_pt old_seg=table->seg_arr[old_seg_idx];
	slot_segment_pt new_seg=table->seg_arr[new_seg_idx];
	slot_pt slot=old_seg->slot_arr[loc];
	slot_pt *old_slot=&old_seg->slot_arr[loc];
	slot_pt *new_slot=&new_seg->slot_arr[loc];
	ulong flag_mask=table->low_mask+1;
	while(slot!=NIL){
		if(slot->hashcode&flag_mask){
			*new_slot=slot;
			new_slot=&slot->next;
		}else{
			*old_slot=slot;
			old_slot=&slot->next;
		}
		slot=slot->next;
	}
	*new_slot=NIL;
	*old_slot=NIL;
//	比较以下版本和以上版本，上面充分利用了指针
//	while(slot!=NIL){
//		if(slot->hashcode&flag_mask){
//			if(new_slot==NIL){
//				new_seg->slot_arr[loc]=slot;
//			}else{
//				new_slot->next=slot;
//			}
//			new_slot=slot;
//		}else{
//			if(old_slot==NIL){
//				old_seg->slot_arr[loc]=slot;
//			}else{
//				old_slot->next=slot;
//			}
//			old_slot=slot;
//		}
//		slot=slot->next;
//	}
}










