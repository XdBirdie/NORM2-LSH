//
// Created by rain on 2022/1/19.
//

#ifndef LSH_DATA_H
#define LSH_DATA_H
#include <string>
#include <fstream>
#include "util.h"

using c_str = const char *;
using std::ios;
using std::string;

template <typename DType>
bool save_data(const string &path, DType *data, int len) {
    std::fstream out(path, ios::out | ios::binary);
    if (!out) {
        return false;
    }
    out.write((char *)data, len * sizeof(DType));
    out.close();
    return true;
}

template <typename DType>
bool load_data(const string &path, DType *data, int len) {
    std::fstream in(path, ios::in | ios::binary);
    if (!in) {
        return false;
    }
    in.read((char *) data, len * sizeof(DType));
    in.close();
    return true;
}


template <typename DType>
class Generator {
public:
    Generator(int n, int d):n(n), d(d) {
        data = new DType[n * d];
        for (int i = 0; i < n; ++i) {
            generate(data + i*d);
        }
    }
//    Generator(int n, int d, )
    ~Generator() {delete[] data;}

    void generate(DType *a) const {
        for (int i = 0; i < d; ++i)
            a[i] = gaussian<DType>();
    }

    DType *operator()() const {
        return data;
    }

    void copy_to(DType *a) const {
        for (int i = 0; i < n * d; ++i) a[i] = data[i];
    }

    void save_to(const std::string &path) const {
        save_data(path, data, n*d);
    }

private:
    DType *data;
    int n, d;
};

template <typename DType>
class DataSet {
public:
    using GT = Result<DType>;
    DataSet() = default;

    void load_dataset(const string &dataset_path) {
        path = dataset_path;
        auto config_path = dataset_path + "/config";

    }

private:
    std::string path;
    DType *data_set;
    DType *query_set;
    GT *ground_truth;
//    const DType *gi
};

#endif //LSH_DATA_H
