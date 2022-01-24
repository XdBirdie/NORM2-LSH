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
struct Result {
    int id;
    DType value;

    Result():id(0) {}
    Result(int id, DType value):id(id), value(value) {}

    template<class T>
    explicit operator Result<T>() {return Result<T>(id, T(value));}

    /**
     * Define standard input and output
     */
    using os = std::ostream;
    friend os &operator<<(os &out, const Result<DType> &res) {
        out << res.id << ' ' << res.value;
        return out;
    }

    /**
     * Define file input and output
     */
    using ifs = std::ifstream;
    friend ifs &operator>>(ifs &in, Result<DType> &res) {
        in >> res.id >> res.value;
        return in;
    }

    using ofs = std::ofstream;
    friend ofs &operator<<(ofs &out, const Result<DType> &res) {
        out << res.id << ' ' << res.value;
        return out;
    }
};


template <typename DType, typename F>
void for_data(DType *data, int len, const F &f) {
    for (int i = 0; i < len; ++i)
        f(data[i]);
}

/**
 * random number generator with corresponding to distribution
 */
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

/**
 * Utility functions for manipulating data
 */
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

/**
 * Converts the element type in array src to TarType
 */
template <typename SrcType, typename TarType>
void cast_data(int d, SrcType *src, TarType *tar) {
    while (d--) {
        *tar++ = (TarType) *src++;
    }
}

/**
 * read/write config file
 */
using cpp_str = std::string;
using Config = std::map<std::string, std::string>;
Config read_config(const cpp_str &path) {
    Config config;
    std::ifstream in(path);
    if (!in) {
        std::cout << "- In file util.h\n"
                  << "  - in function read_config\n"
                  << "  Could not open file: " << path << '\n';
        return config;
    }
    cpp_str key, value;
    while (!in.eof()) {
        in >> key >> value;
        config[key] = value;
    }
    return config;
}

bool save_config(const cpp_str &path, const Config &config) {
    std::ofstream out(path);
    if (!out) {
        std::cout << "- In file util.h\n"
                  << "  - in function read_config\n"
                  << "  Could not open file: " << path << '\n';
        return false;
    }
    for (auto [key, value] : config) {
        out << key << ' ' << value << '\n';
    }
    return true;
}

/**
 * defs
 */
constexpr double MAX_REAL = std::numeric_limits<double>::max();
constexpr double MIN_REAL = std::numeric_limits<double>::min();
constexpr double CHECK_ERROR = 1e-6;
//constexpr int

#endif //LSH_UTIL_H
