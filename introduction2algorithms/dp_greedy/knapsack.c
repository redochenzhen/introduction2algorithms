/*
 * knapsack.c
 *
 *  Created on: 2014年4月15日
 *      Author: Coeus
 */

#include "dynamic_programming.h"

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

int knapsack_complete2(const int weight_lst[],const int value_lst[],int num,int capacity,int* sel_count,int num_lst[]){
	int max_tbl[capacity+1];
	int trace[capacity+1];
	int i,j;
	for(i=0;i<=capacity;++i){
		max_tbl[i]=0;
	}
	for(i=1;i<=capacity;++i){
		//由于考察顺寻递增，因此输出顺序也是递增的
		for(j=0;j<num;++j){
			if(weight_lst[j]>i) continue;
			int sel=value_lst[j]+max_tbl[i-weight_lst[j]];
			if(sel>max_tbl[i]){
				max_tbl[i]=sel;
				trace[i]=j;
			}
		}
	}
	//回溯构造最优解
	*sel_count=0;
	int left=capacity;
	while(left>0){
		num_lst[*sel_count]=trace[left];
		left-=weight_lst[trace[left]];
		++*sel_count;
	}
	return max_tbl[capacity];
}



















