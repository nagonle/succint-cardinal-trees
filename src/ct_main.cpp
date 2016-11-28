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
#include "tests_core.hpp"
using namespace std;
using namespace sdsl;

void print_output(string name_structure, char *name_dataset, string check_status, size_t bp_count, size_t letts_count, size_t voc_size, double time_random, double time_full_tree, double time, size_t letts_size, size_t tree_size) {
	cout << name_dataset << "|" << check_status << "|" << name_structure << "|" << bp_count << "|" << letts_count << "|" << voc_size << "|" << time_random << "|" << time_full_tree << "|" << time << "|" << letts_size << "|" << tree_size << endl;
}

void process_data(char *name_bp, char *name_letts, char *type_wt) {
	char *bp;
	uint8_t *letts;
	size_t voc_size;

	// Get Data.
	// Read total_nodes and symbols.
	uint total_nodes = read_letts(name_letts, &letts);
	uint total_symbols = total_nodes - 1;

	// Read sequence of parentheses and store in bp.
	read_bp(name_bp, &bp, total_nodes);

	bit_vector b(total_nodes*2);
	bp_string_to_bit_vector(bp, &b);

	// Create Cardinal Tree.
	vector<int> info;
	info.push_back(42);

 	replace_null(letts, (char)1, total_symbols);

	int_vector<> my_vector(total_symbols, 0, 8);
	for (int i=0; i<total_symbols; i++) my_vector[i] = letts[i];

	string check_status;
	if (check_data(&b, &my_vector, total_nodes) == true) check_status = "OK";
	else check_status = "FAILED"; 

	string name;
	double time_random, time;
	double time_full_tree = 0;
	if (strcmp(type_wt, "gmr") == 0) {
		name = "Golynski";
		cardinal_tree<wt_gmr<>> ct(my_vector, &b, &info);

		time_random = tester(&ct);
		time = new_test_label_child(&ct);
		time_full_tree = test_label_child(&ct);

		print_output(name, name_letts, check_status, ct.get_bp_count(), ct.get_letts_count(), voc_size, time_random, time_full_tree, time, ct.get_letts_size(), ct.get_tree_size());
	} else if (strcmp(type_wt, "wt") == 0) {
		name = "Balanced Wavelet Tree";
		cardinal_tree<wt_blcd<>> ct(my_vector, &b, &info);
		time_random = tester(&ct);
		time = new_test_label_child(&ct);
		time_full_tree = test_label_child(&ct);
	} else if (strcmp(type_wt, "wth") == 0) {
		name = "Huffman Wavelet Tree";
		cardinal_tree<wt_huff<>> ct(my_vector, &b, &info);
		time_random = tester(&ct);
		time = new_test_label_child(&ct);
		time_full_tree = test_label_child(&ct);
	} else if (strcmp(type_wt, "ls") == 0) {
		name = "Linear Search";
		cardinal_tree_ls ct(&my_vector, &b, &info);
		time_random = tester(&ct);
		time = new_test_label_child(&ct);
		time_full_tree = test_label_child(&ct);
	} else if (strcmp(type_wt, "bs") == 0) {
		name = "Binary Search";
		cardinal_tree_bs ct(&my_vector, &b, &info);
		time_random = tester(&ct);
		time = new_test_label_child(&ct);
		time_full_tree = test_label_child(&ct);
	} else if (strcmp(type_wt, "ap") == 0) {
		name = "Alphabet Partitioning";
		cardinal_tree<wt_ap<>> ct(my_vector, &b, &info);
		time_random = tester(&ct);
		time = new_test_label_child(&ct);
		time_full_tree = test_label_child(&ct);
	}
}


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
	} else {
		print_usage();
	}
	process_data(name_bp, name_letts, type_wt);
	return 0;
}
