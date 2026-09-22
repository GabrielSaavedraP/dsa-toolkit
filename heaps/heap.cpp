#include<bits/stdc++.h>
using namespace::std;

template<typename data_type>
struct Heap {
    int _size;
    vector<data_type> a;

    Heap(int _size = 0) : _size(_size) {
        a = vector<data_type>();
    }

    Heap(const vector<data_type> a) : a(move(a)), _size(a.size()) {
        build_heap();
    }

    int left(int p) {
        return (p << 1) | 1; // 2 * p + 1
    }

    int right(int p) {
        return (p << 1) + 2; // 2 * p + 2
    }

    int parent(int p) {
        return (p - 1) >> 1; // floor((p - 1) / 2)
    }

    void sift_down(int p) {
        while (left(p) < _size) {
            int m = p;
            int l = left(p), r = right(p);
            if (l < _size and a[l] < a[m]) {
                m = l;
            }
            if (r < _size and a[r] < a[m]) {
                m = r;
            }
            if (m == p) break;
            swap(a[p], a[m]);
            p = m;
        }
    }

    void sift_up(int u) {
        while (u > 0 and a[parent(u)] > a[u]) {
            swap(a[parent(u)], a[u]);
            u = parent(u);
        }
    }

    void build_heap() {
        for (int i = parent(_size - 1); i >= 0; --i) {
            sift_down(i);
        }
    }

    data_type top() const {
        return a[0];
    }

    void pop() {
        swap(a[0], a[_size - 1]);
        --_size;
        sift_down(0);
    }

    data_type extract_min() {
        data_type min = a[0];
        pop();
        return min;
    }

    void insert(data_type value) {
        a.emplace_back(value);
        ++_size;
        sift_up(_size - 1);
    }
};

int main() {
    cin.tie(0) -> sync_with_stdio(false);
    
    return 0;
}