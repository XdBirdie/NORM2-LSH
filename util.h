//
// Created by rain on 2022/1/19.
//

#ifndef LSH_UTIL_H
#define LSH_UTIL_H

#include <random>
#include <iostream>
#include <fstream>
#include <limits>
#include <map>

template <typename DType>
inline DType gaussian(double mean=0, double sigma=1) {
    static std::mt19937 mt(std::random_device{}());
    static std::normal_distribution<DType> nd;
    return mean + nd(mt) * sigma;
}

template <typename DType>
inline DType uniform(double l, double r) {
    static std::mt19937 mt(std::random_device{}());
    static std::uniform_real_distribution<DType> urd(0, 1);
    return urd(mt) * (r - l) + l;
}

template <typename DType>
DType calc_ip(int d, const DType *a, const DType *b) {
    DType res = 0;
    for (int i = 0; i < d; ++i)
        res += DType(a[i] * b[i]);
    return res;
}

template <typename DType>
DType calc_norm2_dis(int d, const DType *a, const DType *b) {
    DType res = 0;
    for (int i = 0; i < d; ++i) {
        auto t = a[i] - b[i];
        res += DType(t * t);
    }
    return res;
}

template <typename DType>
DType neg_data(int d, DType *a) {
    for (int i = 0; i < d; ++i) a[i] = -a[i];
}

using cpp_str = std::string;
std::map<cpp_str, cpp_str> read_config(const cpp_str &path) {
    std::map<cpp_str, cpp_str> res;

    std::ifstream in(path);
    if (!in) {
        std::cout << "- In file util.h\n"
                  << "  - in function read_config\n"
                  << "  Could not open file: " << path << '\n';
        return res;
    }
    cpp_str key, value;
    while (!in.eof()) {
        in >> key >> value;
        res[key] = value;
    }
    return res;
}

//template <typename DType>
//DType calc_dis()

template <typename DType>
struct Result {
    int id;
    DType value;

    Result():id(0) {}
    Result(int id, DType value):id(id), value(value) {}
//    bool operator<(const Result<DType> &y) const {
//        return value > y.value;
//    }

    using os = std::ostream;
    friend os &operator<<(os &out, const Result<DType> &res) {
        out << res.id << ' ' << res.value;
        return out;
    }

    using ofs = std::ofstream;
    friend ofs &operator<<(ofs &out, const Result<DType> &res) {
        out << res.id << ' ' << res.value;
        return out;
    }
};

/**
 * defs
 */
constexpr double MAX_REAL = std::numeric_limits<double>::max();
constexpr double MIN_REAL = std::numeric_limits<double>::min();
constexpr double CHECK_ERROR = 1e-6;
//constexpr int

#endif //LSH_UTIL_H
