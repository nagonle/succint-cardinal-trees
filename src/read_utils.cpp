#include <sdsl/bit_vectors.hpp>
#include <sdsl/bp_support.hpp>
#include <sdsl/wavelet_trees.hpp>
#include <vector>
#include <set>
#include "read_utils.hpp"

using namespace std;
using namespace sdsl;

typedef unsigned char uchar;

// Check all data read.
bool check_data(bit_vector *b, string& letts, size_t total_nodes) {
	size_t total_symbols = total_nodes - 1;
	if (b->size() == total_nodes * 2 && letts.size() == total_symbols) return true;
	return false;
}

size_t vocabulary_size(string& letts) {
	size_t count = 0;
	set<char> voc;
	for (size_t i=0; i<letts.size(); i++) {
		voc.insert(letts[i]);
	}
	count = voc.size();
	return count;
}

// read_letts: Store the symbols in letts and return the number of nodes.
uint read_letts(char *name_file, uchar **letts) {
	uint nodes, symbols;
	char tmp[2];
	ifstream in;
	in.open(name_file, ios::in | ios::binary);
	// Read total nodes
	in.read((char*)&nodes, sizeof(uint));
	symbols = nodes - 1;
	// Read zero separator
	in.read((char*)&tmp, sizeof(char));
	(*letts) = (uchar*)malloc(symbols+1);
	in.read((char*)letts, symbols);
	in.close();
	(*letts)[symbols] = 0;

	return nodes;
}
uint read_letts2(char *name_file, char **letts) {
	uint nodes, symbols;
	char tmp[2];
	ifstream in;
	in.open(name_file, ios::in | ios::binary);
	// Read total nodes
	in.read((char*)&nodes, sizeof(uint));
	symbols = nodes - 1;
	// Read zero separator
	in.read((char*)&tmp, sizeof(char));
	(*letts) = (char*)malloc(symbols+1);
	in.read(*letts, symbols);
	in.close();
	(*letts)[symbols] = 0;

	return nodes;
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
		if ((int)bp[i] == 1) bp_stack.push('1');
		else
			if (bp_stack.empty() == false) bp_stack.pop();
			else return false;
	}

	if (bp_stack.empty() == true) return true;
	return false;
}

// bp_string_to_bit_vector: convert bp sequence in a sequence of 1's and 0's. It store in b.
void bp_string_to_bit_vector(char *bp, bit_vector *b) {
	for (size_t i=0; i<b->size(); i++) {
		if (bp[i] == '(') (*b)[i] = 1;
		else (*b)[i] = 0;
	}
}

// read_size: read uint from file.
uint read_size(string name_file) {
	cout << "open file: " << name_file << endl;
	uint total_nodes;
	ifstream in(name_file, ios::binary);
	in.read((char*)&total_nodes, sizeof(total_nodes));
	in.close();
	return total_nodes;
}

// exists: return true if char c, appear in string seq. False in other case.
bool exists(string seq, char c, size_t N) {
	for (size_t i=0; i<N; i++) {
		if (seq[i] == c) return true;
	}
	return false;
}

// replace_null: Search in seq occurrences for NULL character and replace it for char c.
void replace_null(uchar *seq, char c, size_t N) {
	for (size_t i=0; i<N; i++) {
		if (seq[i] == 0) {
			seq[i] = c;
		}
	}
	seq[N] = '\0';
}

void replace_null2(char *seq, char c, size_t N) {
	for (size_t i=0; i<N; i++) {
		if (seq[i] == 0) {
			seq[i] = c;
		}
	}
	seq[N] = '\0';
}
