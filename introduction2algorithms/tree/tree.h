/*
 * avl_tree.c
 *
 *  Created on: 2014年4月1日
 *      Author: Coeus
 */

#ifndef TREE_H_
#define TREE_H_

#include "contract.h"

//���������ɫ
typedef enum{RB_RED,RB_BLACK}rb_color_t;
//����������������
typedef enum{TREE_BS,TREE_RB,TREE_AVL,TREE_OS}tree_type_t;

typedef struct _tree_node *tree_node_pt;
typedef struct _tree_node{
	tree_node_pt parent;
	tree_node_pt left;
	tree_node_pt right;
	union{
		rb_color_t color;
		int bf;
	};
	int key;
	void *satellite;
}tree_node_t;

typedef void (*node_predicate_t)(tree_node_pt);

typedef struct _bs_tree{
	tree_node_pt root;
	tree_node_pt nil;
	tree_type_t type;
}bs_tree_t,*bs_tree_pt;

typedef const bs_tree_t* bs_tree_cpt;

//bs tree fuction
tree_node_pt new_node(int key);
bs_tree_pt new_tree();
void reset_tree(bs_tree_pt tree,int keys[],int length);
bool is_tree_empty(bs_tree_cpt tree);
tree_node_pt minimum_sub(bs_tree_cpt tree,tree_node_pt sub_root);
tree_node_pt maximum_sub(bs_tree_cpt tree,tree_node_pt sub_root);
tree_node_pt successor(bs_tree_cpt tree,tree_node_pt node);
tree_node_pt predecessor(bs_tree_cpt tree,tree_node_pt node);
void free_sub(bs_tree_pt tree,tree_node_pt sub_root);
void free_tree(bs_tree_pt tree);
//node�����ͣ�tree_node_t *const
tree_node_pt delete_node(bs_tree_pt tree,const tree_node_pt node);
void insert_node(bs_tree_pt tree,tree_node_pt node);
void transplant_node(bs_tree_pt tree,tree_node_pt from,tree_node_pt to);
void left_rotate(bs_tree_pt tree,tree_node_pt node);
void right_rotate(bs_tree_pt tree,tree_node_pt node);
void inorder_walk(bs_tree_pt tree,node_predicate_t func);
void inorder_walk_sub(bs_tree_pt tree,tree_node_pt sub_root,node_predicate_t func);
void preorder_walk(bs_tree_pt tree,node_predicate_t func);
void preorder_walk_sub(bs_tree_pt tree,tree_node_pt sub_root,node_predicate_t func);
void postorder_walk(bs_tree_pt tree,node_predicate_t func);
void postorder_walk_sub(bs_tree_pt tree,tree_node_pt sub_root,node_predicate_t func);
tree_node_pt search_key(bs_tree_cpt tree,int key);
tree_node_pt search_key_sub(bs_tree_cpt tree,tree_node_pt sub_root,int key);

//rb tree fuction
tree_node_pt rb_new_node(int key);
bs_tree_pt rb_new_tree();
void rb_reset_tree(bs_tree_pt tree,int keys[],int length);
void rb_insert(bs_tree_pt tree,tree_node_pt node);
tree_node_pt rb_delete(bs_tree_pt tree,const tree_node_pt node);

//avl tree fuction
tree_node_pt avl_new_node(int key);
bs_tree_pt avl_new_tree();
void avl_reset_tree(bs_tree_pt tree,int keys[],int length);
void avl_insert(bs_tree_pt tree,tree_node_pt node);
tree_node_pt avl_delete(bs_tree_pt tree,const tree_node_pt node);
#endif /* TREE_H_ */
