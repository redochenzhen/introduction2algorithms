//============================================================================
// Name        : gtest4introduction2algorithms.cc
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

//#define TREE_TEST
//#define DP_TEST
#define HASH_TEST

#include <gtest/gtest.h>
#include "tree_test.h"
#include "dp_greedy_test.h"
#include "hashing_test.h"

int main(int argc,char* argv[]) {
	init_tree_test();
	init_hashing_test();

	testing::InitGoogleTest(&argc,argv);
	int result=RUN_ALL_TESTS();

	cleanup_tree_test();
	cleanup_hash_test();
	return result;
}
