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


template<class TYPE>
double test_label_child(TYPE * ct) {
	size_t x;
	uint8_t alpha; 
	size_t label_child_t;
	size_t N = 0;
	chrono::high_resolution_clock::time_point start_time, end_time;
	chrono::duration<double> total_time; 
	for(size_t j=0; j<ct->degree(1); j++) {
		alpha = ct->label(1, j+1);
		start_time = chrono::high_resolution_clock::now();
		label_child_t = ct->label_child(1, alpha);
		end_time = chrono::high_resolution_clock::now();
		total_time += end_time - start_time;
		N++;
	}

	for (size_t i=1; i<ct->count_nodes(); i++) {
		x = ct->tree_select0(i) + 1;
		for(size_t j=0; j<ct->degree(x); j++) {
			alpha = ct->label(x, j+1);
			start_time = chrono::high_resolution_clock::now();
			label_child_t = ct->label_child(x, alpha);
			end_time = chrono::high_resolution_clock::now();
			total_time += end_time - start_time;
			N++;
		}
	}
	double time = chrono::duration_cast<chrono::microseconds>(total_time).count();
	return time/N;
}

template<class TYPE>
double new_test_label_child(TYPE * ct, size_t N=300000) {
	uint8_t alpha; 
	size_t label_child_t;
	size_t total_nodes =  ct->get_bp_count() / 2;

	chrono::high_resolution_clock::time_point start_time, end_time;
	chrono::duration<double> total_time; 


	size_t jump = 100;
	size_t node = 1;
	size_t queries = 0;
	size_t degree;
	size_t x = 0;
	size_t begin = 1;
	while (queries < N) {
		if (x > total_nodes) {
			begin++;
			x = begin;
		}
		degree = ct->degree(node);
		while (degree < 1) {
			x += jump;
			node = ct->tree_select0(x) + 1;
			degree = ct->degree(node);
		}

		//alpha = ct->label(node, 1);
		alpha = ct->label(node, degree);
		start_time = chrono::high_resolution_clock::now();
		label_child_t = ct->label_child(node, alpha);
		end_time = chrono::high_resolution_clock::now();
		total_time += end_time - start_time;

		x+=jump;
		node = ct->tree_select0(x) + 1;
		queries++;
	}
	double time = chrono::duration_cast<chrono::microseconds>(total_time).count();
	return time/N;
}

template<class type>
double tester(type *ct, size_t N=300000, bool verbose=false) {
	chrono::high_resolution_clock::time_point start_time, end_time;
	chrono::duration<double> total_time;
	size_t count, degree, node, ith_child, next_node;
	uint8_t label;
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
		label = (uint8_t)ct->label(node, ith_child);
		if (verbose) cout << " New label: " << label << " to int: " << (int)label << endl;
		start_time = chrono::high_resolution_clock::now();
		next_node = ct->label_child(node, label);
		end_time = chrono::high_resolution_clock::now();
		total_time += end_time - start_time;

		node = next_node;
		degree = ct->degree(node);

		count++;
	}
	double time = chrono::duration_cast<chrono::microseconds>(total_time).count();
	return time/N;
}

#endif
