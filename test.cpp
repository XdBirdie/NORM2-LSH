//
// Created by rain on 2022/1/19.
//

#include <iostream>
#include <fstream>
#include <limits>
#include "data.h"
using namespace std;

int main() {
    string path = "/home/rain/Project/LSH/data/Yelp";
    DataSet<float> yelp(path);
    int n = yelp.get_n(), qn = yelp.get_qn();
    int d = yelp.get_d(), k = yelp.get_top_k();
    cout << "n: " << n << '\n'
         << "qn: " << qn << '\n'
         << "d: " << d << '\n'
         << "k: " << k << '\n';
    return 0;
}
