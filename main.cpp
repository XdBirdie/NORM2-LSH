#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include "LSH.h"
#include "LinearScan.h"
#include "data.h"
#include "result.h"



int main() {
    /**
     * the parameters we need
     */
    int n = 70000, qn = 100, d = 50;
    int top_k = 100, candidate_num = n / 5;
    double w = 10;

    // out path
    std::string data_path = "/home/rain/Project/LSH/data/gen";
    std::string out_path = "/home/rain/Project/LSH/out";

    /**
     * generate data to test our lsh method
     */
    DataSet<double> ds(n, qn, d, top_k);
    ds.save_to(data_path + "2", "gen2");
    auto data = ds.get_data_set();
    auto query = ds.get_query_set();
    auto gt = ds.get_gt_set();

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
    MinQueue<double> res(top_k);

    fo << std::fixed << std::setprecision(10);
    for (int i = 0; i < qn; ++i) {
        res.clear();
        int check_k = lsh.top_k(candidate_num, &query[i*d], res);
        fo << i << ": " << res << std::endl;
        record(top_k, check_k, &gt[i * top_k], res);
    }
    std::cout << record << '\n';
    fo.close();
    return 0;
}
