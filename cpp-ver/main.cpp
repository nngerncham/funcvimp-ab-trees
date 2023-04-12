#include "ImpBTree.cpp"
#include <iostream>
#include <iterator>
#include <map>
#include <typeinfo>
#include <x86intrin.h>
#include <numeric>
#include <random>
#include <algorithm>

//#define N 16
#define N (1 << 20)

using ullong = unsigned long long;

void print_result(int i, std::string src, std::string type, ullong begin,
                  ullong end) {
    std::cout << i << "," << src << "," << type << "," << end - begin
              << std::endl;
}

void print_int_vector(std::vector<int> vec) {
	for (int e: vec) {
		std::cout << std::to_string(e) << ", ";
	}
	std::cout << std::endl;
}

void print_str_vector(std::vector<std::string> vec) {
	for (std::string e: vec) {
		std::cout << e << ", ";
	}
	std::cout << std::endl;
}


int main(int argc, char *argv[]) {
	ImpDict *imp_dict = new ImpDict(2, 3);
	imp_dict->insert(5, "five");
	imp_dict->insert(2, "two");
	imp_dict->insert(3, "three");
	imp_dict->insert(1, "one");
	imp_dict->insert(15, "fifteen");
	imp_dict->insert(13, "thirteen");
	imp_dict->insert(12, "twelve");
	imp_dict->insert(11, "eleven");
	ImpDict::print_tree(imp_dict->root.value());
	print_int_vector(imp_dict->root.value()->keys);

    return 0;
}
