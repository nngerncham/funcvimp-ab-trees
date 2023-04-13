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

    return 0;
}
