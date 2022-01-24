//
// Created by rain on 2022/1/20.
//

#ifndef LSH_LIST_H
#define LSH_LIST_H

#include "util.h"


template <typename DType>
class MinQueue {
public:
    using Item = Result<DType>;
    explicit MinQueue(int max_size): max_size(max_size), cnt(0){
        list = new Item[max_size];
    }

    MinQueue(const MinQueue<DType> &other) {
        max_size = other.max_size;
        cnt = other.cnt;
        list = new Item [max_size];
        for (int i = 0; i < max_size; ++i)
            list[i] = other[i];
    }

    ~MinQueue() {delete[] list;}

    void clear() {cnt = 0;}

    Item operator[](int i) const {return list[i];}
    Item &operator[](int i) {return list[i];}

    Item back() const {return list[cnt - 1];}
    Item &back() {return list[cnt - 1];}

    bool full() const {return cnt == max_size;}
    bool empty() const {return cnt == 0;}
    size_t size() const {return cnt;}

    void insert(int id, DType value) {
        if (!full()) {
            list[cnt++] = {id, value};
        } else if (value < back().value) {
            back() = {id, value};
        }
        int i = cnt - 1;
        while (i > 0 && list[i].value < list[i - 1].value) {
            std::swap(list[i], list[i - 1]);
            --i;
        }
    }

    const Item *ptr() const {
        return list;
    }
    void copy_to(Item *other) const {
        for (int i = 0; i < cnt; ++i) other[i] = list[i];
    }

    using os = std::ostream;
    friend os &operator<<(os &out, const MinQueue<DType> &l) {
        for (int i = 0; i < l.cnt; ++i)
            out << l.list[i] << ", ";
        return out;
    }

protected:
    int max_size;
    int cnt;
    Item *list;
};

template <typename DType>
class MaxQueue {
public:
    using Item = Result<DType>;
    explicit MaxQueue(int max_size): max_size(max_size), cnt(0){
        list = new Item[max_size];
    }

    ~MaxQueue() {delete[] list;}

    void clear() {cnt = 0;}

    Item operator[](int i) const {return list[i];}
    Item &operator[](int i) {return list[i];}

    Item back() const {return list[cnt - 1];}
    Item &back() {return list[cnt - 1];}

    bool full() const {return cnt == max_size;}
    bool empty() const {return cnt == 0;}
    size_t size() const {return cnt;}

    void insert(int id, DType value) {
        if (!full()) {
            list[cnt++] = {id, value};
        } else if (value > back().value) {
            back() = {id, value};
        }
        int i = cnt - 1;
        while (i > 0 && list[i].value > list[i - 1].value) {
            std::swap(list[i], list[i - 1]);
            --i;
        }
    }

    using os = std::ostream;
    friend os &operator<<(os &out, const MaxQueue<DType> &l) {
        for (int i = 0; i < l.cnt; ++i)
            out << l.list[i] << ", ";
        return out;
    }

protected:
    int max_size;
    int cnt;
    Result<DType> *list;
};

#endif //LSH_LIST_H

