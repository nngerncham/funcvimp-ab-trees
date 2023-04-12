#include "ImpBTree.cpp"
#include <iostream>
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

int main(int argc, char *argv[]) {
    return 0;
}
