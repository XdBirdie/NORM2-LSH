#include <iostream>
#include <fstream>
#include <vector>
#include "LSH.h"
#include "LinearScan.h"
#include "data.h"
#include "result.h"

int main() {
    /**
     * the parameters we need
     */
    int n = 70000, qn = 100, d = 500;
    int top_k = 100, candidate_num = n / 5;
    double w = 50;

    // out path
    std::string data_path = "/home/rain/Project/LSH/data/gen";
    std::string out_path = "/home/rain/Project/LSH/out";

    /**
     * generate data to test our lsh method
     */
    Generator<double> d_gen(n, d);
    Generator<double> q_gen(qn, d);
    auto data = d_gen();
    auto query = q_gen();

    // save data we generate
    d_gen.save_to(data_path + "/gen.ds");
    q_gen.save_to(data_path + "/gen.q");


    /**
     * generate ground truth and save ground truth
     */
    LinearScan<double> ls(n, d, data);
    MinQueue<double> res(top_k);

    std::vector<MinQueue<double>> gt;

    std::ofstream fout(data_path + "/gen.gt");
    for (int i = 0; i < qn; ++i) {
        res.clear();
        ls.nns(&query[i*d], res);
        gt.push_back(res);
        fout << i << ": " << std::fixed << res << '\n';
        std::cout << i << ": " << std::fixed << res << '\n';
    }
    fout.close();

//     test neg query
//    neg_data(qn * d, query);

    std::cout << "-------------------------\n"
              << "|          LSH          |\n"
              << "-------------------------\n\n";
    /**
     * test our LSH method
     */
    LSH<double, 128> lsh(n, d, w, data);
    std::cout << "Index over!" << std::endl;

    std::ofstream fo(out_path + "/res.out");
    Record<double> record(n, "Generated Data");
    for (int i = 0; i < qn; ++i) {
        res.clear();
        int check_k = lsh.top_k(candidate_num, &query[i*d], res);
        std::cout << i << ": " << std::fixed << res << '\n';
        fo << i << ": " << std::fixed << res << std::endl;
        record(top_k, check_k, gt[i].ptr(), res);
    }
    std::cout << record << '\n';
    fo.close();
    return 0;
}
