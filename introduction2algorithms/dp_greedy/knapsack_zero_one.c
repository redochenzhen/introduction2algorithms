/*
 * knapsack_zero_one.c
 *
 *  Created on: 2014年4月15日
 *      Author: Coeus
 */

#include "dynamic_programming.h"
#define MAX(a,b) ((a)>(b)?(a):(b))

int knapsack_01(const int weight_lst[],const int value_lst[],int num,int capacity,int* sel_count,int num_lst[]){
	int max_tbl[capacity+1];
	int trace[num][capacity+1];
	int w,n;
	for(w=0;w<=capacity;++w){
		max_tbl[w]=0;
		for(n=0;n<num;++n){
			trace[n][w]=0;
		}
	}
	for(n=0;n<num;++n){
		for(w=capacity;w>=weight_lst[n];--w){
			int sel=value_lst[n]+max_tbl[w-weight_lst[n]];
			if(sel>max_tbl[w]){
				max_tbl[w]=sel;
				trace[n][w]=1;
			}
		}
	}
	//回溯构造最优解
	*sel_count=0;
	for(w=capacity,n=num-1;w>0&&n>=0;--n){
		if(trace[n][w]==1){
			num_lst[*sel_count]=n;
			w-=weight_lst[n];
			++*sel_count;
		}
	}
	return max_tbl[capacity];
}


int knapsack_complete(const int weight_lst[],const int value_lst[],int num,int capacity,int* sel_count,int num_lst[]){
	int max_tbl[capacity+1];
	int trace[num][capacity+1];
	int n,w;
	for(w=0;w<=capacity;++w){
		max_tbl[w]=0;
		for(n=0;n<num;++n){
			trace[n][w]=0;
		}
	}
	for(n=0;n<num;++n){
		for(w=weight_lst[n];w<=capacity;++w){
			int p=w-weight_lst[n];
			int sel=value_lst[n]+max_tbl[p];
			if(sel>max_tbl[w]){
				max_tbl[w]=sel;
				trace[n][w]=trace[n][p]+1;
			}
		}
	}
	//回溯构造最优解
	*sel_count=0;
	for(n=num-1,w=capacity;n>=0&&w>0;--n){
		int t=trace[n][w];
		while(t>0){
			num_lst[*sel_count]=n;
			w-=weight_lst[n];
			++*sel_count;
			--t;
		}
	}
	return max_tbl[capacity];
}
