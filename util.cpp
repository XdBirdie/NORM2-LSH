//
// Created by rain on 2022/1/19.
//

#include "util.h"
#include <random>

void test_save_config() {
    cpp_str path = "/home/rain/Project/LSH/data/gen/test-config";
    Config config;
    config["name"] = "rain";
    config["university"] = "XDU";
    config["grade"] = "100";

    save_config(path, config);
}

int main() {
//    test_save_config();

    Result<double> r(1, 10.5);
    std::cout << r << "\n";
    auto p = Result<int>(r);
    std::cout << p << "\n";

//    double(1);
    return 0;
}