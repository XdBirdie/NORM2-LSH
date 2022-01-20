//
// Created by rain on 2022/1/19.
//

#include <iostream>
#include <fstream>
#include <limits>
using namespace std;

int main() {
    auto path = "/home/rain/Project/LSH/out.txt";
    fstream out(path, ios::out | ios::binary);
    int a[] = {1, 2, 3, 4, 5};
    cout << (sizeof a) << '\n';
    out.write((char *)a, sizeof a);
    out.close();

    fstream in(path, ios::in | ios::binary);
    int *b = new int[10];
    in.read((char *)b, 40);
//    in >> b[0] >> b[1] >> b[2] >> b[3];
    in.close();
    for (int i = 0; i < 10; ++i) cout << b[i] << ' ';

    return 0;
}
