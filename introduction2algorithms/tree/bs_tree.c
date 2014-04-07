/*
 * bs_tree.c
 *
 *  Created on: 2014年4月1日
 *      Author: Coeus
 */

#include "tree.h"
#include <stdlib.h>

static int default_compare(elem_t x,elem_t y);

bs_tree_pt bs_new_tree(compare_func_t compare){
	bs_tree_pt tree=(bs_tree_pt)malloc(sizeof(bs_tree_t));
	tree->nil=NIL;
	tree->root=NIL;
	tree->type=TREE_BS;
	tree->compare=compare!=NIL?compare:default_compare;
	return tree;
}

void bs_reset_tree(bs_tree_pt tree,elem_arr_t satellite_arr,int length){
	int i;
	free_subtree(tree,tree->root);
	if(length<=0) return;
	for(i=0;i<length;++i){
		bs_insert(tree,new_node(satellite_arr[i]));
	}
}

tree_node_pt bs_minimum_sub(bs_tree_cpt tree,tree_node_pt sub_root){
	tree_node_pt min=sub_root;
	while(min->left!=tree->nil){
		min=min->left;
	}
	return min;
}

tree_node_pt bs_maximum_sub(bs_tree_cpt tree,tree_node_pt sub_root){
	tree_node_pt max=sub_root;
	while(max->right!=tree->nil){
		max=max->right;
	}
	return max;
}

tree_node_pt bs_successor(bs_tree_cpt tree,tree_node_pt node){
	if(node->right!=tree->nil){
		return bs_minimum_sub(tree,node->right);
	}
	while(node!=tree->root&&node==node->parent->right){
		node=node->parent;
	}
	return node->parent;
}

tree_node_pt bs_predecessor(bs_tree_cpt tree,tree_node_pt node){
	if(node->left!=tree->nil){
		return bs_maximum_sub(tree,node->left);
	}
	while(node!=tree->root&&node==node->parent->left){
		node=node->parent;
	}
	return node->parent;
}

void bs_insert(bs_tree_pt tree,tree_node_pt node){
	int flag;
	tree_node_pt x=tree->root;
	tree_node_pt y=tree->nil;
	while(x!=tree->nil){
		y=x;
		flag=tree->compare(node->satellite,x->satellite);
		if(flag<0){
			x=x->left;
		}else{
			x=x->right;
		}
	}
	node->parent=y;
	//插入之前tree是空树
	if(y==tree->nil){
		tree->root=node;
	}else if(flag<0){
		y->left=node;
	}else{
		y->right=node;
	}
	//插入是外来节点跟树发生关系的唯一入口，这里保证了新节点的left和right指向nil
	node->left=tree->nil;
	node->right=tree->nil;
}

tree_node_pt bs_delete(bs_tree_pt tree,const tree_node_pt node){
	if(node->left==tree->nil){
		transplant_node(tree,node->right,node);
	}else if(node->right==tree->nil){
		transplant_node(tree,node->left,node);
	}else{
		tree_node_pt min=bs_minimum_sub(tree,node->right);
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

tree_node_pt bs_search(bs_tree_cpt tree,elem_t satellite){
	return bs_search_sub(tree,tree->root,satellite);
}

tree_node_pt bs_search_sub(bs_tree_cpt tree,tree_node_pt sub_root,elem_t satellite){
	int flag;
	while(sub_root!=tree->nil){
		flag=tree->compare(satellite,sub_root->satellite);
		if(flag==0) break;
		if(flag<0){
			sub_root=sub_root->left;
		}else{
			sub_root=sub_root->right;
		}
	}
	if(sub_root==tree->nil){
		return NIL;
	}
	return sub_root;
}

//默认将satellite当作long int来比较
int default_compare(elem_t x,elem_t y){
	long int n=(long int)x-(long int)y;
	if(n>0L) return 1;
	if(n<0L) return -1;
	return 0;
}

