#include "ImpBTree.cpp"
#include "FunBTree.cpp"
#include <iostream>
#include <iterator>
#include <map>
#include <typeinfo>
#include <x86intrin.h>
#include <numeric>
#include <random>
#include <algorithm>

//#define N 16
//#define N (1 << 20)
# define N 128

using ullong = unsigned long long;

//void print_int_vector(std::vector<int> vec) {
	//for (int e: vec) {
		//std::cout << std::to_string(e) << ", ";
	//}
	//std::cout << std::endl;
//}

//void print_str_vector(std::vector<std::string> vec) {
	//for (std::string e: vec) {
		//std::cout << e << ", ";
	//}
	//std::cout << std::endl;
//}

std::vector<int> generate_keys(int n) {
	std::vector<int> acc = std::vector<int>(n);

	for (int i = 0; i < n; i++) {
		acc.push_back(rand() % 1000);
	}

	return acc;
}

std::vector<std::string> generate_vals(int n) {
	std::vector<std::string> acc = std::vector<std::string>(n);

	for (int i = 0; i < n; i++) {
		acc.push_back(std::to_string(rand() % 1000000));
	}

	return acc;
}

void prelim_test() {
	ImpDict *imp_dict = new ImpDict(3);
	imp_dict->insert(5, "five");
	imp_dict->insert(2, "two");
	imp_dict->insert(3, "three");
	imp_dict->insert(1, "one");
	imp_dict->insert(15, "fifteen");
	imp_dict->insert(13, "thirteen");
	imp_dict->insert(12, "twelve");
	imp_dict->insert(11, "eleven");

	FunDict *fun_dict = new FunDict(3);
	fun_dict->insert(5, "five");
	fun_dict->insert(2, "two");
	fun_dict->insert(3, "three");
	fun_dict->insert(1, "one");
	fun_dict->insert(15, "fifteen");
	fun_dict->insert(13, "thirteen");
	fun_dict->insert(12, "twelve");
	fun_dict->insert(11, "eleven");
}

void print_result(int i, std::string type, std::string impl, ullong begin, ullong end) {
    std::cout << i << "," << type << "," << impl << "," << end - begin << std::endl;
}

int main(int argc, char *argv[]) {
	int ns[5] = { 128, 4096, 65536, 262144, 524288 };
	int bs[5] = { 2, 3, 8, 16, 100 };

	/**
	  I indicates which i-th element is being inserted.
	  Type in {dict, seq}.
	  Implementation in {imp,fun}
	  Latency indicates how many cycles it took since first calling insert.
	  */
	std::cout << "i,type,implementation,latency" << std::endl;

	for (int n: ns) {
		std::vector<int> test_keys = generate_keys(n);
		std::vector<std::string> test_values = generate_vals(n);

		for (int b: bs) {
			FunDict *fd = new FunDict(b);
			for (int i = 0; i < n; i++) {
				ullong begin = __rdtsc();
				fd->insert(test_keys[i], test_values[i]);
				ullong end = __rdtsc();

				print_result(i, "dict", "fun", begin, end);
			}

			ImpDict *id = new ImpDict(b);
			for (int i = 0; i < n; i++) {
				ullong begin = __rdtsc();
				fd->insert(test_keys[i], test_values[i]);
				ullong end = __rdtsc();

				print_result(i, "dict", "imp", begin, end);
			}
		}
	}

    return 0;
}
