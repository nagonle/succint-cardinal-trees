#ifndef INCLUDED_CARDINAL_TREE_BS
#define INCLUDED_CARDINAL_TREE_BS

#include <sdsl/bit_vectors.hpp>
#include <sdsl/bp_support.hpp>
#include <sdsl/wavelet_trees.hpp>
#include <vector>

using namespace std;
using namespace sdsl;

typedef unsigned char uchar;

class cardinal_tree_bs {
	private:
		int_vector<> *letts; // symbol sequence.
		bp_support_sada<t_sml_blk, t_med_deg, rank_support_v5<0>, bit_vector::select_0_type> *tree; // tree sequence (DFUDS) 
		vector<int> *info; 
		size_t nodes;
		bit_vector *b;
  	public:
		cardinal_tree_bs(int_vector<> *seq_, bit_vector * bp, vector<int> * info_) {
			nodes = (*bp).size() / 2;

			// Inicializar tree BP.
			tree = new bp_support_sada<t_sml_blk, t_med_deg, rank_support_v5<0>, bit_vector::select_0_type>(bp);  // <- pointer to b
			info = info_;
			b = bp;
			letts = seq_;
		}

		char get_bp(size_t x) {
			if ((*b)[x] == 1) return '(';
			return ')';
		}
		size_t get_bp_count() {
			return tree->size();
		}

		size_t get_letts_count() {
			return (*letts).size();
		}
	
		// print_data: print node data.
		void print_data() {
			cout << *info << endl;
		}

		// count_nodes: return total nodes of tree.
		size_t count_nodes() {
			return nodes;
		}

		// Tree operations.
		// tree_rank0: return #0's until n inclusive.
		size_t tree_rank0(size_t x) {
			return tree->rank(x);
		}

		// tree_rank1: return #1's until n inclusive.
		size_t tree_rank1(size_t x) {
			return x - tree->rank(x) + 1;
		}
		
		// tree_select0: return x-th '0'.
		size_t tree_select0(size_t x) {
			return tree->select(x);
		}

		// Cardinal Tree Operations.
		// degree: return degree of node that begin in pos i.
		size_t degree(size_t x) {
			return tree_select0(tree_rank0(x-1) + 1) - x;	
		}
		
		// parent: return parent node's position.
		size_t parent(size_t x)	{
			size_t tmp = tree_rank0(tree->find_open(x-1));
			if (tmp == 0) return 1;
			return tree_select0(tmp) + 1;
		}

		// child: return position of i-th child (i=1..I). 
		size_t child(size_t x, size_t i) {
			return tree->find_close(tree_select0(tree_rank0(x) + 1) - i) + 1;
		}

		// preorder: given a node at position x, return his preorder number.
		size_t preorder(size_t x) {
			return tree_rank0(x - 1);
		}

		// child_rank: given a node x, return his position with respect his siblings.
		size_t child_rank(size_t x) {
			return tree_select0(tree_rank0(tree->find_open(x -1)) + 1) - tree->find_open(x - 1);
		}

		// select_node: return the position of node x with preorder j.
		size_t select_node(size_t j) {
			if (j == 0) return 1;
			return tree_select0(j) + 1;
		}

		// subtree_size: return the size of the subtree rooted at node x.
		size_t subtree_size(size_t x) {
			return (tree->find_close(tree->enclose(x)) - x)/2 + 1;
		}
		
		// ancestor: return true if x is ancestor of y.
		bool ancestor(size_t x, size_t y) {
			if (x > y) return false;
			return tree->find_close(tree->enclose(x)) >= y;
		}

		// access_data: return the data associated to node x.
		int access_data(size_t x) {
			return (*info)[tree_rank0(x-1)];
		}

		// label: return the label of the i-th child of node x. (i=1..I).
		uint8_t label(size_t x, size_t i) {
			// menos 1: porque los indices empiezan de 0 y hay 1 simbolo menos que el preorden del nodo.
			// y menos 1: por el dummy.
			return (*letts)[tree_rank1(x - 1) + i + - 2] ; 
		}

		// binary_search: do binary search in the string seq, between left and right (inclusive)
		// seeking an alpha.
		int binary_search(int_vector<> *seq, int left, int right, uint8_t alpha) {
			int mid;
			while (left <= right) { 
				mid = (left+right)/2; 
				if ((*seq)[mid] < alpha) left = mid+1; 
				else if ((*seq)[mid] > alpha) right = mid-1; 
				else return mid;
			}
			return -1;
		}

		// binary_label_child: return the position of the child of node x, labeled with alpha.
		// This do binary search over the sequence of symbols seq.
		size_t label_child(size_t x, uint8_t alpha) {
			// symbols_previous_count: # simbolos predecesores. Que en el arreglo de simbolos
			// corresponde a la posicion donde empiezan los simbolos del nodo x.
			size_t position_symbols_begin;
			if (x == 1) {
				position_symbols_begin = 0;
			}
			else position_symbols_begin = tree_rank1(x - 1) - 1; 
			size_t position_symbols_end;
			position_symbols_end = tree_rank1(tree_select0(tree_rank0(x - 1) + 1)) - 2;
			size_t i = binary_search(letts, position_symbols_begin, position_symbols_end, alpha);
			i = i-position_symbols_begin+1;
			return child(x, i); 
		}


		// get_binary_size: Return the size of the whole cardinal tree, including bp structure
		// Here is not use a rank/select structure for symbols, so it's not considered in size.
		size_t get_size() {
			cout << size_in_bytes(*tree) << "|" << (*letts).size() << "|";
			//return letts.size() + sizeof(*tree) + sizeof(*tree_s1);
			return 0;
		}

		// get_tree_size: Return size in bytes of the structure that support parentheses.
		size_t get_tree_size() {
			return size_in_bytes(*tree);
		} 

		// get_letts_size: return size in bytes of the symbols.
		size_t get_letts_size() {
			return size_in_bytes(*letts);
		}
		~cardinal_tree_bs() {delete tree;}
};


#endif
