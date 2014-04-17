/*
 * rod_cutting.c
 *
 *  Created on: 2014年4月15日
 *      Author: Coeus
 */
#include "dynamic_programming.h"

/*
 * cut_rod是特化的knapsack_complete问题
 * 特化条件1：物件代价（或重量）分别是1,3,4,5...个数不限
 * 特化条件2：要求背包必须装满（不过若有代价为1的物件且个数不限，则可确保此条件）
 *
 * 可以比较这两个测试DP.CUT_ROD和DP.KNAPSACK_C2
 */
int cut_rod(const int price_list[],int list_len,int rod_len,int* rod_count,int len_arr[]){
	//list_len 也即切割后允许的最大长度
	int max_tbl[rod_len+1];
	int trace[rod_len+1];
	int i,j;
	for(i=0;i<=rod_len;++i){
		max_tbl[i]=0;
	}
	for(i=1;i<=rod_len;++i){
		for(j=1;j<=i&&j<=list_len;++j){
			int sel=price_list[j-1]+max_tbl[i-j];
			if(sel>max_tbl[i]){
				max_tbl[i]=sel;
				trace[i]=j;
			}
		}
	}
	//回溯构造最优解
	*rod_count=0;
	int left=rod_len;
	while(left>0){
		len_arr[*rod_count]=trace[left];
		left-=trace[left];
		++*rod_count;
	}
	return max_tbl[rod_len];
}

//int cut_rod(const int price_list[],int list_len,int rod_len,int* rod_count,int len_arr[]){
//	//list_len 也即切割后允许的最大长度
//	int max_tbl[rod_len+1];
//	int trace[rod_len+1];
//	max_tbl[0]=0;
//	trace[0]=0;
//	int i,j;
//	for(i=1;i<=rod_len;++i){
//		max_tbl[i]=i<=list_len?price_list[i-1]:0;
//		trace[i]=i;
//	}
//	for(i=1;i<=rod_len;++i){					//以下两行这样写，子结构更清晰：
//		for(j=1;j<=i/2&&j<=list_len;++j){		//for(j=1;j<=i&&j<=list_len;++j){
//			int sel=max_tbl[j]+max_tbl[i-j];	//int sel=price_list[j-1]+max_tbl[i-j];
//			if(sel>max_tbl[i]){
//				max_tbl[i]=sel;
//				trace[i]=j;
//			}
//		}
//	}
//	//回溯构造最优解
//	*rod_count=0;
//	int left=rod_len;
//	while(left>0){
//		len_arr[*rod_count]=trace[left];
//		left-=trace[left];
//		++*rod_count;
//	}
//	return max_tbl[rod_len];
//}
