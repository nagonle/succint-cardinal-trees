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

using namespace std;
using namespace sdsl;

double getTime (void) {
	double usertime, systime;
	struct rusage usage;

	getrusage (RUSAGE_SELF, &usage);

	usertime = (double) usage.ru_utime.tv_sec +
		(double) usage.ru_utime.tv_usec / 1000000.0;

	systime = (double) usage.ru_stime.tv_sec +
		(double) usage.ru_stime.tv_usec / 1000000.0;

	return (usertime);
}

void print_index(size_t n){
	for(size_t i=0; i<n; i++) cout << i%10;
	cout << endl;
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
			//cout << "[Node " << 0 << "] child(" << 1 << ", " << j+1 << "): " << ct->preorder(ct->child(1, j+1)) << endl;
			cout << "[Node " << 0 << "] child(" << 1 << ", " << j+1 << "): " << ct->child(1, j+1) << endl;
	}

	for (size_t i=1; i<ct->count_nodes(); i++) {
		x = ct->tree_select0(i) + 1;
		for(size_t j=0; j<ct->degree(x); j++) {
			//cout << "[Node " << i << "] child(" << x << ", " << j+1 << "): " << ct->preorder(ct->child(x, j+1)) << endl;
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
	char alpha; 
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


template<class TYPE>
void test_label_reverse(TYPE * ct) {
	size_t x;
	for(size_t j=0; j<ct->degree(1); j++) {
			cout << "[Node " << 0 << "] label(" << 1 << ", " << j+1 << "): " << ct->label_reverse(1, j+1) << endl;
	}

	for (size_t i=1; i<ct->count_nodes(); i++) {
		x = ct->tree_select0(i) + 1;
		for(size_t j=0; j<ct->degree(x); j++) {
			cout << "[Node " << i << "] label(" << x << ", " << j+1 << "): " << ct->label_reverse(x, j+1) << endl;
		}
	}
}


template<class TYPE>
void test_label_child_reverse(TYPE * ct) {
	size_t x;
	char alpha; 
	size_t label_child_rev;
	for(size_t j=0; j<ct->degree(1); j++) {
		alpha = ct->label_reverse(1, j+1);
		label_child_rev = ct->label_child_reverse(1, alpha);
		cout << "[Node " << 0 << "] label_child(" << 1 << ", '" << alpha << "'): " 
			 << label_child_rev << " -> node " << ct->preorder(label_child_rev) << endl;

	}

	for (size_t i=1; i<ct->count_nodes(); i++) {
		x = ct->tree_select0(i) + 1;
		for(size_t j=0; j<ct->degree(x); j++) {
			alpha = ct->label_reverse(x, j+1);
			label_child_rev = ct->label_child_reverse(x, alpha);
			cout << "[Node " << i << "] label_child(" << x << ", '" << alpha << "'): "
		   		 << label_child_rev << " -> node " << ct->preorder(label_child_rev) << endl;
		}
	}
}
#endif
