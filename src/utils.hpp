#ifndef UTILS_INCLUDE
#define UTILS_INCLUDE
#include <iostream>
#include <sdsl/bit_vectors.hpp>
#include <sdsl/bp_support.hpp>
#include <sdsl/wavelet_trees.hpp>
#include <vector>
#include <sys/time.h>
#include <sys/resource.h>
#include "cardinal_tree.hpp"
#include <algorithm>
#include <numeric>

using namespace std;
using namespace sdsl;

// max_arity: return the arity of a tree.
template <class type>
size_t get_max_arity(type * ct) {
	size_t x;
	size_t max_arity;
	max_arity = ct->degree(1);
	for (size_t i=1; i<ct->count_nodes(); i++) {
		x = ct->tree_select0(i) + 1;
		if (ct->degree(x) > max_arity) {
			max_arity = ct->degree(x);
		}
	}
	return max_arity;
}

template <class type>
double get_average_arity(type * ct) {
	size_t x;
	size_t degree_node = ct->degree(1);
	double sum_arity = degree_node;
	size_t nodes = 1;
	for (size_t i=1; i<ct->count_nodes(); i++) {
		x = ct->tree_select0(i) + 1;
		degree_node = ct->degree(x);
		if (degree_node > 0) {
			sum_arity += degree_node;
			nodes++;
		}
	}
	return sum_arity / nodes;
}


// tree_height: return the max tree height.
template <class type>
size_t get_tree_height(type * ct, int& count,size_t node=1, size_t height=0) {
	count++;
	vector <size_t> height_list;
	size_t node_degree = ct->degree(node);
	size_t new_node;
	if (node_degree == 0) return height;
	else {
		height++;
		for (size_t i=0; i<node_degree; i++) {
			new_node = ct->child(node, i+1);
			height_list.push_back(get_tree_height(ct, count, new_node, height));
		}
	}
	return *max_element(height_list.begin(), height_list.end());
}

// average_tree_height: return the average tree height.
template <class type>
double get_tree_average_height(type * ct, double* sum_heights, double* count_nodes, size_t node=1, size_t height=0) {
	size_t node_degree = ct->degree(node);
	size_t new_node;
	if (node_degree == 0) {
		*sum_heights += height;
		(*count_nodes)++;
		return height;
	}
	else {
		height++;
		for (size_t i=0; i<node_degree; i++) {
			new_node = ct->child(node, i+1);
			get_tree_average_height(ct, sum_heights, count_nodes, new_node, height);
		}
	}
	return *sum_heights / *count_nodes;
}

// Function that run along the tree, and apply the methods in each node.
template<class TYPE>
void test_degree(TYPE * ct) {
	size_t x;
	cout << "[Node 0] degree(1):" << ct->degree(1) << endl;
	for (size_t i=1; i<ct->count_nodes(); i++) {
		x = ct->tree_select0(i) + 1;
		cout << "[Node " << i << "] degree(" << x << "):" << ct->degree(x) << endl;
	}
}

void brute_test_degree(bit_vector *ct) {
	size_t node = 0, degree = 0;
	for (size_t i=1; i<ct->size(); i++) {
		if ((*ct)[i] != 1u){
			cout << "[Node " << node << "] degree(" << i-degree << "):" << degree << endl;
			node++;
			degree = 0;
		} else {
			degree++;
		}

	}
}

template<class TYPE>
void test_parent(TYPE * ct) {
	size_t x;
	cout << "[Node 0] parent(1):" << ct->parent(1) << endl;
	for (size_t i=2; i<ct->count_nodes(); i++) {
		x = ct->tree_select0(i) + 1;
		cout << "[Node " << i << "] parent(" << x << "):" << ct->parent(x) << endl;
	}
}

template<class TYPE>
void test_child(TYPE * ct) {
	size_t x;
	for(size_t j=0; j<ct->degree(1); j++) {
			cout << "[Node " << 0 << "] child(" << 1 << ", " << j+1 << "): " << ct->child(1, j+1) << endl;
	}

	for (size_t i=1; i<ct->count_nodes(); i++) {
		x = ct->tree_select0(i) + 1;
		for(size_t j=0; j<ct->degree(x); j++) {
			cout << "[Node " << i << "] child(" << x << ", " << j+1 << "): " << ct->child(x, j+1) << endl;
		}
	}
}

template<class TYPE>
void test_preorder(TYPE * ct) {
	size_t x;
	cout << "[Node 0] preorder(1):" << ct->preorder(1) << endl;
	for (size_t i=1; i<ct->count_nodes(); i++) {
		x = ct->tree_select0(i) + 1;
		cout << "[Node " << i << "] preorder(" << x << "):" << ct->preorder(x) << endl;
	}
}

template<class TYPE>
void test_child_rank(TYPE * ct) {
	size_t x;
	cout << "[Node 0] child_rank(1):" << ct->child_rank(1) << endl;
	for (size_t i=1; i<ct->count_nodes(); i++) {
		x = ct->tree_select0(i) + 1;
		cout << "[Node " << i << "] child_rank(" << x << "):" << ct->child_rank(x) << endl;
	}
}

template<class TYPE>
void test_select_node(TYPE * ct) {
	for (size_t i=0; i<ct->count_nodes(); i++) {
		cout << "[Node " << i << "] select_node(" << i << "):" << ct->select_node(i) << endl;
	}
}


template<class TYPE>
void test_subtree_size(TYPE * ct) {
	size_t x;
	cout << "[Node 0] preorder(1):" << ct->subtree_size(1) << endl;
	for (size_t i=1; i<ct->count_nodes(); i++) {
		x = ct->tree_select0(i) + 1;
		cout << "[Node " << i << "] subtree_size(" << x << "):" << ct->subtree_size(x) << endl;
	}
}


template<class TYPE>
void test_access_data(TYPE * ct) {
	size_t x;
	cout << "[Node 0] access_data(1): " << ct->access_data(1) << endl;
	for (size_t i=1; i<ct->count_nodes(); i++) {
		x = ct->tree_select0(i) + 1;
		cout << "[Node " << i << "] access_data(" << x << "): " << ct->access_data(x) << endl;
	}
}


template<class TYPE>
void test_label(TYPE * ct) {
	size_t x;
	for(size_t j=0; j<ct->degree(1); j++) {
			cout << "[Node " << 0 << "] label(" << 1 << ", " << j+1 << "): " << ct->label(1, j+1) << endl;
	}

	for (size_t i=1; i<ct->count_nodes(); i++) {
		x = ct->tree_select0(i) + 1;
		for(size_t j=0; j<ct->degree(x); j++) {
			cout << "[Node " << i << "] label(" << x << ", " << j+1 << "): " << ct->label(x, j+1) << endl;
		}
	}
}

template<class TYPE>
void test_label_child(TYPE * ct) {
	size_t x;
	uint8_t alpha;
	size_t label_child_t;
	for(size_t j=0; j<ct->degree(1); j++) {
		alpha = ct->label(1, j+1);
		label_child_t = ct->label_child(1, alpha);
		cout << "[Node " << 0 << "] label_child(" << 1 << ", '" << alpha << "'): "
				<< label_child_t << "--> node " << ct->preorder(label_child_t)<< endl;

	}

	for (size_t i=1; i<ct->count_nodes(); i++) {
		x = ct->tree_select0(i) + 1;
		for(size_t j=0; j<ct->degree(x); j++) {
			alpha = ct->label(x, j+1);
			label_child_t = ct->label_child(x, alpha);
			cout << "[Node " << i << "] label_child(" << x << ", '" << alpha << "'): "
				<< label_child_t << "--> node " << ct->preorder(label_child_t) << endl;
		}
	}
}

#endif
