#ifndef INCLUDED_READ_UTILS
#define INCLUDED_READ_UTILS
#include <sdsl/bit_vectors.hpp>
#include <sdsl/bp_support.hpp>
#include <sdsl/wavelet_trees.hpp>
#include <vector>

using namespace std;
using namespace sdsl;

typedef unsigned char uchar;

// Check all data read.
extern bool check_data(bit_vector *b, string& letts, size_t total_nodes);

// vocabulary_size: Return the count of distinc characters.
extern size_t vocabulary_size(string& letts);

// read_letts: read a file that contains symbols and number of nodes.
extern uint read_letts(char *name_file, uint8_t **letts); 

// read_bp: store in bp a sequence of bp in file name_file
extern void read_bp(char* name_file, char **bp, uint nodes);

// check_balanced: return true if bp contain a balanced sequence of parentheses.
extern bool check_balanced(bit_vector *bp);

// bp_string_to_bit_vector: convert bp sequence in a sequence of 1's and 0's. It store in b.
extern void bp_string_to_bit_vector(char *bp, bit_vector *b); 

// read_size: read uint from file.
extern uint read_size(string name_file); 

// exists: return true if char c, appear in string seq. False in other case.
extern bool exists(string seq, char c, size_t N); 

// replace_null: Search in seq occurrences for NULL character and replace it for char c.
extern void replace_null(uint8_t *seq, char c, size_t N); 

#endif
