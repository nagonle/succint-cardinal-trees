#include <iostream>
#include <sdsl/bit_vectors.hpp>
#include <sdsl/bp_support.hpp>

using namespace std;
using namespace sdsl;

int main()
{
	bit_vector b = bit_vector(8000, 0);
	for (size_t i=0; i < b.size(); i+=100)
		b[i] = 1;
	rank_support_v<1> b_rank(&b);
	for (size_t i=0; i<=b.size(); i+= b.size()/4)
		cout << "(" << i << ", " << b_rank(i) << ") ";
	cout << endl;


	//bit_vector b = {0,1,0,1};
	b = {0,1,0,1};
	rank_support_v<1> b_r1(&b);
	rank_support_v<0> b_r0(&b);
	rank_support_v<10,2> b_r10(&b);
	rank_support_v<01,2> b_r01(&b);
	for (size_t i=0; i<=b.size(); ++i)
		cout << i << ": "<< b_r1(i) << " " << b_r0(i)
			<< " " << b_r10(i) << " " << b_r01(i) << endl;

	//bit_vector b = {0,1,0,1,1,1,0,0,0,1,1};
	cout << "select support 0: " << endl;
	b = {0,1,0,1,1,1,0,0,0,1,1};
	for (size_t i=0; i<b.size(); i++) cout << b[i] << " "; cout << endl;
	size_t zeros = rank_support_v<0>(&b)(b.size());
	bit_vector::select_0_type b_sel(&b);

	for (size_t i=1; i <= zeros; ++i) {
		cout << b_sel(i) << " ";
	}
	cout << endl;


	//              0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9
	//              ( ( ( ) ( ) ) ( ) ( ( ( ) ( ) ) ( ) ) )
	//bit_vector b = {1,1,1,0,1,0,0,1,0,1,1,1,0,1,0,0,1,0,0,0};
	b = {1,1,1,0,1,0,0,1,0,1,1,1,0,1,0,0,1,0,0,0};
	bp_support_sada<> bps(&b); // <- pointer to b
	cout << bps.find_close(0) << ", "
		<< bps.find_open(3) << ", "
		<< bps.enclose(4) << ", "
		<< bps.double_enclose(13, 16) << endl;

	{
		cout << "excess - rank - select" << endl;
		//              0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9
		//              ( ( ( ) ( ) ) ( ) ( ( ( ) ( ) ) ( ) ) )
		bit_vector b = {1,1,1,0,1,0,0,1,0,1,1,1,0,1,0,0,1,0,0,0};
		cout << "Vector" << endl; for (size_t i=0; i<b.size(); i++) cout << b[i] << " "; cout << endl;
		//uint32_t t_sml_blk = 256;
        //uint32_t t_med_deg = 32;
        //class t_rank       = rank_support_v5<>;
        //class t_select     =
		bit_vector::select_0_type bps_sel(&b);
		bp_support_sada<256u, 32u, rank_support_v5<>, bit_vector::select_0_type> bps(&b); // <- pointer to b
		for (size_t i=0; i < b.size(); ++i)
			cout << bps.excess(i)<< " ";
		cout << endl;
		cout << "excess(5): " << bps.excess(5) << endl;
		cout << "rank(5): " << bps.rank(5) << endl // inclusive rank for BPS!!!
			<< "select(4): " << bps.select(4) << endl
			<< "select0(4): " << bps_sel(4) << endl
		    << "preceding_closing_parentheses: " << 5 - bps.rank(5) << endl
			<< endl;
	}
	
	{
	rank_support_v<1> rank_1s(&b);
	cout << "rank_1s(5): " << rank_1s(5) << endl;
	cout << "rank 1's y 0's" << endl;
	b = {0,1,0,1,1};
	rank_support_v<1> b_r1(&b);
	rank_support_v<0> b_r0(&b);
	cout << "bit_vector: " << b << endl;
	cout << "b_r1(4):" << b_r1(5) << endl;
	cout << "b_r0(4):" << b_r0(5) << endl;
	cout << endl;
	}
	{
		cout << "Select operations" << endl;
		b = {0,1,0,1,1,1,0,0,0,1,0,1,0,0,1,0,0,1};
		bit_vector::select_0_type b_s0(&b);
		select_support_mcl<> b_s1(&b);
		cout << "            "; for (size_t i=0; i<b.size(); i++) cout << i%10; cout << endl;
		cout << "bit_vector: " << b << endl;
		cout << "b_s1(5): " << b_s1(5) << endl;
		cout << "b_s0(9): " << b_s0(9) << endl;
	}
}
