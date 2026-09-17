#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <cstdlib>
#include <ctime>

#include "gtest/gtest.h"
#include "algs.h"
#include "tree.h"

using namespace std;
using std::string;

//------------------------------------------------------------------
// Seeding
//
// Seed exactly once, before main() runs, instead of calling
// srand(time(0)) at the top of individual tests. Two tests that
// re-seed within the same second would otherwise replay the same
// random sequence.
//
// Set the environment variable TEST_SEED to a fixed number to make a
// failing run reproducible, e.g.  TEST_SEED=1234 ./runUnitTests
//------------------------------------------------------------------
namespace {
struct SeedInit {
	SeedInit() {
		const char* env = std::getenv("TEST_SEED");
		unsigned seed = env ? (unsigned)std::atoi(env) : (unsigned)std::time(nullptr);
		std::cout << "[  SEED    ] " << seed
		          << "  (re-run with TEST_SEED=" << seed << " to reproduce)"
		          << std::endl;
		std::srand(seed);
	}
};
SeedInit seed_init_;

// std::random_shuffle was deprecated in C++14 and removed in C++17.
// std::shuffle works in C++11 and later.
void shuffle_str(std::string& s) {
	static std::mt19937 gen((unsigned)std::rand());
	std::shuffle(s.begin(), s.end(), gen);
}
} // namespace


TEST(heapsort_rand,heapsort_test){
	const int arr_sz = 20;
	for(int i = 0;i<300;++i){
		vector<int> A;
		vector<int> A_lib_sorted;
		for(int j=0;j<arr_sz;++j){
			A.push_back(rand()%15);
		}
		vector<int> orig_A;


		for(int j = 0; j<arr_sz;++j){
			orig_A.push_back(A[j]);
			A_lib_sorted.push_back(A[j]);
		}

		std::sort(A_lib_sorted.begin(), A_lib_sorted.end());

		ASSERT_NO_THROW(heapsort(A) )<<"Your code threw an exception";

		ASSERT_EQ((size_t)arr_sz, A.size())
			<< "Your code changed the size of the vector";

		string arr_cmp;
		arr_cmp+="\nA ( yours):[";
		for(int j = 0;j<arr_sz;++j) arr_cmp+=to_string(A[j])+",";
		arr_cmp+="]\n";
		arr_cmp+="\nA (target):[";
		for(int j = 0;j<arr_sz;++j) arr_cmp+=to_string(A_lib_sorted[j])+",";
		arr_cmp+="]\n";
		arr_cmp+="\nA ( orig.):[";
		for(int j = 0;j<arr_sz;++j) arr_cmp+=to_string(orig_A[j])+",";
		arr_cmp+="]\n";

		for(int j = 0;j<arr_sz;++j){
			ASSERT_EQ(A[j],A_lib_sorted[j]) << "Your code doesn't properly sort:\n"<<arr_cmp;
		}

	}	
}

struct ana_sample{
	std::string a;
	std::string b;
	bool are_anagrams;
};

ana_sample* gen_ana_sample(){
	int len = 4 + rand()%8;              // lengths 4..11
	ana_sample* s = new ana_sample;
	std::string  temp;
	for(int i = 0;i<len;++i)
		temp+=(char)((int)'A' + rand()%26) ;
	s->a=temp;
	s->are_anagrams = (rand() % 2) < 1;

	s->b=s->a;
	shuffle_str(s->b);
	if(!s->are_anagrams) {
		// Replace one character with a different one. Changing a single
		// letter always changes the multiset, so b is guaranteed not to
		// be an anagram of a.
		int ind = rand()%len;
		char diff_char =
		    (char)( ( (int)s->b[ind]-(int)'A'+rand()%25+1 )//+1..25
		    %26+(int)'A');
		s->b = s->b.substr(0,ind)+diff_char+s->b.substr(ind+1);
	}
	return s;
}



TEST(ana_checker,ana_checker){
	for (int i =0;i<100;++i){
		ana_sample* samp = gen_ana_sample();
		bool res = are_anagrams(samp->a,samp->b);
		std::string msg = "For strings\n" +
			samp->a +",\n" +
			samp->b + "\n"  +
			" your code returned " + (res?"true":"false")+ " when "+
			(samp->are_anagrams?"true":"false")+" was expected\n";
		bool expected = samp->are_anagrams;
		delete samp;
		ASSERT_TRUE(res==expected)<<msg;
	}
}

template<typename T>
BST_Node<T>* gen_tree(vector<T> vals){
    if(vals.size() == 0) return nullptr;

    BST_Node<T>* root = new BST_Node<T> (vals[0]);
    for(size_t i = 1; i<vals.size();++i)
        root->insert(vals[i]);

    return root;
};

//obviously, size had better be smaller than max-min+1
template<typename T>
vector<T> gen_no_duplicate_vec(int size, int min, int max){
    vector<T> vec;

    int rand_range = max-min+1;

    for(int i=0;i<size;++i){
        T r = T(rand()%rand_range+min);//values all integral
        bool dup= false;
        for(int j=0;j<i;++j){
            if(r==vec[j]) dup= true;
        }

        if(dup) --i;
        else vec.push_back(r);
    }

    return vec;
}

template<typename T>
BST_Node<T>* gen_count_sample(int n, int max){
    vector<T> vec = gen_no_duplicate_vec<T>(n,0,max);
    return gen_tree<T>(vec);
};

template<typename T>
BST_Node<T>* gen_dup_sample(int n, int max){
    vector<T> vec = gen_no_duplicate_vec<T>(n,0,max);
    int size = vec.size();
    int dup_ind = rand()%size;
    int repl_ind = (dup_ind+rand()%(size-1)+1)%size;
    vec[repl_ind] = vec[dup_ind];
    return gen_tree<T>(vec);
};

//------------------------------------------------------------------
// One templated body per test, called once for double and once for
// int. The two branches used to be copy-pasted, which is how the
// off-by-one below survived in only one of them for so long.
//------------------------------------------------------------------

template<typename T>
void check_count_case(int ct){
    BST_Node<T>* root = gen_count_sample<T>(ct,200);
    int student_ct = num_nodes<T>(root);
    delete_tree(root);
    ASSERT_EQ(ct,student_ct);
}

TEST(bst_count,bst_count){
    for(int i = 0; i < 100 ; ++i){
        int ct = 20+rand()%10;
        if(rand()%2) check_count_case<double>(ct);
        else         check_count_case<int>(ct);
        if(HasFatalFailure()) return;
    }
}

template<typename T>
void check_dup_case(int ct){
    bool is_dup = rand()%2;
    BST_Node<T>* root = is_dup ? gen_dup_sample<T>(ct,200)
                               : gen_count_sample<T>(ct,200);
    bool student_dup = has_duplicate_val<T>(root);
    delete_tree(root);
    ASSERT_EQ(is_dup,student_dup);
}

TEST(dup_val,dup_val){
    for(int i = 0; i < 100 ; ++i){
        int ct = 15+rand()%10;
        if(rand()%2) check_dup_case<double>(ct);
        else         check_dup_case<int>(ct);
        if(HasFatalFailure()) return;
    }
}

template<typename T>
void check_identical_case(int len){
    bool is_identical = rand()%2;

    // size is len+1, so the valid indices are 0..len. The loop below
    // uses 0..len-1, which leaves vec[len] as the spare value.
    vector<T> vec = gen_no_duplicate_vec<T>(len+1,0,200); //one extra
    vector<T> veca,vecb;
    for(int j = 0; j < len; ++j){
        veca.push_back(vec[j]);
        vecb.push_back(vec[j]);
    }
    if(!is_identical){
        // vec[len], NOT vec[len+1]. vec[len+1] is out of bounds: it
        // aborts under -D_GLIBCXX_ASSERTIONS and otherwise reads
        // uninitialized memory, which frequently made a correct
        // implementation fail this test.
        vecb[rand()%len] = vec[len];
    }
    BST_Node<T>* a = gen_tree<T>(veca);
    BST_Node<T>* b = gen_tree<T>(vecb);
    bool student_result = trees_identical<T>(a,b);
    delete_tree(a);
    delete_tree(b);
    ASSERT_EQ(is_identical,student_result);
}

TEST(tree_identical,tree_identical){
    int len = 20;
    for(int i = 0; i < 100 ; ++i){
        if(rand()%2) check_identical_case<double>(len);  // was rand()%1
        else         check_identical_case<int>(len);
        if(HasFatalFailure()) return;
    }
}
