#include <iostream>
#include <string>
#include <sdsl/bit_vectors.hpp>
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
using namespace std;
using namespace sdsl;

typedef unsigned char uchar;


template<class type>
double tester(type *ct, size_t N=20, bool verbose=false) {
	//double total_time, time;
	chrono::high_resolution_clock::time_point start_time, end_time;
	chrono::duration<double> total_time;
	size_t count, degree, node, ith_child, next_node;
	char label;
	count = degree = 0;
	// 1 is the root node.
	size_t root_degree = ct->degree(1);
	if (verbose) cout << "root_degree:" << root_degree << endl;
	node = 1;
	while (count < N) {
		if (degree == 0) {
			node = 1;
			degree = root_degree;
		}
		ith_child = rand() % degree + 1;
		if (verbose) cout << "[DEBUG] node: " << node << " label: " << label << " Preorder: " << ct->preorder(node) << endl;
		if (verbose) cout << "[DEBUG] degree: " << ct->degree(node) << endl;
		label = ct->label(node, ith_child);
		if (verbose) cout << "[DEBUG] New label: " << label << endl;
		if (verbose) cout << "[DEBUG] ct->label_child(" << node << ", '" << label << "'): " << endl << endl; 

		//time = getTime();
		start_time = chrono::high_resolution_clock::now();
		next_node = ct->label_child(node, label);
		end_time = chrono::high_resolution_clock::now();
		//cout << "microseconds: " << chrono::duration_cast<chrono::microseconds>(end_time - start_time).count()/1000000. << endl;
		//total_time += (getTime() - time);
		total_time += end_time - start_time;

		node = next_node;
		if (node > ct->count_nodes()*2) degree = 0;
		else degree = ct->degree(node);
		//degree = ct->degree(node);

		count++;
	}
	//cout << "[TEST] Total time (" <<  N << " queries): " << total_time << endl;
	//cout << "[CHRONO] " << chrono::duration_cast<chrono::microseconds>(total_time).count() << "ms" << endl;
	//cout << "[CHRONO] " << chrono::duration_cast<chrono::nanoseconds>(total_time).count() << "ns" << endl;
	double time = chrono::duration_cast<chrono::microseconds>(total_time).count();
	//double average_time = total_time / N;
	//return average_time;
	//return total_time;
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

//void replace_null(string seq, char c, size_t N); 
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
		print_usage();
		main_test();
		return 0;
	}
	process_data(name_bp, name_letts, type_wt);
	return 0;
}

void print_output(string structure, char *name_letts, bool check_data, size_t total_nodes, size_t bp_size, double time, size_t voc_size) {
	//cout << "########## TEST [" << structure << "] on [" << name_letts << "] ##########" << endl;
	//cout << "[DEBUG] Check data: " << check_data << endl;
	//cout << "[Tree Info] Total nodes: " << total_nodes << endl;
	//cout << "[Tree Info] Size parentheses: " << bp_size << endl;
	//cout << "[TEST]" << " Average Time: " << time << "[us]" << endl;
	cout << "[" << name_letts << "][" << structure << "][Time][" << time << "][microseconds]" << "[Voc Size][" << voc_size << "]" << endl;
}

void process_data(char *name_bp, char *name_letts, char *type_wt) {
	char *bp, *letts;
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
	letts2 = letts;
	voc_size = vocabulary_size(letts2);
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
		cardinal_tree<wt_gmr<>> ct(letts2, &b, &info);
		time = tester(&ct);
		print_output(name, name_letts, check_data(&b, letts2, total_nodes), total_nodes, b.size(), time, voc_size);
	} else if (strcmp(type_wt, "wt") == 0) {
		name = "Balanced Wavelet Tree";
		cardinal_tree<wt_blcd<>> ct(letts2, &b, &info);
		time = tester(&ct);
		print_output(name, name_letts, check_data(&b, letts2, total_nodes), total_nodes, b.size(), time, voc_size);
	} else if (strcmp(type_wt, "wth") == 0) {
		name = "Huffman Wavelet Tree";
		cardinal_tree<wt_huff<>> ct(letts2, &b, &info);
		time = tester(&ct);
		print_output(name, name_letts, check_data(&b, letts2, total_nodes), total_nodes, b.size(), time, voc_size);
	} else if (strcmp(type_wt, "bin") == 0) {
		name = "Binary Search";
		cardinal_tree_bs ct(letts2, &b, &info);
		time = tester(&ct);
		print_output(name, name_letts, check_data(&b, letts2, total_nodes), total_nodes, b.size(), time, voc_size);
	} else if (strcmp(type_wt, "ap") == 0) {
		name = "Alphabet Partitioning";
		cardinal_tree<wt_gmr<>> ct(letts2, &b, &info);
		time = tester(&ct);
		print_output(name, name_letts, check_data(&b, letts2, total_nodes), total_nodes, b.size(), time, voc_size);
	}

	//cout << "[DEBUG] NAME: " << name << endl;

}


int main_test() {
	bit_vector bptest = {1,1,1,1,0,1,1,0,0,0,0,1,1,0,1,0,0,1,1,0,0,0};
	string letts = "bcrbccrbcr";
	vector<int> info{0,1,2,3,4,5,6,7,8,9,10};

	cardinal_tree<wt_gmr<>> ct(letts, &bptest, &info);
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


	for (size_t i=1; i<17; i++) cout << ct.label(1, i) << " "; cout << endl;
	//cout << "Print data:" << endl; ct_gmr.print_data();
	//cout << "Test Degree." << endl; test_degree(&ct_gmr);
	//cout << "Test Parent." << endl; test_parent(&ct_gmr);
	//cout << "Test Child." << endl; test_child(&ct_gmr);
	//cout << "Test Preorder." << endl; test_preorder(&ct_gmr);
	//cout << "Test Child_rank." << endl; test_child_rank(&ct_gmr);
	//cout << "Test Select_node." << endl; test_select_node(&ct_gmr);
	//cout << "Test Subtree_size." << endl; test_subtree_size(&ct_gmr);
	//cout << "Test access_data: " << endl; test_access_data(&ct_gmr);
	//cout << "Test label(j=0, 1): " << ct_gmr.label(1, 1) << endl;
	cout << "Test label: " << endl; test_label(&ct);
	cout << "Test label_child: " << endl; test_label_child(&ct);
	//cout << "label_rank(7, 'a'): " << ct_gmr.label_rank(7, 'a') << endl;
	//cout << "label_select(1, 'n'): " << ct_gmr.label_select(1, 'n') << endl;
	//cout << "Test label_reverse: " << endl; test_label_reverse(&ct_gmr);
	//cout << "Test label_child_reverse: " << endl; test_label_child_reverse(&ct_gmr);
	//cout << "[Node 0] label_child_reverse(1, 'r'): " << ct_gmr.label_child_reverse(1, 'r') << endl;
	//cout << "[Node 1] label_child_reverse(5, 'c'): " << ct_gmr.label_child_reverse(5, 'c') << endl;
	//cout << "[Node 5] label_child_reverse(11, 'r'): " << ct_gmr.label_child_reverse(11, 'r') << endl;
	//tester(&ct);

	//cout << "ancestor(j=5, j=4): " << ct.ancestor(ct.select_node(5), ct.select_node(4)) << endl;
	//cout << "ancestor(j=5, j=9): " << ct.ancestor(ct.select_node(5), ct.select_node(9)) << endl;
	return 0;
}
