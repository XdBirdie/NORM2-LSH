//
// Created by rain on 2022/1/19.
//

#ifndef LSH_DATA_H
#define LSH_DATA_H

#include <string>
#include <fstream>
#include <iomanip>
#include "util.h"
#include "LinearScan.h"

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
bool load_gt(const string &path, Result<DType> *gt, int qn, int k) {
    std::ifstream in(path);
    if (!in) {return false;}
    int id;
    char c;
    for (int i = 0; i < qn; ++i) {
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
bool save_gt(const string &path, Result<DType> *gt, int qn, int k) {
    std::ofstream out(path);
    if (!out) {return false;}
    out << std::fixed << std::setprecision(10);
    for (int i = 0; i < qn; ++i){
        out << i << ": ";
        Result<DType> *tmp = &gt[i * k];
        for (int j = 0; j < k; ++j) {
            out << tmp[j] << ", ";
        }
        out << "\n";
    }

    return true;
}

template <typename DType>
bool load_gt_p2h(const string &path, Result<DType> *gt, int qn, int k) {
    std::ifstream in(path);
    if (!in) {return false;}
    int id;
    char c;
    for (int i = 0; i < qn; ++i) {
        in >> id;
        --id;
        Result<DType> *tmp = &gt[id * k];
        for (int j = 0; j < k; ++j) {
            in >> c >> tmp[j].id >> c >> tmp[j].value;
        }
    }
    in.close();
    return true;
}


template<typename DType>
class Generator {
public:
    Generator(int n, int d): n(n), d(d) {
        data = nullptr;
    }

//    Generator(int n, int d, )
    ~Generator() { delete[] data; }

    void generate(DType *a) const {
        for (int i = 0; i < d; ++i)
            a[i] = gaussian<DType>();
    }

    DType *operator()() {
        if (!data) {
            data = new DType[n * d];
            for (int i = 0; i < n; ++i) {
                generate(data + i * d);
            }
        }
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
public:
    using GT = Result<DType>;

    DataSet() = default;

    DataSet(int n, int qn, int d, int k):n_(n), qn_(qn), d_(d), top_k_(k) {
        data_set = new DType[n_ * d_];
        query_set = new DType[qn_ * d_];
        gt_set = new GT[top_k_ * qn_];

        for_data(data_set, n_ * d_, [&](DType &x) {x = gaussian<DType>();});
        for_data(query_set, qn_ * d_, [&](DType &x) {x = gaussian<DType>();});
        ground_truth(n_, qn_, d_, top_k_, data_set, query_set, gt_set);
    }

    explicit DataSet(const string &dataset_path) { load(dataset_path); }

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
        config["n"] = std::to_string(n_);
        config["qn"] = std::to_string(qn_);
        config["d"] = std::to_string(d_);
        config["top_k"] = std::to_string(top_k_);

        config["dataset_name"] = dataset_name;
        return config;
    }

    void load(const string &dataset_path) {
        delete[] data_set;
        delete[] query_set;
        delete[] gt_set;

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

        if (!load_data(ds_path, data_set, n_ * d_)) {
            std::cerr << "In " << (__FILE__) << ", " << (__FUNCTION__) << '\n';
            std::cerr << "Error when load " << ds_path << "!\n";
            return;
        }
        if (!load_data(qs_path, query_set, qn_ * d_)) {
            std::cerr << "In " << (__FILE__) << ", " << (__FUNCTION__) << '\n';
            std::cerr << "Error when load " << qs_path << "!\n";
            return;
        }
        if (!load_gt(gt_path, gt_set, qn_, top_k_)) {
            std::cerr << "In " << (__FILE__) << ", " << (__FUNCTION__) << '\n';
            std::cerr << "Error when load " << gt_path << "!\n";
            return;
        }
    }

    void save() const {
        save_to(path, dataset_name);
    }

    void save_to(const string &out_path, const string &ds_name) {
        dataset_name = ds_name;

        auto conf_path = out_path + "/config";
        auto temp = out_path + "/" + ds_name;
        auto ds_path = temp + ".ds";
        auto qs_path = temp + ".q";
        auto gt_path = temp + ".gt";

        auto config = generate_config();

        save_config(conf_path, config);
        save_data(ds_path, data_set, n_ * d_);
        save_data(qs_path, query_set, qn_ * d_);
        save_gt(gt_path, gt_set, qn_, top_k_);
    }

    template <typename T>
    void save_as(const string &out_path, const string &ds_name) const {
        auto conf_path = out_path + "/config";
        auto temp = out_path + "/" + ds_name;
        auto ds_path = temp + ".ds";
        auto qs_path = temp + ".q";
        auto gt_path = temp + ".gt";

        save_config(conf_path, generate_config());

        T *buffer = new T[std::max(n_, qn_) * d_];
        cast_data(n_ * d_, data_set, buffer);
        save_data(ds_path, buffer, n_ * d_);

        cast_data(qn_ * d_, query_set, buffer);
        save_data(qs_path, buffer, qn_ * d_);

        T *gt_buf = new Result<T>[qn_ * top_k_];
        cast_data(qn_ * top_k_, gt_set, gt_buf);
        save_gt(gt_path, gt_buf, qn_, top_k_);

        delete[] buffer;
        delete[] gt_buf;
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

    DType *data_set = nullptr;
    DType *query_set = nullptr;
    GT *gt_set = nullptr;
};


#endif //LSH_DATA_H
