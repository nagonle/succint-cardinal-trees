#ifndef INCLUDED_READ_UTILS
#define INCLUDED_READ_UTILS
#include <sdsl/bit_vectors.hpp>
#include <sdsl/int_vector.hpp>
#include <sdsl/bp_support.hpp>
#include <sdsl/wavelet_trees.hpp>
#include <vector>

using namespace std;
using namespace sdsl;

typedef unsigned char uchar;

// Check consistency on all data read.
extern bool check_data(bit_vector *b, int_vector<> *letts, size_t total_nodes);

// read_bp: store in bp a sequence of bp in file name_file
extern void read_bp(char* name_file, char **bp, uint nodes);

// check_balanced: return true if bp contain a balanced sequence of parentheses.
extern bool check_balanced(bit_vector *bp);

// bp_string_to_bit_vector: convert bp sequence in a sequence of 1's and 0's. It store in b.
extern void bp_string_to_bit_vector(char *bp, bit_vector *b); 

// replace_null: Search in seq occurrences for NULL character and replace it for char c.
template <typename Size_Type>
extern void replace_null(Size_Type *seq, Size_Type c, size_t N) {
	for (size_t i=0; i<N; i++) {
		if (seq[i] == 0) {
			seq[i] = c;
		}
	}
};

// vocabulary_size: Return the count of distinc characters.
template <typename Size_Type>
extern size_t vocabulary_size(int_vector<> *letts) {
	size_t count = 0;
	set<Size_Type> voc;
	for (size_t i=0; i<letts->size(); i++) {
		voc.insert((*letts)[i]);
	}
	count = voc.size();
	return count;
};

// read_letts: Store the symbols in letts and return the number of nodes.
template <typename Size_Type, typename Count_Type>
extern uint64_t read_letts(char *name_file, Size_Type **letts) {
	Count_Type nodes, symbols;
	Size_Type tmp;
	ifstream in;
	in.open(name_file, ios::in | ios::binary);
	// Read total nodes
	in.read((char*)&nodes, sizeof(Count_Type));
	symbols = nodes - 1;
	// Read zero separator
	in.read((char*)&tmp, sizeof(Size_Type));
	(*letts) = (Size_Type*)malloc(sizeof(Size_Type)*symbols);
	in.read((char*)*letts, sizeof(Size_Type)*symbols);
	in.close();

	return nodes;
};

#endif
