#pragma once
#include <stdexcept>

// HashTable<K,V>: separate chaining, own linked-list buckets (no
// std::unordered_map / std::list involved).
//
// HOW IT WORKS
// We keep an array of `capacity_` buckets. Each bucket is the head of
// a tiny singly linked list of (key,value) entries. To find a key: 1)
// hash it to a bucket index, 2) walk that bucket's short chain
// comparing keys. Collisions (two keys landing in the same bucket)
// are handled by just growing that bucket's chain — hence "separate
// chaining". When the load factor (size_/capacity_) gets too high, we
// double capacity_ and re-insert everything (rehash), which is what
// keeps chains short and operations close to O(1) on average.
//
// Complexity (average case, good hash + low load factor):
//   insert / get / remove / contains   O(1) average, O(n) worst case
//   (worst case = every key collides into the same bucket)
template <typename K, typename V>
class HashTable {
private:
    struct Entry {
        K key;
        V value;
        Entry* next;
        Entry(const K& k, const V& v) : key(k), value(v), next(nullptr) {}
    };

    Entry** buckets_;
    int capacity_;
    int size_;

    // Simple polynomial-ish hash for generic keys via a user-supplied
    // hash function object would be more "correct", but for exam use
    // we specialize with a free function overload set below.
    int indexFor(const K& key) const {
        unsigned long h = hashKey(key);
        return (int)(h % (unsigned long)capacity_);
    }

    static unsigned long hashKey(int key) { return (unsigned long)key; }
    static unsigned long hashKey(const char* key) {
        unsigned long h = 5381;
        while (*key) h = h * 33 + (unsigned char)(*key++);
        return h;
    }

    void rehash() {
        int oldCap = capacity_;
        Entry** oldBuckets = buckets_;

        capacity_ *= 2;
        buckets_ = new Entry*[capacity_];
        for (int i = 0; i < capacity_; i++) buckets_[i] = nullptr;

        for (int i = 0; i < oldCap; i++) {
            Entry* cur = oldBuckets[i];
            while (cur) {
                Entry* nxt = cur->next;
                int idx = indexFor(cur->key);
                cur->next = buckets_[idx];
                buckets_[idx] = cur;
                cur = nxt;
            }
        }
        delete[] oldBuckets;
    }

public:
    explicit HashTable(int initialCapacity = 16)
        : capacity_(initialCapacity), size_(0) {
        buckets_ = new Entry*[capacity_];
        for (int i = 0; i < capacity_; i++) buckets_[i] = nullptr;
    }

    ~HashTable() {
        for (int i = 0; i < capacity_; i++) {
            Entry* cur = buckets_[i];
            while (cur) { Entry* nxt = cur->next; delete cur; cur = nxt; }
        }
        delete[] buckets_;
    }

    void insert(const K& key, const V& value) {
        if (size_ + 1 > capacity_ * 0.75) rehash(); // keep load factor <= 0.75

        int idx = indexFor(key);
        Entry* cur = buckets_[idx];
        while (cur) {
            if (cur->key == key) { cur->value = value; return; } // update existing
            cur = cur->next;
        }
        Entry* n = new Entry(key, value);
        n->next = buckets_[idx];
        buckets_[idx] = n;
        size_++;
    }

    bool contains(const K& key) const {
        int idx = indexFor(key);
        Entry* cur = buckets_[idx];
        while (cur) { if (cur->key == key) return true; cur = cur->next; }
        return false;
    }

    V& get(const K& key) {
        int idx = indexFor(key);
        Entry* cur = buckets_[idx];
        while (cur) { if (cur->key == key) return cur->value; cur = cur->next; }
        throw std::runtime_error("key not found");
    }

    void remove(const K& key) {
        int idx = indexFor(key);
        Entry* cur = buckets_[idx];
        Entry* prev = nullptr;
        while (cur) {
            if (cur->key == key) {
                if (prev) prev->next = cur->next; else buckets_[idx] = cur->next;
                delete cur;
                size_--;
                return;
            }
            prev = cur;
            cur = cur->next;
        }
    }

    int size() const { return size_; }
    bool empty() const { return size_ == 0; }
};
