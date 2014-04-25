/*
 * hashing_test.h
 *
 *  Created on: 2014年4月21日
 *      Author: Coeus
 */

#ifndef HASHING_TEST_H_
#define HASHING_TEST_H_

#include <string.h>
#include <gtest/gtest.h>
#include <iostream>
extern "C"{
#include "hashing/hash.h"
}

const ulong l=4000;
ulong *I=NIL;
hash_tbl_pt H1=NIL;
hash_tbl_pt H2=NIL;
dhash_tbl_pt DH1=NIL;
dhash_tbl_pt DH2=NIL;

void init_hashing_test(){
	ulong i;
	I=new ulong[l];
	for(i=0;i<l;++i){
		I[i]=rand();
	}
	//-----------------------------------------------------
	H1=new_hash_tbl(HASH_INTEGER,NIL,NIL);
	for(i=0;i<l;++i){
		hash_insert(H1,(_key_t)(I[i]),(elem_t)(I[i]*I[i]));
	}
	//-----------------------------------------------------
	DH1=new_dhash_tbl(HASH_INTEGER,0,0);
	for(i=0;i<l;++i){
		dhash_insert(DH1,(_key_t)(I[i]),(elem_t)(I[i]*I[i]));
//		std::cout<<D_LOAD_FACTOR(DH1)<<",";
//		if((i+1)%64==0){
//			std::cout<<std::endl;
//		}
	}
	for(i=0;i<2000;++i){
		dhash_delete(DH1,(_key_t)I[i]);
//		std::cout<<D_LOAD_FACTOR(DH1)<<",";
//		if((i+1)%64==0){
//			std::cout<<std::endl;
//		}
	}
	//-----------------------------------------------------
	H2=new_hash_tbl(HASH_STRING,0,0);
	hash_insert(H2,(_key_t)("chen"),(elem_t)(27));
	hash_insert(H2,(_key_t)("wang"),(elem_t)(23));
	hash_insert(H2,(_key_t)("lucy"),(elem_t)(22));
	hash_insert(H2,(_key_t)("tom"),(elem_t)(16));
	hash_insert(H2,(_key_t)("jack"),(elem_t)(11));
	hash_insert(H2,(_key_t)("tomy"),(elem_t)(11));
	hash_insert(H2,(_key_t)("lily"),(elem_t)(33));
	hash_insert(H2,(_key_t)("tim"),(elem_t)(42));
	hash_insert(H2,(_key_t)("coco"),(elem_t)(14));
	hash_insert(H2,(_key_t)("nana"),(elem_t)(23));
	hash_insert(H2,(_key_t)("jorn"),(elem_t)(23));
	hash_delete(H2,H2->slot_arr[2]->key);
	//-----------------------------------------------------
	DH2=new_dhash_tbl(HASH_STRING,0,0);
	dhash_insert(DH2,(_key_t)("chen"),(elem_t)(27));
	dhash_insert(DH2,(_key_t)("wang"),(elem_t)(23));
	dhash_insert(DH2,(_key_t)("lucy"),(elem_t)(22));
	dhash_insert(DH2,(_key_t)("tom"),(elem_t)(16));
	dhash_insert(DH2,(_key_t)("jack"),(elem_t)(11));
	dhash_insert(DH2,(_key_t)("tomy"),(elem_t)(11));
	dhash_insert(DH2,(_key_t)("lily"),(elem_t)(33));
	dhash_insert(DH2,(_key_t)("tim"),(elem_t)(42));
	dhash_insert(DH2,(_key_t)("coco"),(elem_t)(14));
	dhash_insert(DH2,(_key_t)("nana"),(elem_t)(23));
	dhash_insert(DH2,(_key_t)("jorn"),(elem_t)(23));
}

void cleanup_hash_test(){
	free_hash_tbl(H1);
	free_hash_tbl(H2);
	free_dhash_tbl(DH1);
	free_dhash_tbl(DH2);
	delete[] I;
}

#ifdef HASH_TEST
//TEST(HASH,INTEGER_HASH){
//
//}

//TEST(HASH,INTEGER_DHASH){
//	EXPECT_TRUE(dhash_exist(DH1,(_key_t)100));
//	std::cout<<"----"<<D_LOAD_FACTOR(DH1)<<"------";
//}

//TEST(HASH,LOAD_FACTOR){
//	using namespace std;
//	cout<<"load factor:"<<LOAD_FACTOR(H1)<<endl;
//	int x0=0,x1=0,x5=0,x10=0;
//	for(ulong i=0;i<H1->size;++i){
//		cout<<H1->SLOT_DEEP_ARR[i]<<",";
//		if((1+i)%64==0){
//			cout<<endl;
//		}
//		if(H1->SLOT_DEEP_ARR[i]==0) x0++;
//		if(H1->SLOT_DEEP_ARR[i]==1) x1++;
//		if(H1->SLOT_DEEP_ARR[i]>5) x5++;
//		if(H1->SLOT_DEEP_ARR[i]>10) x10++;
//	}
//	cout<<"=0:"<<x0<<endl;
//	cout<<"=1:"<<x1<<endl;
//	cout<<">5:"<<x5<<endl;
//	cout<<">10:"<<x10<<endl;
//}

TEST(HASH,STRING_HASH){
	EXPECT_EQ(27,(ulong)hash_search(H2,"chen"));
	bool result=hash_insert(H2,(_key_t)("chen"),(elem_t)(25));
	EXPECT_FALSE(result);
}
#endif
#endif /* HASHING_TEST_H_ */
