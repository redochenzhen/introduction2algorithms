/*
 * dp_greedy_test.h
 *
 *  Created on: 2014年4月15日
 *      Author: Coeus
 */

#ifndef DP_GREEDY_TEST_H_
#define DP_GREEDY_TEST_H_
#include <gtest/gtest.h>
extern "C"{
#include <dp_greedy/dynamic_programming.h>
}

#ifdef DP_TEST
extern int cut_rod(const int*,int,int,int*,int*);
extern int knapsack_01(const int*,const int*,int,int,int*,int*);
extern int knapsack_complete(const int*,const int*,int,int,int*,int*);

TEST(DP,CUT_ROD){
	int price_list[10]{1,5,8,9,10,17,17,20,24,30};
	int length_arr[100];
	int max_value;
	int rod_count;
	max_value=cut_rod(price_list,10,9,&rod_count,length_arr);
	EXPECT_EQ(25,max_value);
	EXPECT_EQ(2,rod_count);
	int expect_length_arr[]{3,6,10,10,10};
	for(int i=0;i<rod_count;++i){
		EXPECT_EQ(expect_length_arr[i],length_arr[i]);
	}
	//---------------------------------------------------------
	max_value=cut_rod(price_list,10,39,&rod_count,length_arr);
	EXPECT_EQ(115,max_value);
	for(int i=0;i<rod_count;++i){
		EXPECT_EQ(expect_length_arr[i],length_arr[i]);
	}
}

TEST(DP,KNAPSACK_01){
	int weight_lst[]{3,4,5};
	int value_lst[]{4,5,6};
	int num_lst[10];
	int max_value;
	int sel_count;
	max_value=knapsack_01(weight_lst,value_lst,3,10,&sel_count,num_lst);
	EXPECT_EQ(11,max_value);
	EXPECT_EQ(2,sel_count);
	int expect_num_lst[]{2,1};
	int v=0;
	for(int i=0;i<sel_count;++i){
		EXPECT_EQ(expect_num_lst[i],num_lst[i]);
		v+=value_lst[num_lst[i]];
	}
	EXPECT_EQ(max_value,v);
	//---------------------------------------------------------
	int W[]{7,3,2,8,5};
	int V[]{3,3,4,19,11};
	max_value=knapsack_01(W,V,5,15,&sel_count,num_lst);
	EXPECT_EQ(34,max_value);
	EXPECT_EQ(3,sel_count);
}

TEST(DP,KNAPSACK_C){
	int weight_lst[]{3,4,5};
	int value_lst[]{4,5,6};
	int num_lst[10];
	int max_value;
	int sel_count;
	max_value=knapsack_complete(weight_lst,value_lst,3,10,&sel_count,num_lst);
	EXPECT_EQ(13,max_value);
	EXPECT_EQ(3,sel_count);
	int expect_num_lst[]{1,0,0};
	int v=0;
	for(int i=0;i<sel_count;++i){
		EXPECT_EQ(expect_num_lst[i],num_lst[i]);
		v+=value_lst[num_lst[i]];
	}
	EXPECT_EQ(max_value,v);
	//---------------------------------------------------------
	int W[]{1,2,3,4,5,6,7,8,9,10};
	int V[]{1,5,8,9,10,17,17,20,24,30};
	max_value=knapsack_complete(W,V,10,9,&sel_count,num_lst);
	EXPECT_EQ(25,max_value);
	EXPECT_EQ(2,sel_count);
	EXPECT_EQ(5,num_lst[0]);
	EXPECT_EQ(2,num_lst[1]);
}
#endif
#endif /* DP_GREEDY_TEST_H_ */
