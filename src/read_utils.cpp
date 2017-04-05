#include <sdsl/bit_vectors.hpp>
#include <sdsl/bp_support.hpp>
#include <sdsl/wavelet_trees.hpp>
#include <vector>
#include <set>
#include "read_utils.hpp"

using namespace std;
using namespace sdsl;


// Check all data read.
bool check_data(bit_vector *b, int_vector<> *letts, size_t total_nodes) {
	size_t total_symbols = total_nodes - 1;
	bool balanced = check_balanced(b);
	if (balanced == false) cout << "Not Balanced" << endl;
   	if (!(b->size() == total_nodes * 2 && letts->size() == total_symbols)) cout << "Problem with sizes";
	if (balanced == true && b->size() == total_nodes * 2 && letts->size() == total_symbols) return true;
	return false;
}

// read_bp: Store sequence of balanced parentheses in bp.
void read_bp(char* name_file, char **bp, uint nodes) {
	size_t size_block = nodes * 2;
	char *tmp = (char*)malloc(sizeof(char));
	ifstream in(name_file);
	(*bp) = (char*)malloc(size_block+1);
	in.read((*bp), size_block);
	in.close();
	(*bp)[size_block] = 0;
	free(tmp);
}

// check_balanced: return true if bp contain a balanced sequence of parentheses.
bool check_balanced(bit_vector *bp) {
	stack<char> bp_stack;
	size_t N = bp->size(); 
	for (size_t i=0; i<N; i++) {
		if ((*bp)[i] == 1u) bp_stack.push('1');
		else
			if (bp_stack.empty() == false) bp_stack.pop();
			else {
				cout << "there are more close parentheses than open parentheses." << " [pos]" << i << endl;
				return false;
			}
	}

	if (bp_stack.empty() == true) return true;
	cout << "There are more open parentheses than close parentheses" << endl;
	return false;
}

// bp_string_to_bit_vector: convert bp sequence in a sequence of 1's and 0's. It store in b.
void bp_string_to_bit_vector(char *bp, bit_vector *b) {
	for (size_t i=0; i<b->size(); i++) {
		if (bp[i] == '(') (*b)[i] = 1;
		else (*b)[i] = 0;
	}
}

