//
// Created by rain on 2022/1/22.
//

#include <iostream>
#include "data.h"
using namespace std;


void test1() {
    string path = "/home/rain/Project/LSH/data/Yelp";
    DataSet<float> ds(path);
    int n = ds.get_n(), d = ds.get_d();
    int qn = ds.get_qn(), k = ds.get_top_k();
    cout << "n:  " << n << '\n'
         << "d:  " << d << '\n'
         << "qn: " << qn << '\n'
         << "k:  " << k << '\n';

    auto *gt = ds.get_gt_set();
    for (int i = 0; i < qn; ++i) {
        cout << i << ": ";
        for (int j = 0; j < k; ++j) {
            cout << gt[i*k + j] << ", ";
        }
        cout << endl;
    }

    auto *data_set = ds.get_data_set();
    for (int i = 0; i < n; ++i) {
        cout << i << ": ";
        for (int j = 0; j < d; ++j) {
            cout << data_set[i * d + j] << " ";
        }
        cout << endl;
    }


    auto *query_set = ds.get_query_set();
    for (int i = 0; i < qn; ++i) {
        cout << i << ": ";
        for (int j = 0; j < d; ++j) {
            cout << query_set[i * d + j] << " ";
        }
        cout << endl;
    }
}

void test2() {
    string a = "12321";
    const string & b = a;
    cout << typeid(b).name() << " " << typeid(a).name() << endl;
    cout << stoi(b) << '\n';
    string path = "/home/rain/Project/LSH/data/gen";
    DataSet<double> dataSet(path);
    cout << dataSet.get_n() << '\n';
}

void test3() {
    const string path = "/home/rain/Project/LSH/data/Yelp/Yelp.gt";
    const string out = "/home/rain/Project/LSH/data/Yelp/Yelp_2.gt";
    int qn = 100, k = 100;
    auto *gt = new Result<float>[qn*k];
    load_gt_p2h(path, gt, qn, k);
    save_gt(out, gt, qn, k);
}

int main() {
    test1();
    return 0;
}