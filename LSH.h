//
// Created by rain on 2022/1/18.
//

#ifndef LSH_LSH_H
#define LSH_LSH_H

#include <algorithm>
#include <random>
#include <vector>
#include "util.h"
#include "list.h"


template <typename SigType>
class Hash {
public:
    Hash():d(0), w(0), proj_a(nullptr), proj_b(0) {}

    Hash(unsigned d, double w):d(d), w(w) {
        proj_a = new double[d];
        for (int i = 0; i < d; ++i) proj_a[i] = gaussian<double>();
        proj_b = uniform<double>(0, w);
    }

    void init(unsigned d_, double w_) {
        delete[] proj_a;
        d = d_, w = w_;
        proj_a = new double[d];
        for (int i = 0; i < d; ++i) {
            proj_a[i] = gaussian<double>();
        }
        proj_b = uniform<double>(0, w);
    }

    ~Hash() { delete[] proj_a;}

    template<typename Dtype>
    SigType operator()(Dtype *data) const {
        double res = proj_b;
        for (int i = 0; i < d; ++i) {
            res += proj_a[i] * data[i];
        }
        return SigType(std::abs(res / w));
    }

private:
    unsigned d;
    double w;
    double *proj_a;
    double proj_b;
};

template <typename SigType, unsigned M>
class Signature {
public:
    Signature() { sigs = new SigType[M]; }

    ~Signature() { delete[] sigs; }

    template<class DType>
    Signature(const DType *data, const Hash<SigType> *hashes) {
        sigs = new SigType[M];
        for (int i = 0; i < M; ++i) {
            sigs[i] = hashes[i](data);
        }
    }

    template<class DType>
    void calc_sig(const DType *data, const Hash<SigType> *hashes) {
        for (int i = 0; i < M; ++i) {
            sigs[i] = hashes[i](data);
        }
    }

    int match(const Signature<SigType, M>& y) const {
        int cnt = 0, res = 0;
        for (int i = 0; i < M; ++i) {
            if (sigs[i] == y[i]) {
                ++cnt;
                res = std::max(res, cnt);
            } cnt = 0;
        }
        int i = 0, j = M - 1;
        while (i < j && sigs[i] == y[i]) ++i;
        while (i < j && sigs[j] == y[j]) --j;
        res = std::max(res, int(i + M - 1 - j));
        return res;
    }

    int match2(const Signature<SigType, M> &y) const {
        int cnt = 0;
        for (int i = 0; i < M; ++i)
            cnt += sigs[i] == y[i];
        return cnt;
    }

    SigType operator[](int i) const {
        return sigs[i];
    }
    SigType &operator[](int i) {
        return sigs[i];
    }

    friend std::ostream &operator<<(std::ostream &out, const Signature<SigType, M> &y) {
        for (int i = 0; i < M; ++i) {
            out << y[i] << ' ';
        }
        return out;
    }

private:
    SigType *sigs;
};


template <typename DType, unsigned M>
class LSH {
public:
    using SigType = int;

    LSH(int n, int d, double w, const DType *data): n(n), d(d), data(data) {
        hashes = new Hash<SigType>[M];
        for (int i = 0; i < M; ++i) {
            hashes[i].init(d, w);
        }

        std::cout << "hashed function build over!" << std::endl;

        data_sig = new Signature<SigType, M>[n];
        for (int i = 0; i < n; ++i) {
            data_sig[i].calc_sig(&data[i*d], hashes);
        }

        rks = new Item[n];
    }

    ~LSH() {
        delete[] hashes;
        delete[] data_sig;
        delete[] rks;
    }

    template<class F>
    void for_candidates(int steps, const DType *query, const F &f) const {
        auto query_sig = Signature<SigType, M>(query, hashes);

        for (int i = 0; i < n; ++i) {
            rks[i].id = i;
            rks[i].value = query_sig.match2(data_sig[i]);
        }

        std::sort(rks, rks + n, [](Item x, Item y) {return x.value > y.value; });

        for (int i = 0; i < std::min(n, steps); ++i) {
            f(rks[i].id);
        }
    }

    int top_k(int steps, const DType *query, MinQueue<DType> &list) const {
        int cnt = 0;
        for_candidates(steps, query, [&](int id) {
            auto candidate = &data[id*d];
            auto dis = calc_norm2_dis(d, query, candidate);
            list.insert(id, dis);
            ++cnt;
        });
        return cnt;
    }

    using Item = Result<int>;

private:
    int n, d;
    const DType *data;
    Signature<SigType, M> *data_sig;
    Hash<SigType> *hashes;

    Item *rks;
};

#endif //LSH_LSH_H
