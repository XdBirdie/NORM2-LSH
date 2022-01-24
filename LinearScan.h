//
// Created by rain on 2022/1/19.
//

#ifndef LSH_LINEARSCAN_H
#define LSH_LINEARSCAN_H

#include "util.h"
#include "list.h"

template <typename DType>
class LinearScan {
public:
    LinearScan(int n, int d, const DType *data):n(n), d(d), data(data) {}

    template<typename F>
    void for_candidates(int steps, const DType *query, const F &f) {
        for (int i = 0; i < std::min(n, steps); ++i) {
            auto dis = calc_norm2_dis(d, query, &data[i * d]);
            f(i, dis);
//            std::cout << "---" << i << '\n';
        }
    }

    template<class F>
    void for_candidates(const DType *query, const F &f) {
        for_candidates(n, query, f);
    }

    void nns(const DType *query, MinQueue<DType> &q) {
//        std::cout << "nns:" << std::endl;
        for_candidates(query, [&](int id, DType value) {
           q.insert(id, value);
        });
    }

private:
    const DType *data;
    int n, d;
};


template <typename DType>
void ground_truth(int n, int qn, int d, int k,
                  DType *ds, DType *qs, Result<DType> *gt) {
    LinearScan<DType> ls(n, d, ds);
    MinQueue<DType> mq(k);
    for (int i = 0; i < qn; ++i) {
        mq.clear();
        std::cout << i << std::endl;
        auto query = &qs[i * d];
        ls.nns(query, mq);
        mq.copy_to(&gt[i * k]);
    }
}


#endif //LSH_LINEARSCAN_H
