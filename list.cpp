//
// Created by rain on 2022/1/20.
//

#include <random>
#include <algorithm>
#include "list.h"
using namespace std;

int main() {
    MinQueue<double> l1(10);
    MaxQueue<double> l2(10);

    mt19937 mt(random_device{}());
    uniform_real_distribution<double> urd(0, 1);

    double a[100];
    for (int i = 0; i < 20; ++i) {
        auto x =  urd(mt);
        l1.insert(i, x);
        l2.insert(i, x);
        a[i] = x;

        cout << i << ' ' << x << "\n";
    }

    sort(a, a + 20);
    for (int i = 0; i < 20; ++i) {
        cout << a[i] << ' ';
    }
    cout << '\n';

    cout << l1 << endl;
    cout << l2 << endl;
//    l1[10];
    return 0;
}