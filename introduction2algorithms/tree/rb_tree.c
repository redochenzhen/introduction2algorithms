/*
 * rb_tree.c
 *
 *  Created on: 2014��4��1��
 *      Author: Coeus
 */

#include "contract.h"
#include "tree.h"
#include <stdlib.h>

/*
 * ��ɫ��ת�������ɫ�Ǻ�ɫ�����������κκڸ߸ı䣻
 * �����ɫ�Ǻ�ɫ����������ת����ӽڵ�ڸ߼�1���ֵܽڵ�ĺڸ߼�1
 */
static void left_rotate_with_color(bs_tree_pt tree,tree_node_pt node);
static void right_rotate_with_color(bs_tree_pt tree,tree_node_pt node);
static void insert_fixup(bs_tree_pt tree,tree_node_pt node);
static void delete_fixup(bs_tree_pt tree,tree_node_pt node);

tree_node_pt rb_new_node(int key){
	tree_node_pt node=(tree_node_pt)malloc(sizeof(tree_node_t));
	node->key=key;
	node->color=RB_RED;
	return node;
}

bs_tree_pt rb_new_tree(){
	bs_tree_pt tree=(bs_tree_pt)malloc(sizeof(bs_tree_t));
	tree->nil=rb_new_node(INT_MIN);
	tree->nil->color=RB_BLACK;
	tree->root=tree->nil;
	tree->type=TREE_RB;
	return tree;
}

void rb_reset_tree(bs_tree_pt tree,int keys[],int length){
	free_sub(tree,tree->root);
	if(length<=0) return;
	int i=0;
	for(;i<length;++i){
		rb_insert(tree,rb_new_node(keys[i]));
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
			}else if(node==node->parent->right){
				left_rotate(tree,node->parent);
				node=node->left;
			}else{
				right_rotate_with_color(tree,node->parent->parent);	//break;
			}
		}else{
			tree_node_pt uncle=node->parent->parent->left;
			if(uncle->color==RB_RED){
				node->parent->color=RB_BLACK;
				uncle->color=RB_BLACK;
				uncle->parent->color=RB_RED;
				node=uncle->parent;
			}else if(node==node->parent->left){
				right_rotate(tree,node->parent);
				node=node->right;
			}else{
				left_rotate_with_color(tree,node->parent->parent);	//break;
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
		//��ʱdel����ָ��nil������delete_node������֤nil->parent��ʱ������
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
			//��ʱ�Ѿ�ȷ��siblingΪ��ɫ
			if(sibling->left->color==RB_BLACK&&sibling->right->color==RB_BLACK){
				//ʹ��node->parent�ĺڸ߼�1���Ӷ������parent���޸�
				sibling->color=RB_RED;
				node=node->parent;
				continue;
			}else{
				if(sibling->right->color==RB_BLACK){
					right_rotate_with_color(tree,sibling);
					sibling=node->parent->right;
				}
				//��ʱ�Ѿ�ȷ��sibling->rightΪ��ɫ
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
			//��ʱ�Ѿ�ȷ��siblingΪ��ɫ
			if(sibling->right->color==RB_BLACK&&sibling->left->color==RB_BLACK){
				//ʹ��node->parent�ĺڸ߼�1���Ӷ������parent���޸�
				sibling->color=RB_RED;
				node=node->parent;
				continue;
			}else{
				if(sibling->left->color==RB_BLACK){
					left_rotate_with_color(tree,sibling);
					sibling=node->parent->left;
				}
				//��ʱ�Ѿ�ȷ��sibling->leftΪ��ɫ
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
