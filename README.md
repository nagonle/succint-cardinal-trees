Implementation of Compact Cardinal Trees
========================================

In this code, it is proposes a implementation of compact cardinal trees.

Dependencies:
============
- g++
- Default installation of sdsl-lite: https://github.com/simongog/sdsl-lite


Build:
=====
	Make

Run:
===
	./wt_main {data_structure} {tree-path} [uint32_t]

- data_structure: ap | gmr | ls | bs | wth | wth32 | wt | wt32
- This structures use 8 bit to encoding alphabet. wth32 and wt32 use 32 bits to do it.

Example:
=======
	./wt_main gmr mytree

How trees are readed:
====================

- There are two ways to encode the trees into the files.

- There are two files. One with a DFUDS representation of the tree, called {name}.ascii. Is one line with all 2n parentheses.

- The other one is called {name}.letts and this have the next format:

	**{total_nodes}{0}{all_symbols}**

- all_symbols = total_nodes - 1

- if uint32_t is passed as parameter, total_nodes is readed like a uint64_t, then read 32 bits each time. The first read is the zero, then begins the symbols.

- if not, then the symbols and the 0, are readed like a uint8_t, using 8 bits bits of space each. That means, the first 32 bits of the file contain the number, the next 8 bits contain a zero, and then begin the symbols of the tree, 8 bits each time.