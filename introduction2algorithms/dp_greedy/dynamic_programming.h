/*
 * dynamic_programming.h
 *
 *  Created on: 2014年4月15日
 *      Author: Coeus
 */

#ifndef DYNAMIC_PROGRAMMING_H_
#define DYNAMIC_PROGRAMMING_H_

#include "../contract.h"
//rod cutting:
int cut_rod(const int price_list[],int list_len,int rod_len,int* rod_count,int len_arr[]);

//knapsack:
int knapsack_01(const int weight_lst[],const int value_lst[],int num,int capacity,int* sel_count,int num_lst[]);
int knapsack_complete(const int weight_lst[],const int value_lst[],int num,int capacity,int* sel_count,int num_lst[]);

#endif /* DYNAMIC_PROGRAMMING_H_ */
