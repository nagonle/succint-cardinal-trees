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
#include <iomanip>
using namespace std;
using namespace sdsl;

void print_output(string name_structure, char *name_dataset, string check_status, size_t bp_count, size_t letts_count, size_t voc_size, double time_random, size_t letts_size, size_t tree_size) {
	cout << name_dataset << "|" << check_status << "|" << name_structure << "|" << bp_count << "|" << letts_count << "|" << voc_size << "|" << (double)letts_size/letts_count << "|" << (double)tree_size/(letts_count+1) << "|" << (double)((letts_size+tree_size)*8)/(letts_count+1) << "|" << time_random << endl;
}

void print_dataset_info(size_t max_arity, double average_arity, size_t max_height, double average_height) {
	cout << fixed;
	cout << setprecision(2);
	cout << max_arity << "|" << average_arity << "|" << max_height << "|" << average_height << endl;
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

 	replace_null(letts, (uint8_t)1, total_symbols);

	int_vector<> my_vector(total_symbols, 0, 8);
	for (int i=0; i<total_symbols; i++) my_vector[i] = letts[i];

	voc_size = vocabulary_size(&my_vector);
	string check_status;
	if (check_data(&b, &my_vector, total_nodes) == true) check_status = "OK";
	else check_status = "FAILED"; 

	string name;
	double time_random, time;
	double time_full_tree = 0;
	if (strcmp(type_wt, "gmr") == 0) {
		name = "Golynski";
		cardinal_tree<wt_gmr<>> ct(my_vector, &b, &info);

		//time_full_tree = test_label_child(&ct);
		//cout << "time_full_tree OK";
		//time = new_test_label_child(&ct);
		//cout << "time OK";
		time_random = tester(&ct);
		//cout << "time random OK";

		print_output(name, name_letts, check_status, ct.get_bp_count(), ct.get_letts_count(), voc_size, time_random, ct.get_letts_size(), ct.get_tree_size());
	} else if (strcmp(type_wt, "wt") == 0) {
		name = "Balanced Wavelet Tree";
		cardinal_tree<wt_blcd<>> ct(my_vector, &b, &info);

		//time_full_tree = test_label_child(&ct);
		//cout << "time_full_tree OK";
		//time = new_test_label_child(&ct);
		//cout << "time OK";
		time_random = tester(&ct);
		//cout << "time random OK";

		print_output(name, name_letts, check_status, ct.get_bp_count(), ct.get_letts_count(), voc_size, time_random, ct.get_letts_size(), ct.get_tree_size());
	} else if (strcmp(type_wt, "wth") == 0) {
		name = "Huffman Wavelet Tree";
		cardinal_tree<wt_huff<>> ct(my_vector, &b, &info);

		//time_full_tree = test_label_child(&ct);
		//cout << "time_full_tree OK";
		//time = new_test_label_child(&ct);
		//cout << "time OK";
		time_random = tester(&ct);
		//cout << "time random OK";

		print_output(name, name_letts, check_status, ct.get_bp_count(), ct.get_letts_count(), voc_size, time_random, ct.get_letts_size(), ct.get_tree_size());
	} else if (strcmp(type_wt, "ls") == 0) {
		name = "Linear Search";
		cardinal_tree_ls ct(&my_vector, &b, &info);

		//time_full_tree = test_label_child(&ct);
		//cout << "time_full_tree OK";
		//time = new_test_label_child(&ct);
		//cout << "time OK";
		time_random = tester(&ct);
		//cout << "time random OK";

		print_output(name, name_letts, check_status, ct.get_bp_count(), ct.get_letts_count(), voc_size, time_random, ct.get_letts_size(), ct.get_tree_size());
	} else if (strcmp(type_wt, "bs") == 0) {
		name = "Binary Search";
		cardinal_tree_bs ct(&my_vector, &b, &info);

		//time_full_tree = test_label_child(&ct);
		//cout << "time_full_tree OK";
		//time = new_test_label_child(&ct);
		//cout << "time OK";
		time_random = tester(&ct);
		//cout << "time random OK";

		print_output(name, name_letts, check_status, ct.get_bp_count(), ct.get_letts_count(), voc_size, time_random, ct.get_letts_size(), ct.get_tree_size());
	} else if (strcmp(type_wt, "ap") == 0) {
		name = "Alphabet Partitioning";
		cardinal_tree<wt_ap<>> ct(my_vector, &b, &info);

		//time_full_tree = test_label_child(&ct);
		//cout << "time_full_tree OK";
		//time = new_test_label_child(&ct);
		//cout << "time OK";
		time_random = tester(&ct);
		//cout << "time random OK";

		print_output(name, name_letts, check_status, ct.get_bp_count(), ct.get_letts_count(), voc_size, time_random, ct.get_letts_size(), ct.get_tree_size());
	} else if (strcmp(type_wt, "describe") == 0) {
		cardinal_tree<wt_gmr<>> ct(my_vector, &b, &info);

		size_t max_arity = get_max_arity(&ct);
		double average_arity = get_average_arity(&ct);
		size_t tree_height = get_tree_height(&ct);
		double sum_heights = 0, count_nodes = 0;
		double tree_average_height = get_tree_average_height2(&ct, &sum_heights, &count_nodes);

		//test_label(&ct);
		print_dataset_info(max_arity, average_arity, tree_height, tree_average_height);
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
