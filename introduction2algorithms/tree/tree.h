/*
 * avl_tree.c
 *
 *  Created on: 2014年4月1日
 *      Author: Coeus
 */

#ifndef TREE_H_
#define TREE_H_

#include "../contract.h"

//红黑树的颜色
typedef enum{RB_RED,RB_BLACK}rb_color_t;
//二叉搜索树的类型
typedef enum{TREE_BS,TREE_RB,TREE_AVL,TREE_OS}tree_type_t;

typedef void *elem_t,*ext_t;
typedef void **elem_arr_t;

typedef struct _tree_node *tree_node_pt;
typedef struct _tree_node{
	tree_node_pt parent;
	tree_node_pt left;
	tree_node_pt right;
	union{
		rb_color_t color;
		int bf;
	};
	union{
		long size;
		ext_t property;
	};
	elem_t satellite;
}tree_node_t;

typedef void (*node_predicate_t)(tree_node_pt);
typedef int (*compare_func_t)(elem_t,elem_t);

typedef struct _bs_tree{
	tree_node_pt root;
	tree_node_pt nil;
	tree_type_t type;
	compare_func_t compare;
}bs_tree_t,*bs_tree_pt;

typedef const bs_tree_t* bs_tree_cpt;

bool is_tree_empty(bs_tree_cpt tree);
tree_node_pt new_node(elem_t satellite);
void inorder_walk(bs_tree_pt tree,node_predicate_t func);
void inorder_walk_sub(bs_tree_pt tree,tree_node_pt sub_root,node_predicate_t func);
void preorder_walk(bs_tree_pt tree,node_predicate_t func);
void preorder_walk_sub(bs_tree_pt tree,tree_node_pt sub_root,node_predicate_t func);
void postorder_walk(bs_tree_pt tree,node_predicate_t func);
void postorder_walk_sub(bs_tree_pt tree,tree_node_pt sub_root,node_predicate_t func);
void make_sub_empty(bs_tree_pt tree,tree_node_pt sub_root);
void free_tree(bs_tree_pt tree);
void transplant_node(bs_tree_pt tree,tree_node_pt from,tree_node_pt to);
void left_rotate(bs_tree_pt tree,tree_node_pt node);
void right_rotate(bs_tree_pt tree,tree_node_pt node);

//bs tree fuction
bs_tree_pt bs_new_tree(compare_func_t compare);
void bs_reset_tree(bs_tree_pt tree,elem_arr_t satellite_arr,long length);
//node形参的展开类型：tree_node_t *const
tree_node_pt bs_delete(bs_tree_pt tree,const tree_node_pt node);
void bs_insert(bs_tree_pt tree,tree_node_pt node);

tree_node_pt tree_search(bs_tree_cpt tree,elem_t satellite);
tree_node_pt tree_search_sub(bs_tree_cpt tree,tree_node_pt sub_root,elem_t satellite);
tree_node_pt minimum_sub(bs_tree_cpt tree,tree_node_pt sub_root);
tree_node_pt maximum_sub(bs_tree_cpt tree,tree_node_pt sub_root);
tree_node_pt successor(bs_tree_cpt tree,tree_node_pt node);
tree_node_pt predecessor(bs_tree_cpt tree,tree_node_pt node);


//rb tree fuction
tree_node_pt rb_new_node(elem_t satellite);
bs_tree_pt rb_new_tree(compare_func_t compare);
void rb_reset_tree(bs_tree_pt tree,elem_arr_t satellite_arr,long length);
void rb_insert(bs_tree_pt tree,tree_node_pt node);
tree_node_pt rb_delete(bs_tree_pt tree,const tree_node_pt node);

//os tree function
tree_node_pt os_new_node(elem_t satellite);
bs_tree_pt os_new_tree(compare_func_t compare);
void os_reset_tree(bs_tree_pt tree,elem_arr_t satellite_arr,long length);
void os_insert(bs_tree_pt tree,tree_node_pt node);
tree_node_pt os_delete(bs_tree_pt tree,const tree_node_pt node);
tree_node_pt os_select(bs_tree_pt tree,long index);
long os_rank(bs_tree_pt tree,elem_t satellite);
long os_rank_node(bs_tree_pt tree,tree_node_pt node);

//avl tree fuction
tree_node_pt avl_new_node(elem_t satellite);
bs_tree_pt avl_new_tree(compare_func_t compare);
void avl_reset_tree(bs_tree_pt tree,elem_arr_t satellite_arr,long length);
void avl_insert(bs_tree_pt tree,tree_node_pt node);
tree_node_pt avl_delete(bs_tree_pt tree,const tree_node_pt node);

#endif /* TREE_H_ */
