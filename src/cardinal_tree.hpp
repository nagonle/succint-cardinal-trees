#ifndef INCLUDED_CARDINAL_TREE
#define INCLUDED_CARDINAL_TREE

#include <sdsl/bit_vectors.hpp>
#include <sdsl/bp_support.hpp>
#include <sdsl/wavelet_trees.hpp>
#include <sdsl/int_vector.hpp>

//#define t_sml_blk 1024u
//#define t_med_deg 2048u
#define t_sml_blk 4096u
#define t_med_deg 8192u
using namespace std;
using namespace sdsl;


template <class seq_type, typename size_type>
class cardinal_tree 
{
	private:
		// Symbol sequence.
		seq_type *letts; 
		string chars;
		// Tree sequence (DFUDS) 
		bp_support_sada<t_sml_blk, t_med_deg, rank_support_v5<0>, bit_vector::select_0_type> *tree; 
		vector<int> *info; 
		size_t nodes;
		bit_vector *b;
  	public:
		cardinal_tree(int_vector<> seq_, bit_vector * bp, vector<int> * info_) {
			nodes = (*bp).size() / 2;
			letts = new seq_type();

			// Init sequence.
			construct_im(*letts, seq_, 0); 
			// Init tree BP.
			tree = new bp_support_sada<t_sml_blk, t_med_deg, rank_support_v5<0>, bit_vector::select_0_type>(bp); 

			info = info_;
			b = bp;
		}

		cardinal_tree(string seq_, bit_vector * bp, vector<int> * info_) {
			nodes = (*bp).size() / 2;
			letts = new seq_type();

			// Init sequence.
			construct_im(*letts, seq_, 1); 
			// Init tree BP.
			tree = new bp_support_sada<t_sml_blk, t_med_deg, rank_support_v5<0>, bit_vector::select_0_type>(bp); 

			info = info_;
			b = bp;
		}

		size_type get_symbol(size_t x) {
			return (*letts)[x];
		}

		char get_bp(size_t x) {
			if ((*b)[x] == 1) return '(';
			return ')';
		}

		size_t get_bp_count() {
			return tree->size();
		}

		size_t get_letts_count() {
			return letts->size();
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

		// Symbols operations.
		// label_rank: return count of char s until position x-1 (not inclusive!).
		size_t label_rank(size_t x, size_type s) {
			return letts->rank(x, s);
		}

		// label_select: Return position of x-th char s.
		size_t label_select(size_t x, size_type s) {
			return letts->select(x, s);
		}

		// Cardinal Tree Operations.
		// degree: return degree of node that begin in pos i.
		size_t degree(size_t x) {
			return tree_select0(tree_rank0(x - 1) + 1) - x;	
		}
		
		// parent: return parent node's position.
		size_t parent(size_t x)	{
			size_t tmp = tree_rank0(tree->find_open(x-1));
			if (tmp == 0) return 1;
			return tree_select0(tmp) + 1;
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
		size_type label(size_t x, size_t i) {
			// less 1, because index begin in 0, and there is 1 symbol less than preorder's 
			// number of the node, and less 1 because the dummy parenthese. 
			return (*letts)[tree_rank1(x - 1) + i - 2] ; 
		}

		// child: return position of i-th child (i=1..I). 
		size_t child(size_t x, size_t i) {
			return tree->find_close(tree_select0(tree_rank0(x) + 1) - i) + 1;
		}

		// label_child: return the position of the child of node x, labeled with alpha.
		size_t label_child(size_t x, size_type alpha) {
			// symbols_previous_count: # predecessor symbols. In symbols array this represent
			// position where begin the symbols of node x.
			size_t position_symbols_begin;
			if (x == 1) {
				position_symbols_begin = 0;
			}
			else position_symbols_begin = tree_rank1(x - 1) - 1; 
			size_t alpha_previous_count; 
			if (position_symbols_begin == 0) alpha_previous_count = 0;
			else alpha_previous_count = label_rank(position_symbols_begin, alpha);
			// position_next_alpha: position of next apparition of searched alpha.
			size_t position_alpha = label_select(alpha_previous_count + 1, alpha); 
			int position_symbols_end = position_symbols_begin + degree(x) - 1;
			if (!(position_alpha >= position_symbols_begin && position_alpha <= position_symbols_end)) {
				return 0;
			}
			// substract both positions, where symbols begin and the position of searched alpha.
			size_t i = position_alpha - position_symbols_begin + 1;
			return child(x, i); 
		}

		// get_size: Return the size of the whole cardinal tree, including bp structure
		// rank/select structure for symbols.
		size_t get_size() {
			return get_tree_size() + get_letts_size();
		}

		// get_tree_size: Return size in bytes of the structure that support parentheses.
		size_t get_tree_size() {
			return size_in_bytes(*tree);
		} 

		// get_letts_size: return size in bytes of the symbols.
		size_t get_letts_size() {
			return size_in_bytes(*letts);
		}

		~cardinal_tree() {delete tree; delete letts;}
};


#endif
