//============================================================================
// Name        : gtest4introduction2algorithms.cc
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

//#define TREE_TEST
#define DP_TEST

#include <gtest/gtest.h>
#include "tree_test.h"
#include "dp_greedy_test.h"

int main(int argc,char* argv[]) {
	testing::InitGoogleTest(&argc,argv);
	return RUN_ALL_TESTS();
	return 0;
}
