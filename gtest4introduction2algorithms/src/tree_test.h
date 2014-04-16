/*
 * tree_test.h
 *
 *  Created on: 2014年4月14日
 *      Author: coeus
 */

#ifndef TREE_TEST_H_
#define TREE_TEST_H_

#include <string.h>
#include <gtest/gtest.h>
extern "C"{
#include "tree/tree.h"
}

#ifdef TREE_TEST
static bs_tree_pt T=0;
typedef struct _student{
	unsigned short age;
	const char* name;
}student_t,*student_pt;

int stu_age_cmp(void* x,void* y){
	unsigned short x_age=((student_pt)x)->age;
	unsigned short y_age=((student_pt)y)->age;
	return x_age-y_age;
}

int stu_name_cmp(void* x,void* y){
	const char* x_name=((student_pt)x)->name;
	const char* y_name=((student_pt)y)->name;
	return strcmp(x_name,y_name);
}

TEST(TREE,BS_TREE){
	T=bs_new_tree(0);
	EXPECT_TRUE(is_tree_empty(T));
	bs_reset_tree(T,(elem_arr_t)new long[2]{4,6},2);
	EXPECT_FALSE(is_tree_empty(T));
	EXPECT_EQ(4,(long)T->root->satellite);
	bs_reset_tree(T,(elem_arr_t)new long[2]{9,1},2);
	EXPECT_EQ(9,(long)T->root->satellite);
	make_sub_empty(T,T->root);
	EXPECT_TRUE(is_tree_empty(T));
	//EXPECT_EQ(T->root->parent,T->nil);
	free_tree(T);
}

TEST(TREE,RB_TREE){
	T=rb_new_tree(0);
	rb_reset_tree(T,(elem_arr_t)new long[3]{4,6,9},3);
	EXPECT_EQ(6,(long)T->root->satellite);
	EXPECT_EQ(RB_BLACK,T->root->color);
	EXPECT_EQ(4,(long)T->root->left->satellite);
	EXPECT_EQ(RB_RED,T->root->left->color);
	EXPECT_EQ(9,(long)T->root->right->satellite);
	EXPECT_EQ(RB_RED,T->root->right->color);
	rb_insert(T,new_node((elem_t)1));
	EXPECT_EQ(1,(long)T->root->left->left->satellite);
	EXPECT_EQ(RB_BLACK,T->root->left->color);
	EXPECT_EQ(RB_BLACK,T->root->right->color);
	EXPECT_EQ(T->root->parent,T->nil);
	free_tree(T);
}

TEST(TREE,OS_TREE){
	//11<17<19<20<27
	//a<b<c<d<e
	student_pt students[5]={
		new student_t{20,"djame"},
		new student_t{19,"coeus"},
		new student_t{27,"elucy"},
		new student_t{17,"bjack"},
		new student_t{11,"atom"}
	};
	T=os_new_tree(stu_age_cmp);
	os_reset_tree(T,(elem_arr_t)students,5);
	EXPECT_EQ(5,T->root->size);
	EXPECT_EQ(3,T->root->left->size);
	EXPECT_EQ(20,((student_pt)T->root->satellite)->age);
	tree_node_pt n1=os_select(T,2);
	EXPECT_EQ(19,((student_pt)n1->satellite)->age);
	//-------------------------------------------
	T=os_new_tree(stu_name_cmp);
	os_reset_tree(T,(elem_arr_t)students,5);
	EXPECT_EQ(5,T->root->size);
	EXPECT_EQ(3,T->root->left->size);
	EXPECT_EQ(20,((student_pt)T->root->satellite)->age);
	tree_node_pt n2=os_select(T,2);
	EXPECT_STREQ("coeus",((student_pt)n2->satellite)->name);
	EXPECT_EQ(4,os_rank(T,students[2]));
	for(int i=0;i<5;++i){
		delete students[i];
	}
	EXPECT_EQ(T->root->parent,T->nil);
	free_tree(T);
}
#endif
#endif /* TREE_TEST_H_ */
