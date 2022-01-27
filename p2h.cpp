//
// Created by rain on 2022/1/26.
//

#include <iostream>
#include "data.h"
#include "LSH.h"
#include "result.h"
using namespace std;

template <typename DType>
DType calc_transform_data_norm(int d, const DType *data) {
    DType n2 = 0, n4 = 0, tmp;
    for (int i = 0; i < d; ++i) {
        tmp = data[i] * data[i];
        n2 += tmp; n4 += tmp * tmp;
    }
    return (n2 * n2 + n4) / 2.0;
}

template <typename DType>
void transform_data(DType M2, DType norm2, int d, const DType *src, DType *tar) {
    int c = 0;
    while (c < d) tar[c] = src[c] * src[c], ++c;
    for (int i = 0; i < d; ++i) {
        for (int j = i + 1; j < d; ++j)
            tar[c++] = src[i] * src[j];
    }
    tar[c] = std::sqrt(M2 - norm2);
}

template <typename DType>
DType preprocess_data(int n, int d, const DType *src, DType *tar) {
    auto *norm2 = new DType[n];
    DType M2 = 0;
    for (int i = 0; i < n; ++i) {
        norm2[i] = calc_transform_data_norm(d, &src[i * d]);
        M2 = std::max(norm2[i], M2);
    }
    int d_t = d * (d + 1) / 2 + 1;
    for (int i = 0; i < n; ++i) {
        transform_data(M2, norm2[i], d, src + i*d, tar + i*d_t);
    }
    delete[] norm2;
    return M2;
}

template <typename DType>
DType calc_transform_query_norm(int d, const DType *query) {
    DType n2 = 0, n4 = 0, tmp;
    for (int i = 0; i < d; ++i) {
        tmp = query[i] * query[i];
        n2 += tmp; n4 += tmp * tmp;
    }
    return 2 * n2 * n2 - n4;
}

template <typename DType>
void transform_query(DType M2, int d, const DType *src, DType *tar) {
    int c = 0;
    auto norm2 = calc_transform_query_norm(d, src);
    while (c < d) tar[c] = src[c] * src[c], ++c;
    for (int i = 0; i < d; ++i) {
        for (int j = i + 1; j < d; ++j)
            tar[c++] = 2 * src[i] * src[j];
    }
    auto lambda = -std::sqrt(M2 / norm2);
    for (int i = 0; i< c; ++i)
        tar[i] = tar[i] * lambda;
}

template <typename DType>
void normalize_query(int d, const DType *src, DType *tar) {
    DType norm = std::sqrt(calc_ip(d - 1, src, src));
//    cout << norm <i< "---- ";
    for (int i = 0; i < d; ++i) {
        tar[i] = src[i] / norm;
//        std::cout << src[i] << ' ';
    }
//    cout << '\n';
}

int main() {
    string path = "/home/rain/Project/LSH/data/Yelp";
    DataSet<float> yelp(path);
    int n = yelp.get_n(), qn = yelp.get_qn();
    int d = yelp.get_d(), k = yelp.get_top_k();
    int d_t = d * (d + 1) / 2 + 1;

    auto data = yelp.get_data_set();
    auto query = yelp.get_query_set();
    auto gt = yelp.get_gt_set();

    auto data_t = new float[n * d_t];
    auto M2 = preprocess_data(n, d, data, data_t);

    auto query_n = new float[d * qn];
    auto query_t = new float[d_t * qn];
    for (int i = 0; i < qn; ++i) {
        normalize_query(d, &query[i * d], &query_n[i * d]);
        transform_query(M2, d, &query_n[i * d], &query_t[i * d_t]);
    }

    float w = 0.1;
    LSH<float, 128> lsh(n, d_t, w, data_t);

    MinQueue<float> mq(k);
    Record<float> record(n, "Yelp");
    int cands = 70000;
    for (int i = 0; i < qn; ++i) {
        mq.clear();
        auto q = &query_t[i * d_t];

        lsh.for_candidates(cands, q, [&](int id) {
            const float *point = &data[id * d];
            float dis = std::abs(calc_ip(d, point, &query_n[i * d]));

            mq.insert(id + 1, dis);
        });
        cout << i << ": " << mq << '\n';
        auto[recall, precision, ratio, fraction] = record(k, cands, &gt[i*k], mq);
//        cout << i << ": " << recall << endl;
    }
    cout << record << '\n';

    delete[] query_t;
    delete[] query_n;
    delete[] data_t;
}