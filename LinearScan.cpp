//
// Created by rain on 2022/1/19.
//

#include "LinearScan.h"
#include "util.h"
#include <iostream>
using namespace std;

int main() {
    int n = 10, d = 2;
    auto *data = new double[n * d];
    for (int i = 0; i < n*d; ++i) data[i] = uniform<double>(0, 1);
    LinearScan<double> ls(n, d, data);
    for (int i = 0; i < n * d; i += 2) {
        cout << data[i] << ' ' << data[i + 1] << '\n';
    }

    MinQueue<double> res(5);
    double query[2];
    while (true) {
        cin >> query[0] >> query[1];
        ls.nns(query, res);
        cout << res << '\n';
    }

    return 0;
}