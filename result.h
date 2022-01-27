//
// Created by rain on 2022/1/20.
//

#ifndef LSH_RESULT_H
#define LSH_RESULT_H

#include <string>
#include <tuple>
#include <cmath>
#include "util.h"
#include "list.h"

template <typename DType>
class Record {
public:
    using str = const std::string &;

    explicit Record(int n, str dataset_name=""):
        n(n), dataset_name(dataset_name) {
        g_recall = g_precision = g_ratio = g_fraction = 0;
        num = 0;
    }

    double calc_ratio(int k, const Result<DType> *gt, const MinQueue<DType> &res) {
        if (res.size() < k) return sqrt(MAX_REAL);

        double sum = 0.0, r;
        for (int j = 0; j < k; ++j) {
            if (abs(res[j].value - gt[j].value) < CHECK_ERROR) r = 1.0f;
            else r = sqrt((res[j].value + 1e-9) / (gt[j].value + 1e-9));
            sum += std::log(r);
        }
        return std::exp(sum / k);
    }
    
    int count_same(int top_k, const Result<DType> *gt, const MinQueue<DType> &res) {
        auto len = res.size();
        auto a = new int[len];
        auto b = new int[top_k];

        for (int i = 0; i < len; ++i) a[i] = res[i].id;
        for (int j = 0; j < top_k; ++j) b[j] = gt[j].id;
        std::sort(a, a + len);
        std::sort(b, b + top_k);

        int cnt = 0, i = 0, j = 0;
        while ((i < len) && j < top_k) {
            if (a[i] == b[j]) ++cnt, ++i, ++j;
            else if (a[i] < b[j]) ++i;
            else ++j;
        }

        delete[] a;
        delete[] b;
        return cnt;
    }

    std::tuple<double, double, double, double> operator()(
            int top_k, int check_k,
            const Result<DType> *gt,
            const MinQueue<DType> &res
    ) {
        int cnt = count_same(top_k, gt, res);
        double recall = 100.0 * cnt / top_k;
        double precision = 100.0 * cnt / check_k;
        double ratio = calc_ratio(top_k, gt, res);

        g_recall += recall;
        g_precision += precision;
        g_ratio += ratio;
        g_fraction += 100.0 * check_k / n;

        ++num;

        return {recall, precision, ratio, 100.0 * check_k / n};
    }

    double overall_recall() const {return g_recall / num;}
    double overall_precision() const {return g_precision / num;}
    double overall_ratio() const {return g_ratio / num;}
    double overall_fraction() const {return g_fraction / num;}

    using os = std::ostream;
    friend os &operator<<(os &out, const Record &r) {
        out << r.dataset_name << "\n"
            << " - overall recall:    " << r.overall_recall() << '\n'
            << " - overall precision: " << r.overall_precision() << '\n'
            << " - overall ratio:     " << r.overall_ratio() << '\n'
            << " - overall fraction:  " << r.overall_fraction();
        return out;
    }

private:
    int n, num;
    str dataset_name;
    double g_recall, g_precision, g_ratio, g_fraction{};
};

#endif //LSH_RESULT_H
