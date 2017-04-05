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
	cout << name_dataset << "|" << check_status << "|" << name_structure << "|" << bp_count << "|" << letts_count << "|" << voc_size << "|" << (double)letts_size*8/letts_count << "|" << (double)tree_size*8/(letts_count+1) << "|" << (double)((letts_size+tree_size)*8)/(letts_count+1) << "|" << time_random << endl;
}

void print_dataset_info(size_t max_arity, double average_arity, size_t max_height, double average_height) {
	cout << fixed;
	cout << setprecision(2);
	cout << max_arity << "|" << average_arity << "|" << max_height << "|" << average_height << endl;
}

void check_tolerance(bit_vector * bp, uint64_t NODES) {
		bp_support_sada<> tree(bp);
		cout << "rank: " << tree.rank(2*NODES - 1) << endl;
		cout << "find_close(0): " << tree.find_close(0) << endl;
		cout << "total nodes: " << NODES << endl << endl;
		bp_support_sada<256u, 32u, rank_support_v5<1>, bit_vector::select_0_type> bps(bp); 
		cout << "rank: " << bps.rank(2*NODES - 1) << endl;
		cout << "find_close(0): " << bps.find_close(0) << endl;
		cout << "total nodes: " << NODES << endl;
}

template <typename Size_Type, typename Count_Type>
void process_data(char *name_bp, char *name_letts, char *type_wt) {
	char *bp;
	size_t voc_size;
	double time_random;
	Count_Type total_symbols, total_nodes;
	string name;

	// Get Data.
	// Read total_nodes and symbols.
	Size_Type *letts;
	total_nodes = read_letts<Size_Type, Count_Type>(name_letts, &letts);
	total_symbols = total_nodes - 1;

	// Construct symbols sequence.
	int_vector<> sequence_vector(total_symbols, 0, sizeof(Size_Type)*8);
	replace_null<Size_Type>(letts, (Size_Type)1, total_symbols);
	for (int i=0; i<total_symbols; i++) sequence_vector[i] = letts[i];

	// Read sequence of parentheses and store in bp.
	read_bp(name_bp, &bp, total_nodes);

	// Construct parentheses sequence.
	bit_vector b(total_nodes*2);
	bp_string_to_bit_vector(bp, &b);

	// #####################
	// # CHECK BP SADAKANE #
	// #####################
	bool CHECK=false;
	if (CHECK == true) {
		check_tolerance(&b, total_nodes);
		return ;
	}
	// ############# END CHECK ############# //

	// Create Cardinal Tree.
	vector<int> info;
	info.push_back(42);


	// Get vocabulary size.
	voc_size = vocabulary_size<Size_Type>(&sequence_vector);
	// Check consistency on data.
	string check_status;
	if (check_data(&b, &sequence_vector, total_nodes) == true) check_status = "OK";
	else check_status = "FAILED";
	//cout << "END CHECK: " << check_status << endl;
	//return ;

	// Construct Cardinal Tree and run tests.
	if (strcmp(type_wt, "gmr") == 0) {
		name = "Golynski";
		cardinal_tree<wt_gmr<>, Size_Type> ct(sequence_vector, &b, &info);

		time_random = tester<cardinal_tree<wt_gmr<>, Size_Type>, Size_Type>(&ct);

		print_output(name, name_letts, check_status, ct.get_bp_count(), ct.get_letts_count(), voc_size, time_random, ct.get_letts_size(), ct.get_tree_size());
	} else if (strcmp(type_wt, "wt") == 0) {
		name = "Balanced Wavelet Tree";
		string chars = string((char*)letts);
		cardinal_tree<wt_blcd<>, Size_Type> ct(chars, &b, &info);

		time_random = tester<cardinal_tree<wt_blcd<>, Size_Type>, Size_Type>(&ct);

		print_output(name, name_letts, check_status, ct.get_bp_count(), ct.get_letts_count(), voc_size, time_random, ct.get_letts_size(), ct.get_tree_size());
	} else if (strcmp(type_wt, "wth") == 0) {
		name = "Huffman Wavelet Tree";
		string chars = string((char*)letts);
		cardinal_tree<wt_huff<>, Size_Type> ct(chars, &b, &info);

		time_random = tester<cardinal_tree<wt_huff<>, Size_Type>, Size_Type>(&ct);

		print_output(name, name_letts, check_status, ct.get_bp_count(), ct.get_letts_count(), voc_size, time_random, ct.get_letts_size(), ct.get_tree_size());
	} else if (strcmp(type_wt, "wt32") == 0) {
		name = "Balanced Wavelet Tree";
		cardinal_tree<wt_int<>, Size_Type> ct(sequence_vector, &b, &info);

		time_random = tester<cardinal_tree<wt_int<>, Size_Type>, Size_Type>(&ct);

		print_output(name, name_letts, check_status, ct.get_bp_count(), ct.get_letts_count(), voc_size, time_random, ct.get_letts_size(), ct.get_tree_size());
	} else if (strcmp(type_wt, "wth32") == 0) {
		name = "Huffman Wavelet Tree";
		cardinal_tree<wt_huff_int<>, Size_Type> ct(sequence_vector, &b, &info);

		time_random = tester<cardinal_tree<wt_huff_int<>, Size_Type>, Size_Type>(&ct);

		print_output(name, name_letts, check_status, ct.get_bp_count(), ct.get_letts_count(), voc_size, time_random, ct.get_letts_size(), ct.get_tree_size());
	} else if (strcmp(type_wt, "ls") == 0) {
		name = "Linear Search";
		cardinal_tree_ls<Size_Type> ct(&sequence_vector, &b, &info);

		time_random = tester<cardinal_tree_ls<Size_Type>, Size_Type>(&ct);

		print_output(name, name_letts, check_status, ct.get_bp_count(), ct.get_letts_count(), voc_size, time_random, ct.get_letts_size(), ct.get_tree_size());
	} else if (strcmp(type_wt, "bs") == 0) {
		name = "Binary Search";
		cardinal_tree_bs<Size_Type> ct(&sequence_vector, &b, &info);

		time_random = tester<cardinal_tree_bs<Size_Type>, Size_Type>(&ct);

		print_output(name, name_letts, check_status, ct.get_bp_count(), ct.get_letts_count(), voc_size, time_random, ct.get_letts_size(), ct.get_tree_size());
	} else if (strcmp(type_wt, "ap") == 0) {
		name = "Alphabet Partitioning";
		cardinal_tree<wt_ap<>, Size_Type> ct(sequence_vector, &b, &info);

		time_random = tester<cardinal_tree<wt_ap<>, Size_Type>, Size_Type>(&ct);

		print_output(name, name_letts, check_status, ct.get_bp_count(), ct.get_letts_count(), voc_size, time_random, ct.get_letts_size(), ct.get_tree_size());
	} else if (strcmp(type_wt, "ap32") == 0) {
		name = "Alphabet Partitioning";
		cardinal_tree<wt_ap<wt_huff_int<>, wt_int<>>, Size_Type> ct(sequence_vector, &b, &info);

		time_random = tester<cardinal_tree<wt_ap<wt_huff_int<>, wt_int<>>, Size_Type>, Size_Type>(&ct);

		print_output(name, name_letts, check_status, ct.get_bp_count(), ct.get_letts_count(), voc_size, time_random, ct.get_letts_size(), ct.get_tree_size());
	} else if (strcmp(type_wt, "describe") == 0) {
		cardinal_tree<wt_huff_int<>, Size_Type> ct(sequence_vector, &b, &info);

		size_t max_arity = get_max_arity(&ct);
		double average_arity = get_average_arity(&ct);
		int count = 0;
		size_t tree_height = get_tree_height(&ct, count);
		double sum_heights = 0, count_nodes = 0;
		double tree_average_height = get_tree_average_height(&ct, &sum_heights, &count_nodes);

		print_dataset_info(max_arity, average_arity, tree_height, tree_average_height);
	} else if (strcmp(type_wt, "arity") == 0) {
		cardinal_tree<wt_huff_int<>, Size_Type> ct(sequence_vector, &b, &info);
		double average_arity = get_average_arity(&ct);
		cout << "Average arity: " << average_arity << endl;
	} else if (strcmp(type_wt, "max_arity") == 0) {
		cardinal_tree<wt_huff_int<>, Size_Type> ct(sequence_vector, &b, &info);
		double max_arity = get_max_arity(&ct);
		cout << "Max arity: " << max_arity << endl;
	} else if (strcmp(type_wt, "height") == 0) {
		cardinal_tree<wt_huff_int<>, Size_Type> ct(sequence_vector, &b, &info);
		double sum_heights = 0, count_nodes = 0;
		double average_height = get_tree_average_height(&ct, &sum_heights, &count_nodes);
		cout << "Average height: " << average_height << endl;
	} else if (strcmp(type_wt, "max_height") == 0) {
		cardinal_tree<wt_huff_int<>, Size_Type> ct(sequence_vector, &b, &info);
		int count = 0;
		double max_height = get_tree_height(&ct, count);
		cout << "Max height: " << max_height << endl;
		cout << "Count: " << count << endl;
	} else if (strcmp(type_wt, "nodes") == 0) {
		cout << "Total nodes: " << total_nodes << endl;
	} else if (strcmp(type_wt, "symbols") == 0) {
		cout << "Total symbols: " << total_symbols << endl;
	} 
};


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
	if (argc == 3 || argc == 4) {
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
	if (argc == 4) {
		if (strcmp(argv[3], "uint32_t") == 0) {
			process_data<uint32_t, uint64_t>(name_bp, name_letts, type_wt);
		}
	} else {
		process_data<uint8_t, uint32_t>(name_bp, name_letts, type_wt);
	}
	return 0;
}
