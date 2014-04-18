/*
 * rb_tree.c
 *
 *  Created on: 2014年4月1日
 *      Author: Coeus
 */

#include "tree.h"
#include <stdlib.h>

/*
 * 带色旋转，如果下色是红色，不会引起任何黑高改变；如果下
 * 色是黑色，会引起上节点的黑高加1，旋转轴的子节点黑高减1
 */
static void left_rotate_with_color(bs_tree_pt tree,tree_node_pt node);
static void right_rotate_with_color(bs_tree_pt tree,tree_node_pt node);
static void insert_fixup(bs_tree_pt tree,tree_node_pt node);
static void delete_fixup(bs_tree_pt tree,tree_node_pt node);

tree_node_pt rb_new_node(elem_t satellite){
	tree_node_pt node=(tree_node_pt)malloc(sizeof(tree_node_t));
	node->satellite=satellite;
	node->color=RB_RED;
	return node;
}

bs_tree_pt rb_new_tree(compare_func_t compare){
	bs_tree_pt tree=bs_new_tree(compare);
	tree->nil=rb_new_node(NIL);
	tree->nil->color=RB_BLACK;
	tree->root=tree->nil;
	tree->type=TREE_RB;
	return tree;
}

void rb_reset_tree(bs_tree_pt tree,elem_arr_t satellite_arr,long int length){
	long int i;
	make_sub_empty(tree,tree->root);
	if(length<=0) return;
	for(i=0;i<length;++i){
		rb_insert(tree,rb_new_node(satellite_arr[i]));
	}
}

void rb_insert(bs_tree_pt tree,tree_node_pt node){
	bs_insert(tree,node);
	node->color=RB_RED;
	insert_fixup(tree,node);
}

void insert_fixup(bs_tree_pt tree,tree_node_pt node){
	while(node->parent->color==RB_RED){
		if(node->parent==node->parent->parent->left){
			tree_node_pt uncle=node->parent->parent->right;
			if(uncle->color==RB_RED){
				node->parent->color=RB_BLACK;
				uncle->color=RB_BLACK;
				uncle->parent->color=RB_RED;
				node=uncle->parent;
				continue;
			}else if(node==node->parent->right){
				left_rotate(tree,node->parent);
				node=node->left;
				continue;
			}else{
				right_rotate_with_color(tree,node->parent->parent);
				break;
			}
		}else{
			//以下代码是上面情形的镜像
			tree_node_pt uncle=node->parent->parent->left;
			if(uncle->color==RB_RED){
				node->parent->color=RB_BLACK;
				uncle->color=RB_BLACK;
				uncle->parent->color=RB_RED;
				node=uncle->parent;
				continue;
			}else if(node==node->parent->left){
				right_rotate(tree,node->parent);
				node=node->right;
				continue;
			}else{
				left_rotate_with_color(tree,node->parent->parent);
				break;
			}
		}
	}
	tree->root->color=RB_BLACK;
}

tree_node_pt rb_delete(bs_tree_pt tree,const tree_node_pt node){
	/*
	 * 跟插入不同，不能简单调用bs_delete
	 */
	tree_node_pt del=node;
	rb_color_t d_color=node->color;
	if(node->left==tree->nil){
		del=node->right;
		transplant_node(tree,node->right,node);
	}else if(node->right==tree->nil){
		del=node->left;
		transplant_node(tree,node->left,node);
	}else{
		del=bs_minimum_sub(tree,del->right);
		d_color=del->color;
		del->color=node->color;
		//对右子树最小节点不是右节点的情况进行转化
		if(node!=del->parent){
			transplant_node(tree,del->right,del);
			del->right=node->right;
			del->right->parent=del;
		}else{
			//防止del->right为nil时无法向上追溯,其它情况由transplant_node保证
			del->right->parent=del;
		}
		transplant_node(tree,del,node);
		del->left=node->left;
		del->left->parent=del;
		del=del->right;
	}
	if(d_color==RB_BLACK){
		//此时del可能指向nil，但是delete_node方法保证nil->parent暂时有意义
		delete_fixup(tree,del);
	}
	node->parent=NIL;
	node->left=NIL;
	node->right=NIL;
	return node;
}

void delete_fixup(bs_tree_pt tree,tree_node_pt node){
	while(node!=tree->root&&node->color==RB_BLACK){
		if(node==node->parent->left){
			tree_node_pt sibling=node->parent->right;
			if(sibling->color==RB_RED){
				left_rotate_with_color(tree,node->parent);
				sibling=node->parent->right;
			}
			//此时已经确保sibling为黑色
			if(sibling->left->color==RB_BLACK&&sibling->right->color==RB_BLACK){
				//使得node->parent的黑高减1，从而推向对parent的修复
				sibling->color=RB_RED;
				node=node->parent;
				continue;
			}else{
				if(sibling->right->color==RB_BLACK){
					right_rotate_with_color(tree,sibling);
					sibling=node->parent->right;
				}
				//此时已经确保sibling->right为红色
				sibling->color=RB_BLACK;
				left_rotate_with_color(tree,node->parent);
				return;
			}
		}else{
			//以下代码是上面情形的镜像
			tree_node_pt sibling=node->parent->left;
			if(sibling->color==RB_RED){
				right_rotate_with_color(tree,node->parent);
				sibling=node->parent->left;
			}
			//此时已经确保sibling为黑色
			if(sibling->right->color==RB_BLACK&&sibling->left->color==RB_BLACK){
				//使得node->parent的黑高减1，从而推向对parent的修复
				sibling->color=RB_RED;
				node=node->parent;
				continue;
			}else{
				if(sibling->left->color==RB_BLACK){
					left_rotate_with_color(tree,sibling);
					sibling=node->parent->left;
				}
				//此时已经确保sibling->left为红色
				sibling->color=RB_BLACK;
				right_rotate_with_color(tree,node->parent);
				return;
			}
		}
	}
	node->color=RB_BLACK;
}

void left_rotate_with_color(bs_tree_pt tree,tree_node_pt node){
	rb_color_t left_color=node->color;
	node->color=node->right->color;
	node->right->color=left_color;
	left_rotate(tree,node);
}

void right_rotate_with_color(bs_tree_pt tree,tree_node_pt node){
	rb_color_t right_color=node->color;
	node->color=node->left->color;
	node->left->color=right_color;
	right_rotate(tree,node);
}
