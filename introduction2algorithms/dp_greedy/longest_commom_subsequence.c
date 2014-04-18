/*
 * longest_commom_subsequence.c
 *
 *  Created on: 2014年4月15日
 *      Author: Coeus
 */

#include <string.h>
#include "dynamic_programming.h"

typedef enum{TRACE_LEFT,TRACE_UP,TRACE_LEFT_UP}trace_t;

/*
 * 	将计算lcs长度的空间优化到O(min{len1,len2})
 *	时间复杂度及用于回溯求解的空间仍需要O(len1*len2)
 */
int lcs(const char* seq1,const char* seq2,char* result){
	size_t len1=strlen(seq1);
	size_t len2=strlen(seq2);
	const char** short_seq=&seq1;
	const char** long_seq=&seq2;
	if(len1>len2){
		short_seq=&seq2;
		long_seq=&seq1;
		len1+=len2;
		len2=len1-len2;
		len1-=len2;
	}
	int lcs_tbl[len1+1];
	trace_t trace[len2][len1];
	int i,j;
	for(i=0;i<=len1;++i){
		lcs_tbl[i]=0;
	}
	//p：记住上一轮（即子序列同时回退时）的lcs长度
	//c：当前lcs长度被覆盖前的值，本次循环结束前转交给p
	int p,c;
	for(j=0;j<len2;++j){
		p=0;
		for(i=0;i<len1;++i){
			c=lcs_tbl[i+1];
			if((*long_seq)[j]==(*short_seq)[i]){
				lcs_tbl[i+1]=p+1;
				trace[j][i]=TRACE_LEFT_UP;
			}else if(lcs_tbl[i]>lcs_tbl[i+1]){
				lcs_tbl[i+1]=lcs_tbl[i];
				trace[j][i]=TRACE_LEFT;
			}else{
				trace[j][i]=TRACE_UP;
			}
			p=c;
		}
	}
	//回溯构造最优解
	int l=lcs_tbl[len1]-1;
	for(i=len1-1,j=len2-1;l>=0;){
		if(trace[j][i]==TRACE_LEFT_UP){
			result[l]=(*short_seq)[i];
			--i;
			--j;
			--l;
		}else if(trace[j][i]==TRACE_LEFT){
			--i;
		}else{
			--j;
		}
	}
	result[lcs_tbl[len1]]='\0';
	return lcs_tbl[len1];
}







