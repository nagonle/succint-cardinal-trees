#include <iostream>
#include <sdsl/bit_vectors.hpp>
#include <sdsl/bp_support.hpp>
#include <sdsl/wavelet_trees.hpp>
using namespace std;
using namespace sdsl;

int main(int argc, char *argv[])
{
	load_tree();
	if (strcmp(argv[1], "height") cout << get_tree_height();
	else if (strcmp(argv[1], "average_height") cout << get_tree_average_height();
	else if (strcmp(argv[1], "max_arity") cout << get_max_arity();
	else if (strcmp(argv[1], "average_arity") cout << get_average_arity();
	
	return 0;
}

