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

template<typename DType>
bool save_data(const string &path, DType *data, int len) {
    std::fstream out(path, ios::out | ios::binary);
    if (!out) {
        return false;
    }
    out.write((char *) data, len * sizeof(DType));
    out.close();
    return true;
}

template<typename DType>
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
bool load_gt(const string &path, Result<DType> *gt, int n, int k) {
    std::cerr << path << "\n";
    std::ifstream in(path);
    if (!in) {return false;}
    int id;
    char c;
    for (int i = 0; i < n; ++i) {
        in >> id >> c;
        Result<DType> *tmp = &gt[id * k];
        for (int j = 0; j < k; ++j) {
            in >> tmp[j] >> c;
        }
    }
    in.close();
    return true;
}

template <typename DType>
bool save_gt(const string &path, Result<DType> *gt, int n, int k) {
    std::ofstream out(path);
    if (!out) {return false;}
    for (int i = 0; i < n; ++i){
        out << i << ": ";
        Result<DType> *tmp = &gt[i * k];
        for (int j = 0; j < k; ++j) {
            out << tmp[j] << ", ";
        }
    }

    return true;
}


template<typename DType>
class Generator {
public:
    Generator(int n, int d) : n(n), d(d) {
        data = new DType[n * d];
        for (int i = 0; i < n; ++i) {
            generate(data + i * d);
        }
    }

//    Generator(int n, int d, )
    ~Generator() { delete[] data; }

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
        save_data(path, data, n * d);
    }

private:
    DType *data;
    int n, d;
};

template<typename DType>
class DataSet {
private:

public:
    using GT = Result<DType>;

    DataSet() = default;

    explicit DataSet(const string &dataset_path) { load_dataset(dataset_path); }

    ~DataSet() {
        delete[] data_set;
        delete[] query_set;
        delete[] gt_set;
    }

    bool parse_config(Config conf) {
        auto parser = [&](int &x, const string &t) -> bool {
            if (conf[t].empty()) return false;
            x = std::stoi(conf[t]);
            return true;
        };
        if (!parser(n_, "n")) return false;
        if (!parser(qn_, "qn")) return false;
        if (!parser(d_, "d")) return false;
        if (!parser(top_k_, "top_k")) return false;

        if (conf["dataset_name"].empty()) return false;
        dataset_name = conf["dataset_name"];
        return true;
    }

    Config generate_config() const {
        Config config;
        config["n"] = std::string(n_);
        config["qn"] = std::string(qn_);
        config["d"] = std::string(d_);
        config["top_k"] = std::string(top_k_);

        config["dataset_name"] = dataset_name;
        return config;
    }

    void load_dataset(const string &dataset_path) {
        path = dataset_path;

        auto config_path = dataset_path + "/config";

        auto conf = read_config(config_path);
        if (!parse_config(conf)) {
            std::cerr << "The required parameters in the configuration file are incomplete!" << std::endl;
            return;
        }

        auto temp = dataset_path + "/" + dataset_name;
        auto ds_path = temp + ".ds";
        auto qs_path = temp + ".q";
        auto gt_path = temp + ".gt";

        data_set = new DType[n_ * d_];
        query_set = new DType[qn_ * d_];
        gt_set = new GT[qn_ * top_k_];

        load_data(ds_path, data_set, n_ * d_);
        load_data(qs_path, query_set, qn_ * d_);
        load_gt(gt_path, gt_set, n_, top_k_);
    }

    void save_to(const string &out_path, const string &ds_name) const {
        auto conf_path = out_path + "/config";
        auto temp = out_path + "/" + ds_name;
        auto ds_path = temp + ".ds";
        auto qs_path = temp + ".q";
        auto gt_path = temp + ".gt";

        save_config(conf_path, generate_config());
        save_data(ds_path, data_set);
        save_data(qs_path, query_set);
        save_gt(gt_path, gt_set, n_, top_k_);
    }

    // getters
    int get_n() const { return n_; }
    int get_qn() const { return qn_; }
    int get_d() const { return d_; }
    int get_top_k() const { return top_k_; }

    const DType *get_data_set() const { return data_set; }
    const DType *get_query_set() const { return query_set; }
    const GT *get_gt_set() const { return gt_set; }

    const DType *get_data(int i) const { return &data_set[i * d_]; }
    const DType *get_query(int i) const { return &query_set[i * d_]; }
    const GT *get_gt(int i) const { return &gt_set[i * top_k_]; }

    std::tuple<const DType *, const GT *>
    get_query_pair(int i) const {
        return std::make_tuple(get_query(i), get_gt(i));
    }

private:
    int n_{}, qn_{}, d_{}, top_k_{};
    string dataset_name;
    string path;

    DType *data_set;
    DType *query_set;
    GT *gt_set;
};

#endif //LSH_DATA_H
