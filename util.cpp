//
// Created by rain on 2022/1/19.
//

#include "util.h"
#include <random>

int main() {
    cpp_str path = "/home/rain/Project/LSH/data/gen/config";
    auto res = read_config(path);

    for (auto[key, value] : res) {
        std::cout << key << ' ' << value << '\n';
    }
    return 0;
}