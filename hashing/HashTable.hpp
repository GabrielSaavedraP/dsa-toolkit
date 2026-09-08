#include <iostream>
using namespace std;

// my_map<key_type, value_type>: hash table with separate chaining.
//
// HOW IT WORKS
// We keep an array of `m` "buckets" (chains[0..m-1]). Each bucket is
// the HEAD of its own tiny linked list of Entry nodes. To find/insert
// a key:
//   1) Hash the key to get a bucket index (0 to m-1) via _hash().
//   2) Walk that bucket's chain comparing keys one by one.
//   3) If found, return/update it. If not found (walked off the end,
//      cur == nullptr), that's a "miss" -- for operator[], we create
//      a brand new Entry and make it the new head of that chain.
// Two different keys landing in the SAME bucket is called a
// "collision" -- we don't fight it, we just let that bucket's chain
// grow longer. That's the whole idea behind "separate chaining":
// separate the colliding keys into their own list instead of
// overwriting each other.
//
// HOW TO IMPLEMENT IT (the recipe, step by step):
//   a) Define a small Entry node: key, value, and a pointer to the
//      next Entry in the SAME bucket's chain (this is just a singly
//      linked list node, nothing new).
//   b) Allocate `chains` as an array of `m` pointers (Entry*), all
//      starting at nullptr (m empty chains).
//   c) _hash(key): turn the key into a number in [0, m), by any
//      deterministic function -- here we do it digit by digit for
//      integers, since it's an int key, mixing each digit in with a
//      multiplier B so different digit patterns spread across buckets.
//   d) operator[](key): hash to find the right bucket, then WALK the
//      chain with a raw pointer (`cur = chains[i]; while (cur && ...)
//      cur = cur->next;`), comparing `cur->key != key` at each step.
//      If you fall off the chain without finding it, prepend a new
//      Entry to that bucket (new node's `next` = the old head, then
//      the bucket now points at the new node) -- prepending is O(1),
//      no need to walk to the end.
//   e) has_key(key): identical walk, but you only need a yes/no
//      answer at the end (cur == nullptr means "not found").
//   f) Don't forget a destructor that walks and deletes every Entry in
//      every bucket, plus `delete[] chains` -- otherwise every bucket
//      list you built with `new` leaks memory when the table is destroyed.
//
// Complexity: operator[] / has_key   O(1) average (assuming the hash
// spreads keys evenly across the m buckets, so each chain stays
// short), but O(n) worst case if every key collided into one bucket
// (a bad hash function, or m too small for how many keys you insert).
// This version does NOT auto-resize when chains get long -- if you
// need that guarantee, add a rehash step like in HashTable.hpp from
// the toolkit (doubles m and reinserts everything once load factor
// gets too high).
template <typename key_type, typename value_type>
struct my_map {
    struct Entry {
        key_type key;
        value_type value;
        Entry* next;
        Entry(const key_type& k, const value_type& v, Entry* nxt)
            : key(k), value(v), next(nxt) {}
    };

    int m;
    Entry** chains; // array of m linked-list heads (own nodes, not std::vector)

    my_map(int m) : m(m) {
        chains = new Entry*[m];
        for (int i = 0; i < m; ++i) chains[i] = nullptr;
    }

    ~my_map() {
        for (int i = 0; i < m; ++i) {
            Entry* cur = chains[i];
            while (cur) { Entry* nxt = cur->next; delete cur; cur = nxt; }
        }
        delete[] chains;
    }

    value_type& operator[](const key_type& key) {
        int chain_position = _hash(key);
        Entry* cur = chains[chain_position];
        while (cur != nullptr && cur->key != key) cur = cur->next;
        if (cur == nullptr) {
            // prepend: O(1), no need to reach the end of the chain
            chains[chain_position] = new Entry(key, value_type(), chains[chain_position]);
            cur = chains[chain_position];
        }
        return cur->value;
    }

    bool has_key(const key_type& key) const {
        Entry* cur = chains[_hash(key)];
        while (cur != nullptr && cur->key != key) cur = cur->next;
        return cur != nullptr;
    }

    int _hash(key_type key) const {
        // Para enteros: mezcla dígito por dígito con un multiplicador B
        const int B = 311;
        const int MOD = 1e9 + 7;
        int hash_value = 0;
        while (key > 0) {
            int d = key % 10;
            hash_value = (1ll * hash_value * B + (d + 1)) % MOD;
            key /= 10;
        }
        return hash_value % m;
    }

    void print() {
        for (int i = 0; i < m; ++i) {
            cout << "Bucket " << i << ": " << endl;
            for (Entry* cur = chains[i]; cur != nullptr; cur = cur->next) {
                cout << cur->key << " --> " << cur->value << endl;
            }
            cout << "End bucket" << endl;
        }
    }
};

int main() {
    my_map<int, long long> hash_table(20);
    for (int i = 0; i < 100; ++i) {
        hash_table[i] = 21;
    }
    hash_table.print();
    return 0;
}