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

tree_node_pt avl_new_node(int key){
	tree_node_pt node=(tree_node_pt)malloc(sizeof(tree_node_t));
	node->key=key;
	node->bf=0;
	return node;
}

bs_tree_pt avl_new_tree(){
	bs_tree_pt tree=(bs_tree_pt)malloc(sizeof(bs_tree_t));
	tree->nil=NIL;
	tree->root=NIL;
	tree->type=TREE_AVL;
	return tree;
}

void avl_reset_tree(bs_tree_pt tree,int keys[],int length){
	free_sub(tree,tree->root);
	if(length<=0) return;
	int i=0;
	for(;i<length;++i){
		avl_insert(tree,avl_new_node(keys[i]));
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
	if(del==tree->nil){
		if(node->parent->bf==1||node->parent->bf==-1){
			node->parent->bf=0;
			del=node->parent;
		}
	}
	delete_node(tree,node);
	//此时del不可能指向nil
	delete_fixup(tree,del);
	node->parent=NIL;
	node->left=NIL;
	node->right=NIL;
	return node;
}

void delete_fixup(bs_tree_pt tree,tree_node_pt node){
	while(node!=tree->root){
		tree_node_pt p=node->parent;
		if(node==p->left){
			p->bf++;
			node=p->right;
		}else{
			p->bf--;
			node=p->left;
		}
		if(p->bf==0){
			node=p;
			continue;
		}else if(p->bf==1||p->bf==-1) return;
		else{
			int bf=node->bf+p->bf;
			switch(bf){
			case 3:
				left_rotate_with_bf(tree,p);
				node=node->parent;
				continue;
			case 2:
				left_rotate_with_bf(tree,p);
				return;
			case -3:
				right_rotate_with_bf(tree,p);
				continue;
			case -2:
				right_rotate_with_bf(tree,p);
				return;
			case 1:
				rl_rotate_with_bf(tree,node);
				continue;
			case -1:
				lr_rotate_with_bf(tree,node);
				continue;
			}
		}
	}
}

void left_rotate_with_bf(bs_tree_pt tree,tree_node_pt p){
	if(p->right->bf==0){
		p->bf=1;
		p->right->bf=-1;
	}else{
		p->bf=0;
		p->right->bf=0;
	}
	left_rotate(tree,p);
}

void right_rotate_with_bf(bs_tree_pt tree,tree_node_pt p){
	if(p->left->bf==0){
		p->bf=-1;
		p->left->bf=1;
	}else{
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
