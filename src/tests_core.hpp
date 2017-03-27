#ifndef INCLUDED_TEST_CORE
#define INCLUDED_TEST_CORE

#include <iostream>
#include <string>
#include <sdsl/bit_vectors.hpp>
#include <sdsl/int_vector.hpp>
#include <sdsl/bp_support.hpp>
#include <sdsl/wavelet_trees.hpp>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <sys/time.h>
#include <sys/resource.h>
#include "cardinal_tree.hpp"
using namespace std;
using namespace sdsl;


// tester: Function that perform test on label_child operation.
template <class type, typename Size_Type>
double tester(type *ct, size_t N=100000, bool verbose=true) {
	chrono::high_resolution_clock::time_point start_time, end_time;
	chrono::duration<double> total_time;
	srand(0);
	size_t count, degree, node, ith_child, next_node;
	Size_Type label;
	count = degree = 0;
	// 1 is the root node.
	size_t root_degree = ct->degree(1);
	if (verbose) cout << "root_degree:" << root_degree << endl;
	node = 1;
	while (count < N) {
		if (degree == 0) {
			node = 1;
			degree = root_degree;
			label = '@';
		}
		ith_child = rand() % degree + 1;
		if (verbose) cout << "actual_node: " << node << " Preorder(actual_node): " << ct->preorder(node) << " degree(actual_node): " << ct->degree(node); 
		label = (Size_Type)ct->label(node, ith_child);
		if (verbose) cout << " New label: " << label << " to int: " << (int)label << "|" << ct->get_bp(node) << "|" << ct->get_bp(node-1) << endl;
		start_time = chrono::high_resolution_clock::now();
		next_node = ct->label_child(node, label);
		if (next_node == 0) {
			degree = 0;
			continue;
		}
		end_time = chrono::high_resolution_clock::now();
		total_time += end_time - start_time;

		node = next_node;
		degree = ct->degree(node);

		count++;
	}
	double time = chrono::duration_cast<chrono::microseconds>(total_time).count();
	return time/N;
};

#endif
