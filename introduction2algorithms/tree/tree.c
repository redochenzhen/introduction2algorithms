/*
 * tree.c
 *
 *  Created on: 2014年4月1日
 *      Author: Coeus
 */

#include "contract.h"
#include "tree.h"
#include <stdlib.h>

tree_node_pt new_node(int key){
	tree_node_pt node=(tree_node_pt)malloc(sizeof(tree_node_t));
	node->key=key;
	return node;
}

bs_tree_pt new_tree(){
	bs_tree_pt tree=(bs_tree_pt)malloc(sizeof(bs_tree_t));
	tree->nil=NIL;
	tree->root=NIL;
	tree->type=TREE_BS;
	return tree;
}

void reset_tree(bs_tree_pt tree,int keys[],int length){
	free_sub(tree,tree->root);
	if(length<=0) return;
	int i=0;
	for(;i<length;++i){
		insert_node(tree,new_node(keys[i]));
	}
}

tree_node_pt minimum_sub(bs_tree_cpt tree,tree_node_pt sub_root){
	tree_node_pt min=sub_root;
	while(min->left!=tree->nil){
		min=min->left;
	}
	return min;
}

tree_node_pt maximum_sub(bs_tree_cpt tree,tree_node_pt sub_root){
	tree_node_pt max=sub_root;
	while(max->right!=tree->nil){
		max=max->right;
	}
	return max;
}

tree_node_pt successor(bs_tree_cpt tree,tree_node_pt node){
	if(node->right!=tree->nil){
		return minimum_sub(tree,node->right);
	}
	while(node!=tree->root&&node==node->parent->right){
		node=node->parent;
	}
	return node->parent;
}

tree_node_pt predecessor(bs_tree_cpt tree,tree_node_pt node){
	if(node->left!=tree->nil){
		return maximum_sub(tree,node->left);
	}
	while(node!=tree->root&&node==node->parent->left){
		node=node->parent;
	}
	return node->parent;
}

void insert_node(bs_tree_pt tree,tree_node_pt node){
	tree_node_pt x=tree->root;
	tree_node_pt y=tree->nil;
	while(x!=tree->nil){
		y=x;
		if(node->key<x->key){
			x=x->left;
		}else{
			x=x->right;
		}
	}
	node->parent=y;
	if(y==tree->nil){				//插入之前tree是空树
		tree->root=node;
	}else if(node->key<y->key){
		y->left=node;
	}else{
		y->right=node;
	}
	//插入是外来节点跟树发生关系的唯一入口，这里保证了新节点的left和right指向nil
	node->left=tree->nil;
	node->right=tree->nil;
}

void transplant_node(bs_tree_pt tree,tree_node_pt from,tree_node_pt to){
	if(to->parent==tree->nil){
		tree->root=from;
	}else if(to==to->parent->left){
		to->parent->left=from;
	}else{
		to->parent->right=from;
	}
	if(from!=NIL){
		//在红黑树中，即使from是nil，nil->parent也暂时指向to->parent
		from->parent=to->parent;
	}
}

tree_node_pt delete_node(bs_tree_pt tree,const tree_node_pt node){
	if(node->left==tree->nil){
		transplant_node(tree,node->right,node);
	}else if(node->right==tree->nil){
		transplant_node(tree,node->left,node);
	}else{
		tree_node_pt min=minimum_sub(tree,node->right);
		//对右子树最小节点不是右节点的情况进行转化
		if(node!=min->parent){
			transplant_node(tree,min->right,min);
			min->right=node->right;
			min->right->parent=min;
		}
		transplant_node(tree,min,node);
		min->left=node->left;
		min->left->parent=min;
	}
	node->parent=NIL;
	node->left=NIL;
	node->right=NIL;
	return node;
}

bool is_tree_empty(bs_tree_cpt tree){
	return tree->root==tree->nil;
}

static void recursive_free_sub(bs_tree_pt tree,tree_node_pt sub_root){
	if(sub_root->left!=tree->nil){
		recursive_free_sub(tree,sub_root->left);
	}
	if(sub_root->right!=tree->nil){
		recursive_free_sub(tree,sub_root->right);
	}
	free(sub_root);
	sub_root=NIL;
}

void free_sub(bs_tree_pt tree,tree_node_pt sub_root){
	if(sub_root==tree->nil) return;
	if(sub_root==tree->root){
		tree->root=tree->nil;
	}else if(sub_root==sub_root->parent->left){
		sub_root->parent->left=tree->nil;
	}else{
		sub_root->parent->right=tree->nil;
	}
	recursive_free_sub(tree,sub_root);
}

void free_tree(bs_tree_pt tree){
	free_sub(tree,tree->root);
	free(tree->nil);
	tree->nil=NIL;
	free(tree);
	tree=NIL;
}

void left_rotate(bs_tree_pt tree,tree_node_pt node){
	if(node->right==tree->nil) return;
	tree_node_pt r=node->right;
	node->right=r->left;
	if(r->left!=tree->nil){
		r->left->parent=node;
	}
	r->parent=node->parent;
	//旋转跟节点后需要修复tree->root
	if(node==tree->root){
		tree->root=r;
	}else if(node==node->parent->left){
		node->parent->left=r;
	}else{
		node->parent->right=r;
	}
	node->parent=r;
	r->left=node;
}

void right_rotate(bs_tree_pt tree,tree_node_pt node){
	if(node->left==tree->nil) return;
	tree_node_pt l=node->left;
	node->left=l->right;
	if(l->right!=tree->nil){
		l->right->parent=node;
	}
	l->parent=node->parent;
	if(node==tree->root){
		tree->root=l;
	}else if(node==node->parent->right){
		node->parent->right=l;
	}else{
		node->parent->left=l;
	}
	node->parent=l;
	l->right=node;
}

void inorder_walk(bs_tree_pt tree,node_predicate_t func){
	inorder_walk_sub(tree,tree->root,func);
}

void inorder_walk_sub(bs_tree_pt tree,tree_node_pt sub_root,node_predicate_t func){
	if(sub_root==tree->nil) return;
	inorder_walk_sub(tree,sub_root->left,func);
	func(sub_root);
	inorder_walk_sub(tree,sub_root->right,func);
}

void preorder_walk(bs_tree_pt tree,node_predicate_t func){
	preorder_walk_sub(tree,tree->root,func);
}
void preorder_walk_sub(bs_tree_pt tree,tree_node_pt sub_root,node_predicate_t func){
	if(sub_root==tree->nil) return;
	func(sub_root);
	preorder_walk_sub(tree,sub_root->left,func);
	preorder_walk_sub(tree,sub_root->right,func);
}
void postorder_walk(bs_tree_pt tree,node_predicate_t func){
	postorder_walk_sub(tree,tree->root,func);
}
void postorder_walk_sub(bs_tree_pt tree,tree_node_pt sub_root,node_predicate_t func){
	if(sub_root==tree->nil) return;
	postorder_walk_sub(tree,sub_root->left,func);
	postorder_walk_sub(tree,sub_root->right,func);
	func(sub_root);
}

tree_node_pt search_key(bs_tree_cpt tree,int key){
	return search_key_sub(tree,tree->root,key);
}

tree_node_pt search_key_sub(bs_tree_cpt tree,tree_node_pt sub_root,int key){
	while(sub_root!=tree->nil&&sub_root->key!=key){
		if(key<sub_root->key){
			sub_root=sub_root->left;
		}else{
			sub_root=sub_root->right;
		}
	}
	return sub_root;
}
