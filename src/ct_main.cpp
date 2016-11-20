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
#include "utils.hpp"
#include "read_utils.hpp"
#include "cardinal_tree_bs.hpp"
#include "cardinal_tree_ls.hpp"
using namespace std;
using namespace sdsl;

typedef unsigned char uchar;


template<class type>
double tester(type *ct, size_t N=100000, bool verbose=false) {
	//double total_time, time;
	chrono::high_resolution_clock::time_point start_time, end_time;
	chrono::duration<double> total_time;
	size_t count, degree, node, ith_child, next_node;
	//char label;
	uint8_t label;
	cout << ct->get_letts_size() << "|";
	cout << ct->get_bp_size() << "|"; 
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
		//cout << "[DEBUG] ith_child_random: " << ith_child << " ";
		//cout << "p: " << ct->get_bp(node-2) << " " << ct->get_bp(node-1) << " " << ct->get_bp(node) << " " << ct->get_bp(node+1) << " " << ct->get_bp(node+2) << endl;
		if (verbose) cout << "actual_node: " << node << " Preorder(actual_node): " << ct->preorder(node) << " degree(actual_node): " << ct->degree(node); 
		//cout << "[DEBUG] "<< " label: " << label << endl;
		label = (uint8_t)ct->label(node, ith_child);
		if (verbose) cout << " New label: " << label << " to int: " << (int)label << endl;
		start_time = chrono::high_resolution_clock::now();
		next_node = ct->label_child(node, label);
		//cout << "next_node: " << next_node << endl; 
		//cout << endl << endl;
		end_time = chrono::high_resolution_clock::now();
		total_time += end_time - start_time;

		node = next_node;
		degree = ct->degree(node);

		count++;
	}
	cout << "N:"<<N<<"|";
	double time = chrono::duration_cast<chrono::microseconds>(total_time).count();
	return time/N;
}

int main_test();
void process_data(char *name_bp, char *name_letts, char *type_wt);

void print_usage() {
	cout << "\tCardinal Trees" << endl;
	cout << "Usage:" << endl;
	cout << "\tct_main {type structure} {name file bp} {name file letts}" << endl;
	cout << "Example:" << endl;
	cout << "\tct_main gmr path/to/file.bp path/to/file.letts" << endl;
	cout << "\tct_main gmr path/to/prefix_file" << endl;
	cout << "\tprefix_file.ascii and prefix_file.letts must be exists." << endl << endl;
}

int main(int argc, char *argv[]) {
	char *name_bp, *name_letts, *name_size, *type_wt;

	if (argc == 4) {
		name_bp = (char*)malloc(strlen(argv[2]));
	   	strcpy(name_bp, argv[2]);

	   	name_letts = (char*)malloc(strlen(argv[3]));
	   	strcpy(name_letts, argv[3]);

	   	type_wt = (char*)malloc(strlen(argv[1]));
	   	strcpy(type_wt, argv[1]);
	} else if (argc == 3) {
		name_bp = (char*)malloc(strlen(argv[2])+6);
		strcpy(name_bp, argv[2]);
		strcat(name_bp, ".ascii");

		name_letts = (char*)malloc(strlen(argv[2])+6);
		strcpy(name_letts, argv[2]);
		strcat(name_letts, ".letts");

		type_wt = (char*)malloc(strlen(argv[1]));
	   	strcpy(type_wt, argv[1]);
	} else if (argc == 1) {
		main_test();
		return 0;
	} else if (strcmp(argv[2], "-h") == 0) {
		print_usage();
	}
	process_data(name_bp, name_letts, type_wt);
	return 0;
}

void print_output(string structure, char *name_letts, bool check_data, size_t total_nodes, size_t bp_size, double time, size_t voc_size) {
	//cout << "########## TEST [" << structure << "] on [" << name_letts << "] ##########" << endl;
	//cout << "[DEBUG] Check data: " << check_data << endl;
	//cout << "[Tree Info] Total nodes: " << total_nodes << endl;
	//cout << "[Tree Info] Size parentheses: " << bp_size << endl;
	//cout << "[" << name_letts << "][" << structure << "][Time][" << time << "][microseconds]" << "[Voc Size][" << voc_size << "]" << endl;
	cout << name_letts << "|" << structure << "|" << time << "|" << voc_size << "|";
}

bool count_parentheses(bit_vector *bp) {
	size_t open = 0, close = 0;
	bit_vector values = {0, 1};
	for (size_t i=0; i<bp->size(); i++) {
		if ((*bp)[i] == 1u) open++;
		else if ((*bp)[i] == 0u) close++;
		if (open < close){
			cout << "open: " << open << endl;
			cout << "close: " << close << endl;
			return false;
		}
	}
	cout << "open: " << open << endl;
	cout << "close: " << close << endl;
	if (open == close) return true;
	return false;

}


void process_data(char *name_bp, char *name_letts, char *type_wt) {
	char *bp;
	uint8_t *letts;
	size_t voc_size;
	// Get Data.
	// Read total_nodes and symbols.
	uint total_nodes = read_letts(name_letts, &letts);
	uint total_symbols = total_nodes - 1;
	//cout << "[DEBUG][prepare_data] total_nodes: " << total_nodes << endl;

	// Read sequence of parentheses.
	read_bp(name_bp, &bp, total_nodes);
	//cout << "[DEBUG][prepare_data] size parentheses: " << strlen(bp) << endl;
	//cout << "[DEBUG][prepare_data] bp sequence 0-21: "; for (size_t i=0; i<22; i++) cout << bp[i] << " "; cout << endl;

	bit_vector b(total_nodes*2);
	bp_string_to_bit_vector(bp, &b);
	//cout << "[DEBUG] count_parentheses(b): " << count_parentheses(&b) << endl;
	//cout << "[DEBUG] check_balanced(b): " << check_balanced(&b) << endl;
	
	//cout << "[DEBUG][prepare_data] bit_vector.size(): " << b.size() << endl;
	//cout << "[DEBUG][prepare_data] Check balanced parentheses: " << check_balanced(bp) << endl << endl;


	// Create Cardinal Tree.
	vector<int> info;
	info.push_back(42);

	/*for (size_t i=0; i<total_nodes-1; i++) {
		if (letts[i] == 0) letts[i] = (char)1;
	}
	letts[total_nodes-1] = '\0';*/
 	replace_null(letts, (char)1, total_symbols);

	string letts2;
	letts2.reserve(total_nodes);
	letts2 = reinterpret_cast<char*>(letts);
	voc_size = vocabulary_size(letts2);
	if (check_data(&b, letts2, total_nodes) == true) cout << "OK|";
	else cout << "FAILED|"; 

	int_vector<> my_vector(total_symbols, 0, 8);
	for (int i=0; i<total_symbols; i++) my_vector[i] = letts[i];

	//cout << "label root: ";
	//for (int i=0; i<16; i++) cout << letts2[i]; cout << endl;
	//for (size_t i=0; i<209; i++) cout << i << ": " << (int)letts2[i] << endl;
	//cout << "[DEBUG][prepare_data] letts 0-11: "; for (size_t i=0; i<11; i++) cout << letts2[i] << " "; cout << endl;
	//cout << "[DEBUG][prepare_data] letts.capacity(): " << letts2.capacity() << endl;
	//cout << "[DEBUG][prepare_data] letts.size(): " << letts2.size() << endl;
	
	//cardinal_tree<wt_gmr<>> ct(letts2, &b, &info);
	//cardinal_tree_bs ct(letts2, &b, &info);
	
	//cout << "[DEBUG][prepare_data] Creation Cardinal Tree Successful." << endl;
	string name;
	double time;
	if (strcmp(type_wt, "gmr") == 0) {
		name = "Golynski";
		cardinal_tree<wt_gmr<>> ct(my_vector, &b, &info);

		//cout << "bp.size(): " << b.size() << endl;
		//cout << "tree_rank0(60272746): " << ct.tree_rank0(b.size()-1) << endl;
		//cout << "tree_rank1(60272746): " << ct.tree_rank1(b.size()-1) << endl;

		//cout << "letts2.size(): " << letts2.size() << endl;
		//cout << "get_letts_size(): " << ct.get_letts_size() << endl;
		//cout << "NEXT LINE" << endl;
		//cout << "label_rank(A): " << ct.label_rank(30136371, 'A') << endl;
		//cout << "label_rank(G): " << ct.label_rank(30136371, 'G') << endl;
		//cout << "label_rank(T): " << ct.label_rank(30136371, 'T') << endl;
		//cout << "label_rank(C): " << ct.label_rank(30136371, 'C') << endl;
		//cout << "label_rank((char)1): " << ct.label_rank(30136371, (char)1) << endl;

		//cout << "degree(ct.child(1, 16)): " << ct.degree(ct.child(1, 16)) << endl;
		//cout << "child(1, 16): " << ct.child(1,16) << endl;
		//cout << "child(ct.child(1, 16), 4): " << ct.child(ct.child(1, 16), 4) << endl;
		
		
		//cout << "degree(362): " << ct.degree(362) << endl;
		//cout << "child(362, 4): " << ct.child(362, 4) << endl;
		time = tester(&ct);

		//cout << "Test Child." << endl; test_child(&ct);
		print_output(name, name_letts, check_data(&b, letts2, total_nodes), total_nodes, b.size(), time, voc_size);
		ct.get_size();
	} else if (strcmp(type_wt, "wt") == 0) {
		name = "Balanced Wavelet Tree";
		//cardinal_tree<wt_blcd<>> ct(letts2, &b, &info);
		cardinal_tree<wt_blcd<>> ct(my_vector, &b, &info);
		time = tester(&ct);
		//cout << "Test Degree." << endl; brute_test_degree(&b);
		print_output(name, name_letts, check_data(&b, letts2, total_nodes), total_nodes, b.size(), time, voc_size);
		ct.get_size();
	} else if (strcmp(type_wt, "wth") == 0) {
		name = "Huffman Wavelet Tree";
		//cardinal_tree<wt_huff<>> ct(letts2, &b, &info);
		cardinal_tree<wt_huff<>> ct(my_vector, &b, &info);
		time = tester(&ct);
		//cout << "Test Degree." << endl; brute_test_degree(&b);
		print_output(name, name_letts, check_data(&b, letts2, total_nodes), total_nodes, b.size(), time, voc_size);
		ct.get_size();
	} else if (strcmp(type_wt, "ls") == 0) {
		name = "Linear Search";
		cardinal_tree_ls ct(letts2, &b, &info);
		//cardinal_tree_ls ct(my_vector, &b, &info);
		time = tester(&ct);
		print_output(name, name_letts, check_data(&b, letts2, total_nodes), total_nodes, b.size(), time, voc_size);
		ct.get_size();
	} else if (strcmp(type_wt, "bs") == 0) {
		name = "Binary Search";
		//cardinal_tree_bs ct(letts2, &b, &info);
		cardinal_tree_bs ct(&my_vector, &b, &info);
		time = tester(&ct);
		//cout << "Test Degree." << endl; brute_test_degree(&b);
		print_output(name, name_letts, check_data(&b, letts2, total_nodes), total_nodes, b.size(), time, voc_size);
		ct.get_size();
	} else if (strcmp(type_wt, "ap") == 0) {
		name = "Alphabet Partitioning";
		//cardinal_tree<wt_ap<>> ct(letts2, &b, &info);
		cardinal_tree<wt_ap<>> ct(my_vector, &b, &info);
		time = tester(&ct);
		//cout << "Test Degree." << endl; brute_test_degree(&b);
		print_output(name, name_letts, check_data(&b, letts2, total_nodes), total_nodes, b.size(), time, voc_size);
		ct.get_size();
	}
	cout << endl;
}


int main_test() {
	bit_vector b = {1,1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0};
	string letts = "abcdabcdabcd";
	letts = "abcdefghijkl";
	vector<int> info{0,1,2,3,4,5,6,7,8,9,10,11,12};

	cout << "Validador Total parentesis: " << count_parentheses(&b) << endl;
	cout << "[CHECK DATA] ";
	size_t total_nodes = 13;
	if (check_data(&b, letts, total_nodes) == true) cout << "OK" << endl;
	else cout << "FAILED" << endl;

	cardinal_tree<wt_gmr<>> ct(letts, &b, &info);

	//cout << "label_rank: " << ct.label_rank(0, 'a') << endl;
	//cout << "label_select: " << ct.label_select(1, 'a') << endl;
	//cout << "tree_rank0: " << ct.tree_rank0(5) << endl;
	//cout << "tree_rank1: " << ct.tree_rank1(0) << endl;
	//cout << "tree_select0: " << ct.tree_select0(1) << endl;
	//cout << "tree_select1: " << ct.tree_select1(1) << endl;
	//cout << "Test Preorder." << endl; test_preorder(&ct);
	//cout << "Wrong (8): " << ct.preorder(8) << endl;
	//cout << "Wrong (11): " << ct.preorder(11) << endl;

	//cout << "Test Child." << endl; test_child(&ct);
	cout << "Test label: " << endl; test_label(&ct);
	//cout << "Test label_child: " << endl; test_label_child(&ct);
	cout << "Test degree: " << endl; test_degree(&ct);

	return 0;
}


int main_test2() {
	bit_vector b = {1,1,1,1,0,1,1,0,0,0,0,1,1,0,1,0,0,1,1,0,0,0};
	string letts = "bcrbcczbcr";
	vector<int> info{0,1,2,3,4,5,6,7,8,9,10};

	cout << "[CHECK DATA] ";
	size_t total_nodes = 11;
	if (check_data(&b, letts, total_nodes) == true) cout << "OK" << endl;
	else cout << "FAILED" << endl;

	cardinal_tree<wt_gmr<>> ct(letts, &b, &info);
	//cardinal_tree<wt_huff<>> ct(letts, &bptest, &info);
	//cardinal_tree<wt_blcd<>> ct(letts, &bptest, &info);
	//cardinal_tree<wt_gmr<>> ct(letts, &bptest, &info);

	cout << "[TREE OPERATIONS TEST]" << endl;
	cout << "tree_rank0(7): " << ct.tree_rank0(7) << endl;
	cout << "tree_rank1(7): " << ct.tree_rank1(7) << endl;
	cout << "tree_rank0(17): " << ct.tree_rank0(17) << endl;
	cout << "tree_rank1(17): " << ct.tree_rank1(17) << endl;
	cout << "tree_select0(7): " << ct.tree_select0(7) << endl;
	cout << "tree_select1(7): " << ct.tree_select1(7) << endl;
	cout << "size bp: " << ct.count_nodes() << endl;
	cout << "[TREE OPERATIONS OK]" << endl << endl;


	//for (size_t i=1; i<17; i++) cout << ct.label(1, i) << " "; cout << endl;
	//cout << "Print data:" << endl; ct.print_data();
	//cout << "Test Degree." << endl; test_degree(&ct);
	//cout << "Test Parent." << endl; test_parent(&ct);
	//cout << "Test Child." << endl; test_child(&ct);
	//cout << "Test Preorder." << endl; test_preorder(&ct);
	//cout << "Test Child_rank." << endl; test_child_rank(&ct);
	//cout << "Test Select_node." << endl; test_select_node(&ct);
	//cout << "Test Subtree_size." << endl; test_subtree_size(&ct);
	//cout << "Test access_data: " << endl; test_access_data(&ct);
	//cout << "Test label(j=0, 1): " << ct.label(1, 1) << endl;
	//cout << "Test label: " << endl; test_label(&ct);
	cout << "Test label_child: " << endl; test_label_child(&ct);
	//cout << "label_rank(7, 'a'): " << ct.label_rank(7, 'a') << endl;
	//cout << "label_select(1, 'n'): " << ct.label_select(1, 'n') << endl;
	//cout << "Test label_reverse: " << endl; test_label_reverse(&ct);
	//cout << "Test label_child_reverse: " << endl; test_label_child_reverse(&ct);
	//cout << "[Node 0] label_child_reverse(1, 'r'): " << ct.label_child_reverse(1, 'r') << endl;
	//cout << "[Node 1] label_child_reverse(5, 'c'): " << ct.label_child_reverse(5, 'c') << endl;
	//cout << "[Node 5] label_child_reverse(11, 'r'): " << ct.label_child_reverse(11, 'r') << endl;
	//tester(&ct);

	//cout << "ancestor(j=5, j=4): " << ct.ancestor(ct.select_node(5), ct.select_node(4)) << endl;
	//cout << "ancestor(j=5, j=9): " << ct.ancestor(ct.select_node(5), ct.select_node(9)) << endl;
	return 0;
}
