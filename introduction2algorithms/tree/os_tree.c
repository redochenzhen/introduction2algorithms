/*
 * os_tree.c
 *
 *  Created on: 2014年4月6日
 *      Author: Coeus
 */

#include "tree.h"
#include <stdlib.h>

static void left_rotate_with_size(bs_tree_pt tree,tree_node_pt node);
static void right_rotate_with_size(bs_tree_pt tree,tree_node_pt node);
static void left_rotate_with_color_size(bs_tree_pt tree,tree_node_pt node);
static void right_rotate_with_color_size(bs_tree_pt tree,tree_node_pt node);
static void insert_fixup(bs_tree_pt tree,tree_node_pt node);
static void delete_fixup(bs_tree_pt tree,tree_node_pt node);
static void os_insert_node(bs_tree_pt tree,tree_node_pt node);

tree_node_pt os_new_node(elem_t satellite){
	tree_node_pt node=rb_new_node(satellite);
	node->size=1;
	return node;
}

bs_tree_pt os_new_tree(compare_func_t compare){
	bs_tree_pt tree=rb_new_tree(compare);
	tree->nil->size=0;
	tree->type=TREE_OS;
	return tree;
}

void os_reset_tree(bs_tree_pt tree,elem_arr_t satellite_arr,longsize_t length){
	longsize_t i;
	free_subtree(tree,tree->root);
	if(length<=0) return;
	for(i=0;i<length;++i){
		os_insert(tree,os_new_node(satellite_arr[i]));
	}
}

void os_insert(bs_tree_pt tree,tree_node_pt node){
	os_insert_node(tree,node);
	node->color=RB_RED;
	insert_fixup(tree,node);
}

void os_insert_node(bs_tree_pt tree,tree_node_pt node){
	tree_node_pt x=tree->root;
	tree_node_pt y=tree->nil;
	compare_func_t cmp=tree->compare;
	while(x!=tree->nil){
		y=x;
		x->size++;	//除了这一行代码，其它完全与insert_node相同
		if(cmp(node->satellite,x->satellite)<0){
			x=x->left;
		}else{
			x=x->right;
		}
	}
	node->parent=y;
	if(y==tree->nil){
		tree->root=node;
	}else if(cmp(node->satellite,y->satellite)<0){
		y->left=node;
	}else{
		y->right=node;
	}
	//插入是外来节点跟树发生关系的唯一入口，这里保证了新节点的left和right指向nil
	node->left=tree->nil;
	node->right=tree->nil;
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
				left_rotate_with_size(tree,node->parent);
				node=node->left;
				continue;
			}else{
				right_rotate_with_color_size(tree,node->parent->parent);
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
				right_rotate_with_size(tree,node->parent);
				node=node->right;
				continue;
			}else{
				left_rotate_with_color_size(tree,node->parent->parent);
				break;
			}
		}
	}
	tree->root->color=RB_BLACK;
}

tree_node_pt os_delete(bs_tree_pt tree,const tree_node_pt node){
	/*
	 * 跟插入不同，不能简单调用delete_node
	 */
	tree_node_pt d;
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
			//防止del->right为nil时无法向上追溯
			del->right->parent=del;
		}
		transplant_node(tree,del,node);
		del->left=node->left;
		del->left->parent=del;
		del=del->right;
	}
	d=del->parent;
	while(d!=tree->nil){
		d->size--;
		d=d->parent;
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
				left_rotate_with_color_size(tree,node->parent);
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
					right_rotate_with_color_size(tree,sibling);
					sibling=node->parent->right;
				}
				//此时已经确保sibling->right为红色
				sibling->color=RB_BLACK;
				left_rotate_with_color_size(tree,node->parent);
				return;
			}
		}else{
			//以下代码是上面情形的镜像
			tree_node_pt sibling=node->parent->left;
			if(sibling->color==RB_RED){
				right_rotate_with_color_size(tree,node->parent);
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
					left_rotate_with_color_size(tree,sibling);
					sibling=node->parent->left;
				}
				//此时已经确保sibling->left为红色
				sibling->color=RB_BLACK;
				right_rotate_with_color_size(tree,node->parent);
				return;
			}
		}
	}
	node->color=RB_BLACK;
}

void left_rotate_with_color_size(bs_tree_pt tree,tree_node_pt node){
	rb_color_t left_color=node->color;
	node->color=node->right->color;
	node->right->color=left_color;
	left_rotate_with_size(tree,node);
}

void right_rotate_with_color_size(bs_tree_pt tree,tree_node_pt node){
	rb_color_t right_color=node->color;
	node->color=node->left->color;
	node->left->color=right_color;
	right_rotate_with_size(tree,node);
}

void left_rotate_with_size(bs_tree_pt tree,tree_node_pt node){
	node->right->size=node->size;
	node->size=node->size-node->right->right->size-1;
	left_rotate(tree,node);
}

void right_rotate_with_size(bs_tree_pt tree,tree_node_pt node){
	node->left->size=node->size;
	node->size=node->size-node->left->left->size-1;
	right_rotate(tree,node);
}


tree_node_pt os_select(bs_tree_pt tree,long int index){
	long int i;
	tree_node_pt node;
	if(index>=tree->root->size){
		return NIL;
	}
	node=tree->root;
	i=node->left->size;
	while(i!=index){
		if(index<i){
			node=node->left;
			i--;
			i-=node->right->size;
		}else{
			node=node->right;
			i++;
			i+=node->left->size;
		}
	}
	return node;
}

long int os_rank(bs_tree_pt tree,elem_t satellite){
	/*
	 * 比较版本1和版本2很有意思，两者都体现了一种维持
	 * “不变量”的编程思想，但分别选择了不同的“不变量”
	 */
	//版本1
	long int i=0;
	tree_node_pt node=tree->root;
	int flag;
	while(node!=tree->nil){
		flag=tree->compare(satellite,node->satellite);
		if(flag<0){
			node=node->left;
		}else{
			i+=node->left->size;
			if(flag==0) break;
			node=node->right;
			i++;
		}
	}
	if(node==tree->nil){
		return -1;
	}
	return i;

//	//版本2
//	int flag;
//	long int i;
//	tree_node_pt node=tree->root;
//	if(node==tree->nil){
//		return -1;
//	}
//	i=node->left->size;
//	for(;;){
//		flag=tree->compare(satellite,node->satellite);
//		if(flag==0) break;
//		else if(flag<0){
//			node=node->left;
//			if(node==tree->nil){
//				return -1;
//			}
//			i-=node->right->size+1;
//		}else{
//			node=node->right;
//			if(node==tree->nil){
//				return -1;
//			}
//			i+=node->left->size+1;
//		}
//	}
//	return i;

//	//版本3
//	tree_node_pt node=search_tree(tree,satellite);
//	return os_rank_node(tree,node);
}

long int os_rank_node(bs_tree_pt tree,tree_node_pt node){
	long int i;
	if(node==NIL||node==tree->nil){
		return -1;
	}
	i=node->left->size;
	while(node!=tree->root){
		if(node==node->parent->right){
			i+=node->parent->left->size+1;
		}
		node=node->parent;
	}
	return i;
}

