/*
 * tree.c
 *
 *  Created on: 2014年4月7日
 *      Author: coeus
 */

#include "tree.h"
#include <stdlib.h>

#ifdef _WIN32
BOOL is_tree_empty(bs_tree_cpt tree){
	return tree->root==tree->nil;
}
#else
bool is_tree_empty(bs_tree_cpt tree){
	return tree->root==tree->nil;
}
#endif

tree_node_pt new_node(elem_t satellite){
	tree_node_pt node=(tree_node_pt)malloc(sizeof(tree_node_t));
	node->satellite=satellite;
	return node;
}

void transplant_node(bs_tree_pt tree,tree_node_pt from,tree_node_pt to){
	if(to==tree->root){
		tree->root=from;
	}else if(to==to->parent->left){
		to->parent->left=from;
	}else{
		to->parent->right=from;
	}
	if(from!=NIL){
		//即使from是nil，nil->parent也暂时指向to->parent
		from->parent=to->parent;
	}
}

void left_rotate(bs_tree_pt tree,tree_node_pt node){
	tree_node_pt r;
	if(node->right==tree->nil) return;
	r=node->right;
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
	tree_node_pt l;
	if(node->left==tree->nil) return;
	l=node->left;
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

static void recursive_free_sub(bs_tree_pt tree,tree_node_pt sub_root){
	if(sub_root->left!=tree->nil){
		recursive_free_sub(tree,sub_root->left);
	}
	if(sub_root->right!=tree->nil){
		recursive_free_sub(tree,sub_root->right);
	}
	free(sub_root);
}

//free_subtree之后得到的是一颗空树，tree->nil可能还占空间
void free_subtree(bs_tree_pt tree,tree_node_pt sub_root){
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
	free_subtree(tree,tree->root);
	free(tree->nil);
	tree->nil=NIL;
	free(tree);
}
