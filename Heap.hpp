#ifndef HEAP_HPP
#define HEAP_HPP

#include "HashMap.hpp"
#include <stdexcept>
#include <vector>

template<typename T>
struct less {
    // returns true if lhs < rhs
    constexpr bool operator()(const T &lhs, const T &rhs) const {
        return lhs < rhs;
    }
};

template<typename T>
struct greater {
    // returns true if lhs > rhs
    constexpr bool operator()(const T &lhs, const T &rhs) const {
        return lhs > rhs;
    }
};


template<typename K, typename V>
struct Element {
    // heap element with priority v
    K key;
    V value;

    Element(K k, V v) {
        key = k;
        value = v;
    }
};

template<typename K, typename V, typename Compare>
class IndexedHeap {
    // Implementation of Indexed Heap
private:
    std::vector<Element<K, V> > heap; // ACBT array
    HashMap<K, int> ind_map; // internal hashmap to get index of array by key
    Compare comp; // custom comparator

    static int parent(int i) {
        // get index of parent of index i
        return (i - 1) / 2;
    }

    int left(int i) {
        // get index of left child of index i
        return (2 * i + 1);
    }

    int right(int i) {
        // get index of right child of index i
        return (2 * i + 2);
    }

    void swap(int i, int j) {
        // swaps elements in indices i and j
        ind_map.insert(heap[i].key, j);
        ind_map.insert(heap[j].key, i);
        std::swap(heap[i], heap[j]);
    }

    void sift_up(int i) {
        // trickles up element to correct position
        while (i > 0 && comp(heap[i].value, heap[parent(i)].value)) {
            swap(i, parent(i));
            i = parent(i);
        }
    }

    void sift_down(int i) {
        // trickles down element to correct position
        while (i < heap.size()) {
            int child;
            if (left(i) >= heap.size() && right(i) >= heap.size()) {
                break;
            } else if (left(i) < heap.size() && right(i) < heap.size()) {
                if (comp(heap[left(i)].value, heap[right(i)].value)) {
                    child = left(i);
                } else {
                    child = right(i);
                }
            } else if (left(i) < heap.size()) {
                child = left(i);
            } else if (right(i) < heap.size()) {
                child = right(i);
            }
            if (comp(heap[child].value, heap[i].value)) {
                swap(i, child);
                i = child;
            } else {
                break;
            }
        }
    }

public:
    size_t size() const {
        // get size of internal heap array
        return heap.size();
    }

    bool empty() const {
        // if heap is empty
        return heap.empty();
    }

    bool contains(const K &key) const {
        // if key is present in heap
        return ind_map.count(key);
    }

    const V &get(const K &key) const {
        // returns value of key if present in heap
        return heap[ind_map.get(key)].value;
    }

    void push(const K &key, const V &value) {
        // inserts a key:value pair if key is not present previously.
        if (ind_map.count(key)) {
            throw std::invalid_argument("Key already exists in Heap.");
        }
        heap.push_back({key, value});
        int i = heap.size() - 1;
        ind_map.insert(key, i);
        sift_up(i);
    }

    void pop() {
        // deletes key:value with the highest priority
        if (empty()) {
            throw std::out_of_range("Heap is empty.");
        }
        K todel = heap[0].key;
        swap(0, heap.size() - 1);
        heap.pop_back();
        ind_map.remove(todel);
        sift_down(0);
    }

    const Element<K, V> &top() const {
        // returns element with the highest priority
        if (empty()) {
            throw std::out_of_range("Heap is empty.");
        }
        return heap[0];
    }

    void update(const K &key, const V &new_value) {
        // updates previously existing key with new priority
        if (!(contains(key))) {
            throw std::out_of_range("Key not in heap.");
        }
        int i = ind_map.get(key);
        V old_value = heap[i].value;
        heap[i].value = new_value;
        if (comp(new_value, old_value)) {
            sift_up(i);
        } else {
            sift_down(i);
        }
    }

    void build(const std::vector<Element<K, V> > &items) {
        // build heap from array of elements in O(n)
        heap = items;
        ind_map.clear();
        for (int i = 0; i < heap.size(); i++) {
            ind_map.insert(heap[i].key, i);
        }
        for (int i = parent(heap.size() - 1); i >= 0; --i) {
            sift_down(i);
        }
    }

    void append(const std::vector<Element<K, V> > &items) {
        // append array of elements to heap in O(n)
        for (int i = 0; i < items.size(); i++) {
            ind_map.insert(items[i].key, i + size());
        }
        heap.insert(heap.end(), items.begin(), items.end());
        for (int i = parent(heap.size() - 1); i >= 0; --i) {
            sift_down(i);
        }
    }
};

#endif
