/*
 * avl_tree.c
 *
 *  Created on: 2014年4月1日
 *      Author: Coeus
 */

#include "contract.h"
#include "tree.h"
#include <stdlib.h>

static void left_rotate_with_bf(bs_tree_pt tree,tree_node_pt p);
static void right_rotate_with_bf(bs_tree_pt tree,tree_node_pt p);
static void lr_rotate_with_bf(bs_tree_pt tree,tree_node_pt node);
static void rl_rotate_with_bf(bs_tree_pt tree,tree_node_pt node);
static void insert_fixup(bs_tree_pt tree,tree_node_pt node);
static void delete_fixup(bs_tree_pt tree,tree_node_pt node);

tree_node_pt avl_new_node(elem_t satellite){
	tree_node_pt node=(tree_node_pt)malloc(sizeof(tree_node_t));
	node->satellite=satellite;
	node->bf=0;
	return node;
}

bs_tree_pt avl_new_tree(compare_func_t compare,identify_func_t identify){
	bs_tree_pt tree=new_tree(compare,identify);
	tree->nil=avl_new_node(NIL);
	tree->root=tree->nil;
	tree->type=TREE_AVL;
	return tree;
}

void avl_reset_tree(bs_tree_pt tree,elem_arr_t satellite_arr,int length){
	free_sub(tree,tree->root);
	if(length<=0) return;
	int i=0;
	for(;i<length;++i){
		int x=satellite_arr[i];
		avl_insert(tree,avl_new_node(satellite_arr[i]));
	}
}

void avl_insert(bs_tree_pt tree,tree_node_pt node){
	insert_node(tree,node);
	node->bf=0;
	insert_fixup(tree,node);
}

void insert_fixup(bs_tree_pt tree,tree_node_pt node){
	while(node!=tree->root){
		tree_node_pt p=node->parent;
		if(node==p->left){
			p->bf--;
		}else{
			p->bf++;
		}
		if(p->bf==0) return;
		else if(p->bf==1||p->bf==-1){
			node=p;
			continue;
		}else{
			int bf=node->bf+p->bf;
			switch(bf){
			case -3:
				right_rotate_with_bf(tree,p);
				return;
			case 3:
				left_rotate_with_bf(tree,p);
				return;
			case -1:
				lr_rotate_with_bf(tree,node);
				return;
			case 1:
				rl_rotate_with_bf(tree,node);
				return;
			}
		}
	}
}

tree_node_pt avl_delete(bs_tree_pt tree,const tree_node_pt node){
	tree_node_pt del=node;
	if(del->left==tree->nil){
		del=del->right;
	}else if(del->right==tree->nil){
		del=del->left;
	}else{
		del=minimum_sub(tree,del->right);
		del->bf=node->bf;
		del=del->right;
	}
	delete_node(tree,node);
	//此时del可能指向nil，但是delete_node方法保证nil->parent暂时有意义
	delete_fixup(tree,del);
	node->parent=NIL;
	node->left=NIL;
	node->right=NIL;
	return node;
}

void delete_fixup(bs_tree_pt tree,tree_node_pt node){
	while(node!=tree->root){
		tree_node_pt p=node->parent;
		tree_node_pt sibling=NIL;
		//特殊情况，会使得p的左右子节点相同
		if(p->left==tree->nil&&p->right==tree->nil){
			p->bf=0;
		}
		else if(node==p->left){
			p->bf++;
			sibling=p->right;
		}else{
			p->bf--;
			sibling=p->left;
		}
		if(p->bf==0){
			node=p;
			continue;
		}else if(p->bf==1||p->bf==-1) return;
		else{
			int bf=sibling->bf+p->bf;
			switch(bf){
			case 3:
				left_rotate_with_bf(tree,p);
				break;
			case 2:
				left_rotate_with_bf(tree,p);
				return;
			case -3:
				right_rotate_with_bf(tree,p);
				break;
			case -2:
				right_rotate_with_bf(tree,p);
				return;
			case 1:
				rl_rotate_with_bf(tree,sibling);
				break;
			case -1:
				lr_rotate_with_bf(tree,sibling);
				break;
			}
			//进入下一次循环之前，应将node指向参与旋转的最高节点
			node=node->parent->parent;
		}
	}
}

void left_rotate_with_bf(bs_tree_pt tree,tree_node_pt p){
	if(p->right->bf==0){
		p->bf=1;
		p->right->bf=-1;
	}else{	//else==1
		p->bf=0;
		p->right->bf=0;
	}
	left_rotate(tree,p);
}

void right_rotate_with_bf(bs_tree_pt tree,tree_node_pt p){
	if(p->left->bf==0){
		p->bf=-1;
		p->left->bf=1;
	}else{	//else==-1
		p->bf=0;
		p->left->bf=0;
	}
	right_rotate(tree,p);
}

void lr_rotate_with_bf(bs_tree_pt tree,tree_node_pt node){
	tree_node_pt r=node->right;
	tree_node_pt p=node->parent;
	if(r->bf<=0){
		node->bf=0;
		p->bf=-r->bf;
	}else{
		node->bf=-1;
		p->bf=0;
	}
	r->bf=0;
	left_rotate(tree,node);
	right_rotate(tree,p);
}

void rl_rotate_with_bf(bs_tree_pt tree,tree_node_pt node){
	tree_node_pt l=node->left;
	tree_node_pt p=node->parent;
	if(l->bf>=0){
		node->bf=0;
		p->bf=-l->bf;
	}else{
		node->bf=1;
		p->bf=0;
	}
	l->bf=0;
	right_rotate(tree,node);
	left_rotate(tree,p);
}
