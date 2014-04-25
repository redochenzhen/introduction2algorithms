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

void init_tree_test();
void cleanup_tree_test();

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

bs_tree_pt T1,T2,T3,T4;
long *long_arr=new long[3]{4,6,9};
long *long_arr2=new long[2]{9,1};
student_pt *students=new student_pt[5]{
	new student_t{20,"djame"},
	new student_t{19,"coeus"},
	new student_t{27,"elucy"},
	new student_t{17,"bjack"},
	new student_t{11,"atom"}
};

void init_tree_test(){
	T1=bs_new_tree(NIL);
	//-----------------------------------------------------
	T2=rb_new_tree(NIL);
	rb_reset_tree(T2,(elem_arr_t)long_arr,3);
	//-----------------------------------------------------
	T3=os_new_tree(stu_age_cmp);
	os_reset_tree(T3,(elem_arr_t)students,5);
	//-----------------------------------------------------
	T4=os_new_tree(stu_name_cmp);
	os_reset_tree(T4,(elem_arr_t)students,5);
}

void cleanup_tree_test(){
	delete []long_arr;
	delete []long_arr2;
	for(int i=0;i<5;++i){
		delete students[i];
	}
	delete[] students;
	free_tree(T1);
	free_tree(T2);
	free_tree(T3);
	free_tree(T4);
}

#ifdef TREE_TEST
TEST(TREE,BS_TREE){
	EXPECT_TRUE(is_tree_empty(T1));
	bs_reset_tree(T1,(elem_arr_t)long_arr,2);
	EXPECT_FALSE(is_tree_empty(T1));
	EXPECT_EQ(4,(long)T1->root->satellite);
	bs_reset_tree(T1,(elem_arr_t)long_arr2,2);
	EXPECT_EQ(9,(long)T1->root->satellite);
	make_sub_empty(T1,T1->root);
	EXPECT_TRUE(is_tree_empty(T1));
	//EXPECT_EQ(T1->root->parent,T1->nil);
}

TEST(TREE,RB_TREE){
	EXPECT_EQ(6,(long)T2->root->satellite);
	EXPECT_EQ(RB_BLACK,T2->root->color);
	EXPECT_EQ(4,(long)T2->root->left->satellite);
	EXPECT_EQ(RB_RED,T2->root->left->color);
	EXPECT_EQ(9,(long)T2->root->right->satellite);
	EXPECT_EQ(RB_RED,T2->root->right->color);
	rb_insert(T2,new_node((elem_t)1));
	EXPECT_EQ(1,(long)T2->root->left->left->satellite);
	EXPECT_EQ(RB_BLACK,T2->root->left->color);
	EXPECT_EQ(RB_BLACK,T2->root->right->color);
	EXPECT_EQ(T2->root->parent,T2->nil);
}

TEST(TREE,OS_TREE){
	EXPECT_EQ(5,T3->root->size);
	EXPECT_EQ(3,T3->root->left->size);
	EXPECT_EQ(20,((student_pt)T3->root->satellite)->age);
	tree_node_pt n1=os_select(T3,2);
	EXPECT_EQ(19,((student_pt)n1->satellite)->age);
}

TEST(TREE,OS_TREE2){
	EXPECT_EQ(5,T4->root->size);
	EXPECT_EQ(3,T4->root->left->size);
	EXPECT_EQ(20,((student_pt)T4->root->satellite)->age);
	tree_node_pt n2=os_select(T4,2);
	EXPECT_STREQ("coeus",((student_pt)n2->satellite)->name);
	EXPECT_EQ(4,os_rank(T4,students[2]));
	EXPECT_EQ(T4->root->parent,T4->nil);
}
#endif
#endif /* TREE_TEST_H_ */
