/*
 * rb_tree.c
 *
 *  Created on: 2014年4月1日
 *      Author: Coeus
 */

#include "contract.h"
#include "tree.h"
#include <stdlib.h>

/*
 * 带色旋转，如果下色是红色，不会引起任何黑高改变；
 * 如果下色是黑色，会引起旋转轴的子节点黑高减1，兄弟节点的黑高加1
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

bs_tree_pt rb_new_tree(compare_func_t compare,identify_func_t identify){
	bs_tree_pt tree=new_tree(compare,identify);
	tree->nil=rb_new_node(NIL);
	tree->nil->color=RB_BLACK;
	tree->root=tree->nil;
	tree->type=TREE_RB;
	return tree;
}

void rb_reset_tree(bs_tree_pt tree,elem_arr_t satellite_arr,int length){
	free_sub(tree,tree->root);
	if(length<=0) return;
	int i=0;
	for(;i<length;++i){
		rb_insert(tree,rb_new_node(satellite_arr[i]));
	}
}

void rb_insert(bs_tree_pt tree,tree_node_pt node){
	insert_node(tree,node);
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
	tree_node_pt del=node;
	rb_color_t d_color=del->color;
	if(del->left==tree->nil){
		del=del->right;
	}else if(del->right==tree->nil){
		del=del->left;
	}else{
		del=minimum_sub(tree,del->right);
		d_color=del->color;
		del->color=node->color;
		del=del->right;
	}
	delete_node(tree,node);
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
